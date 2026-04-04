#include "codexion.h"

int		simulation_stopped(t_data *data)
{
	int data_stop;

	if (pthread_mutex_lock(&data->stop_mutex) != 0)
		return (0);
	data_stop = data->stop;
	pthread_mutex_unlock(&data->stop_mutex);
	return (data_stop);
}

void	stop_simulation(t_data *data)
{
	int	i;

	i = 0;
	if (pthread_mutex_lock(&data->stop_mutex) != 0)
		return ;
	data->stop = 1;
	pthread_mutex_unlock(&data->stop_mutex);

	while (i < data->number_of_coders)
	{
		pthread_mutex_lock(&data->dongles[i].mutex);
		pthread_cond_broadcast(&data->dongles[i].cond);
		pthread_mutex_unlock(&data->dongles[i].mutex);
		i++;
	}
}

int		all_coders_done(t_data *data)
{
	int	i;
	int	done;

	done = 0;
	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_lock(&data->coders[i].mutex);
		if (data->coders[i].compile_count >= data->number_of_compiles_required)
			done++;
		pthread_mutex_unlock(&data->coders[i].mutex);
		i++;
	}
	if (done >= data->number_of_coders)
		return (1);
	return (0);
}

int		has_burnout(t_data *data)
{
	int	i;
	long ts;

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_lock(&data->coders[i].mutex);
		if (data->coders[i].last_compile_start == 0)
			ts = get_timestamp(data->start_time);
		else
			ts = get_timestamp(data->coders[i].last_compile_start);
		if (ts > data->time_to_burnout && data->coders[i].compile_count < data->number_of_compiles_required)
		{
			log_action(&data->coders[i], "burned out");
			pthread_mutex_unlock(&data->coders[i].mutex);
			return (1);
		}
		pthread_mutex_unlock(&data->coders[i].mutex);
		i++;
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (!simulation_stopped(data))
	{
		if (has_burnout(data))
		{
			stop_simulation(data);
			return (NULL);
		}
		if (all_coders_done(data))
		{
			stop_simulation(data);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}
#include "codexion.h"

void	coder_action(t_coder *coder, int action)
{
	if (action == 1)
	{
		pthread_mutex_lock(&coder->mutex);
		coder->last_compile_start = get_time_ms();
		pthread_mutex_unlock(&coder->mutex);
		log_action(coder, "is compiling");
		ms_sleep(coder->data->time_to_compile, coder->data);
		pthread_mutex_lock(&coder->mutex);
		coder->compile_count++;
		pthread_mutex_unlock(&coder->mutex);
		drop_dongles(coder);
	}
	else if (action == 2)
	{
		log_action(coder, "is debugging");
		ms_sleep(coder->data->time_to_debug, coder->data);
	}
	else if (action == 3)
	{
		log_action(coder, "is refactoring");
		ms_sleep(coder->data->time_to_refactor, coder->data);
	}
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	int		i;
	int		done;

	coder = (t_coder *)arg;
	while (!simulation_stopped(coder->data))
	{
		pthread_mutex_lock(&coder->mutex);
		done = (coder->compile_count >= coder->data->number_of_compiles_required);
		pthread_mutex_unlock(&coder->mutex);
		if (done)
			break ;
		if (!take_dongles(coder))
			break ;
		i = 1;
		while (i < 4 && !simulation_stopped(coder->data))
		{
			coder_action(coder, i);
			i++;
		}
	}
	return (NULL);
}

int		create_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		if (pthread_create(&data->coders[i].thread, NULL, coder_routine, &data->coders[i]) != 0)
			return (0);
		i++;
	}
	if (pthread_create(&data->monitor, NULL, monitor_routine, data) != 0)
		return (0);
	i = 0;
	while (i < data->number_of_coders)
	{
		if (pthread_join(data->coders[i].thread, NULL) != 0)
			return (0);
		i++;
	}
	if (pthread_join(data->monitor, NULL) != 0)
			return (0);

	return (1);
}

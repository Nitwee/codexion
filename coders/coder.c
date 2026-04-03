#include "codexion.h"

void	*coder_routine(void *arg)
{
	t_coder *coder;

	coder = (t_coder *)arg;
	while (!simulation_stopped(coder->data))
	{
		while (!simulation_stopped(coder->data) && !take_dongles(coder))
			ms_sleep(1, coder->data);
		if (simulation_stopped(coder->data))
			return (NULL);
		coder->last_compile_start = get_time_ms();
		log_action(coder, "is compiling");
		ms_sleep(coder->data->time_to_compile, coder->data);
		coder->compile_count++;
		drop_dongles(coder);
		if (simulation_stopped(coder->data))
			return (NULL);
		log_action(coder, "is debugging");
		ms_sleep(coder->data->time_to_debug, coder->data);
		if (simulation_stopped(coder->data))
			return (NULL);
		log_action(coder, "is refactoring");
		ms_sleep(coder->data->time_to_refactor, coder->data);
		if (simulation_stopped(coder->data))
			return (NULL);
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

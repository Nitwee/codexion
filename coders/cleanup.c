#include "codexion.h"

void	destroy_data(t_data *data, int param)
{
	int	i;

	i = 0;
	if (!data)
		return ;
	while (i < data->number_of_coders)
	{
		if (data->dongles && i < data->dongles_created)
		{
			pthread_mutex_destroy(&data->dongles[i].mutex);
			pthread_cond_destroy(&data->dongles[i].cond);
			free(data->dongles[i].heap.reqs);
		}
		if (data->coders && i < data->coders_created)
			pthread_mutex_destroy(&data->coders[i].mutex);
		i++;
	}
	if (param > 1)
		pthread_mutex_destroy(&data->stop_mutex);
	if (param > 2)
		pthread_mutex_destroy(&data->print_mutex);
	if (data->dongles)
		free(data->dongles);
	if (data->coders)
		free(data->coders);
}

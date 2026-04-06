#include "codexion.h"

int	create_single_coder(t_data *data, t_coder *coders, int i)
{
	coders[i].id = i + 1;
	coders[i].compile_count = 0;
	coders[i].last_compile_start = 0;
	coders[i].data = data;
	if (pthread_mutex_init(&coders[i].mutex, NULL) != 0)
		return (0);
	coders[i].left = &data->dongles[i];
	if (data->number_of_coders == 1)
		coders[i].right = &data->dongles[i];
	else
	{
		if (i < data->number_of_coders - 1)
			coders[i].right = &data->dongles[i + 1];
		else
			coders[i].right = &data->dongles[0];
	}
	return (1);
}

int	create_coders(t_data *data)
{
	int	i;

	data->coders = malloc(sizeof(t_coder) * data->number_of_coders);
	if (!data->coders)
		return (0);
	i = 0;
	while (i < data->number_of_coders)
	{
		if (!create_single_coder(data, data->coders, i))
			return (0);
		data->coders_created += 1;
		i++;
	}
	return (1);
}

int	create_dongles(t_data *data)
{
	int	i;

	i = 0;
	data->dongles = malloc(sizeof(t_dongle) * data->number_of_coders);
	if (!data->dongles)
		return (0);
	while (i < data->number_of_coders)
	{
		data->dongles[i].taken = 0;
		data->dongles[i].cooldown_until = 0;
		if (pthread_mutex_init(&data->dongles[i].mutex, NULL) != 0)
			return (0);
		if (pthread_cond_init(&data->dongles[i].cond, NULL) != 0)
			return (0);
		if (!init_heap(&data->dongles[i].heap, data->number_of_coders))
			return (0);
		data->dongles_created += 1;
		i++;
	}
	return (1);
}

int	init_data(t_data *data)
{
	define_base_data(data);
	if (pthread_mutex_init(&data->stop_mutex, NULL) != 0)
	{
		destroy_data(data, 1);
		return (0);
	}
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
	{
		destroy_data(data, 2);
		return (0);
	}
	if (!create_dongles(data))
	{
		destroy_data(data, 3);
		return (0);
	}
	if (!create_coders(data))
	{
		destroy_data(data, 3);
		return (0);
	}
	return (1);
}

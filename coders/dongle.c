#include "codexion.h"

int	check_add_heap(t_coder *coder, t_dongle *dongle, int *added)
{
	if (!coder_in_heap(&dongle->heap, coder))
	{
		if (!heap_add(&dongle->heap, coder))
		{
			pthread_mutex_unlock(&dongle->mutex);
			return (0);
		}
		*added = 1;
	}
	return (1);
}

int	check_end_dongle(t_coder *coder, t_dongle *dongle, int added)
{
	if (simulation_stopped(coder->data))
	{
		if (added)
			remove_from_heap(&dongle->heap, coder);
		pthread_mutex_unlock(&dongle->mutex);
		return (0);
	}
	return (1);
}

int	get_dongle(t_coder *coder, t_dongle *dongle)
{
	struct timespec	ts;
	int				added;

	added = 0;
	if (pthread_mutex_lock(&dongle->mutex) != 0)
		return (0);
	if (!check_add_heap(coder, dongle, &added))
		return (0);
	while (!simulation_stopped(coder->data) && !can_take_dongle(dongle, coder))
	{
		if (dongle->cooldown_until > get_time_ms())
		{
			ms_to_timespec(dongle->cooldown_until, &ts);
			pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &ts);
		}
		else
			pthread_cond_wait(&dongle->cond, &dongle->mutex);
	}
	if (!check_end_dongle(coder, dongle, added))
		return (0);
	heap_pop(&dongle->heap, coder->data->scheduler);
	dongle->taken = 1;
	pthread_mutex_unlock(&dongle->mutex);
	log_action(coder, "has taken a dongle");
	return (1);
}

int	take_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	if (coder->data->number_of_coders == 1)
	{
		if (!get_dongle(coder, coder->left))
			return (0);
		while (!simulation_stopped(coder->data))
			usleep(500);
		release_dongle(coder->left, coder->data);
		return (0);
	}
	set_dongle_order(coder, &first, &second);
	if (!get_dongle(coder, first))
		return (0);
	if (!get_dongle(coder, second))
	{
		release_dongle(first, coder->data);
		return (0);
	}
	return (1);
}

#include "codexion.h"

int	coder_in_heap(t_heap *heap, t_coder *coder)
{
	int	i;

	i = 0;
	while (i < heap->size)
	{
		if (heap->reqs[i].coder == coder)
			return (1);
		i++;
	}
	return (0);
}

int	can_take_dongle(t_dongle *dongle, t_coder *coder)
{
	if (dongle->taken)
		return (0);
	if (dongle->cooldown_until > get_time_ms())
		return (0);
	if (dongle->heap.size == 0)
		return (0);
	if (dongle->heap.reqs[0].coder != coder)
		return (0);
	return (1);
}

void	set_dongle_order(t_coder *coder, t_dongle **first, t_dongle **second)
{
	if (coder->id == 1)
	{
		*first = coder->right;
		*second = coder->left;
	}
	else
	{
		*first = coder->left;
		*second = coder->right;
	}
}

int	release_dongle(t_dongle *dongle, t_data *data)
{
	if (pthread_mutex_lock(&dongle->mutex) != 0)
		return (0);
	dongle->taken = 0;
	dongle->cooldown_until = get_time_ms() + data->dongle_cooldown;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
	return (1);
}

void	drop_dongles(t_coder *coder)
{
	release_dongle(coder->left, coder->data);
	if (coder->data->number_of_coders == 1)
		return ;
	release_dongle(coder->right, coder->data);
}

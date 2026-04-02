#include "codexion.h"


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

int	handle_single_coder(t_coder *coder)
{
	if (pthread_mutex_lock(&coder->left->mutex) != 0)
		return (0);
	coder->left->taken = 1;
	log_action(coder, "has taken a dongle");
	pthread_mutex_unlock(&coder->left->mutex);
	return (0); // A REVOIR
}

int	take_first_dongle(t_coder *coder, t_dongle *first)
{
	if (pthread_mutex_lock(&first->mutex) != 0)
		return (0);
	if (first->taken == 1)
	{
		pthread_mutex_unlock(&first->mutex);
		return (0);
	}
	first->taken = 1;
	log_action(coder, "has taken a dongle");
	return (1);
}

int	take_second_dongle(t_coder *coder, t_dongle *first, t_dongle *second)
{
	if (pthread_mutex_lock(&second->mutex) != 0)
	{
		first->taken = 0;
		pthread_mutex_unlock(&first->mutex);
		return (0);
	}
	if (second->taken == 1)
	{
		first->taken = 0;
		pthread_mutex_unlock(&first->mutex);
		pthread_mutex_unlock(&second->mutex);
		return (0);
	}
	second->taken = 1;
	log_action(coder, "has taken a dongle");
	return (1);
}

int	take_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	if (coder->data->number_of_coders == 1)
		return (handle_single_coder(coder));
	set_dongle_order(coder, &first, &second);
	if (!take_first_dongle(coder, first))
		return (0);
	if (!take_second_dongle(coder, first, second))
		return (0);
	return (1);
}


void	drop_dongles(t_coder *coder)
{
	coder->left->taken = 0;
	pthread_mutex_unlock(&coder->left->mutex);
	if (coder->data->number_of_coders == 1)
		return;
	coder->right->taken = 0;
	pthread_mutex_unlock(&coder->right->mutex);
}
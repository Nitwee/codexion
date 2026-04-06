#include "codexion.h"

void	define_base_data(t_data *data)
{
	data->stop = 0;
	data->start_time = 0;
	data->coders = NULL;
	data->coders_created = 0;
	data->dongles = NULL;
	data->dongles_created = 0;
}

int	init_heap(t_heap *heap, int capacity)
{
	heap->reqs = malloc(sizeof(t_request) * capacity);
	if (!heap->reqs)
		return (0);
	heap->size = 0;
	heap->capacity = capacity;
	heap->reqs_count = 0;
	return (1);
}

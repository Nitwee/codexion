#include "codexion.h"

int	heap_add(t_heap *heap, t_coder *coder)
{
	long	start;
	int		i;

	if (!heap || !coder)
		return (0);
	if (heap->size >= heap->capacity)
		return (0);
	i = heap->size;

	heap->reqs[i].coder = coder;
	heap->reqs[i].request_time = get_time_ms();
	if (coder->last_compile_start == 0)
		start = coder->data->start_time;
	else
		start = coder->last_compile_start;
	heap->reqs[i].deadline = start + coder->data->time_to_burnout;
	heap->reqs_count++;
	heap->reqs[i].req_nb = heap->reqs_count;
	heap->size++;
	repair_heap_up(heap, coder->data->scheduler, i);
	return (1);
}

int	remove_from_heap(t_heap *heap, t_coder *coder)
{
	int			i;

	i = 0;
	while (i < heap->size)
	{
		if (heap->reqs[i].coder == coder)
		{
			heap->reqs[i] = heap->reqs[heap->size - 1];
			heap->size--;
			if (i == heap->size)
				return (1);
			if (i > 0 && compare_requests(heap->reqs[i], heap->reqs[(i - 1) / 2], coder->data->scheduler))
				repair_heap_up(heap, coder->data->scheduler, i);
			else
				repair_heap_down(heap, coder->data->scheduler, i);
			return (1);
		}
		i++;
	}
	return (0);
}


t_request	heap_pop(t_heap *heap, int scheduler)
{
	t_request	res;

	res.coder = NULL;
	res.request_time = 0;
	res.deadline = 0;
	res.req_nb = 0;
	if (!heap || heap->size == 0)
		return (res);

	res = heap->reqs[0];
	heap->reqs[0] = heap->reqs[heap->size - 1];
	heap->size--;
	repair_heap_down(heap, scheduler, 0);

	return (res);
}

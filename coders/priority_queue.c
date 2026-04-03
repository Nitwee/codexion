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
	if (coder->data->scheduler == EDF)
		edf_swapper(heap, i);
	return (1);
}

int	edf_swapper(t_heap *heap, int pos)
{
	long	curr_ddline;
	long	prev_ddline;
	int		prev_pos;
	t_request	tmp;

	curr_ddline = heap->reqs[pos].deadline;
	while (pos > 0)
	{
		prev_pos = (pos - 1) / 2;
		prev_ddline = heap->reqs[prev_pos].deadline;
		if (curr_ddline > prev_ddline)
			break ;
		if (curr_ddline == prev_ddline
			&& heap->reqs[pos].req_nb > heap->reqs[prev_pos].req_nb)
			break ;
		tmp = heap->reqs[prev_pos];
		heap->reqs[prev_pos] = heap->reqs[pos];
		heap->reqs[pos] = tmp;
		pos = prev_pos;
		curr_ddline = heap->reqs[pos].deadline;
	}
	return (1);
}

int	compare_requests(t_request req_1, t_request req_2, int param)
{
	if (param == EDF)
	{
		if (req_1.deadline < req_2.deadline)
			return (1);
		else if (req_1.deadline == req_2.deadline)
			return (req_1.req_nb < req_2.req_nb);
		else
			return (0);
	}
	else
		return (req_1.req_nb < req_2.req_nb);
}

void	heap_pop_sort(t_heap *heap, int scheduler)
{
	int			pos;
	int			l_pos;
	int			r_pos;
	int			good_pos;
	t_request	tmp;

	pos = 0;
	while (1)
	{
		l_pos = 2 * pos + 1;
		r_pos = 2 * pos + 2;
		if (l_pos >= heap->size)
			break;
		good_pos = l_pos;
		if (r_pos < heap->size)
		{
			if (!compare_requests(heap->reqs[l_pos], heap->reqs[r_pos], scheduler))
				good_pos = r_pos;
		}
		if (compare_requests(heap->reqs[pos], heap->reqs[good_pos], scheduler))
			break ;
		tmp = heap->reqs[pos];
		heap->reqs[pos] = heap->reqs[good_pos];
		heap->reqs[good_pos] = tmp;
		pos = good_pos;
	}
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
	heap_pop_sort(heap, scheduler);

	return (res);
}

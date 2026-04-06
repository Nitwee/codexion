#include "codexion.h"

void	repair_heap_down(t_heap *heap, int sched, int pos)
{
	int			l_p;
	int			r_p;
	int			good_pos;
	t_request	tmp;

	while (1)
	{
		l_p = 2 * pos + 1;
		r_p = 2 * pos + 2;
		if (l_p >= heap->size)
			break ;
		good_pos = l_p;
		if (r_p < heap->size)
		{
			if (!compare_requests(heap->reqs[l_p], heap->reqs[r_p], sched))
				good_pos = r_p;
		}
		if (compare_requests(heap->reqs[pos], heap->reqs[good_pos], sched))
			break ;
		tmp = heap->reqs[pos];
		heap->reqs[pos] = heap->reqs[good_pos];
		heap->reqs[good_pos] = tmp;
		pos = good_pos;
	}
}

int	compare_requests(t_request req_1, t_request req_2, int scheduler)
{
	if (scheduler == EDF)
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

void	repair_heap_up(t_heap *heap, int scheduler, int pos)
{
	int			parent;
	t_request	tmp;

	while (1)
	{
		if (pos == 0)
			break ;
		parent = (pos - 1) / 2;
		if (!compare_requests(heap->reqs[pos], heap->reqs[parent], scheduler))
			break ;
		tmp = heap->reqs[parent];
		heap->reqs[parent] = heap->reqs[pos];
		heap->reqs[pos] = tmp;
		pos = parent;
	}
}

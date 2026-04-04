#include "codexion.h"

void	repair_heap_down(t_heap *heap, int scheduler, int pos)
{
	int			l_pos;
	int			r_pos;
	int			good_pos;
	t_request	tmp;

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
	int	parent;
	t_request tmp;

	while (1)
	{
		if (pos == 0)
			break;
		parent = (pos - 1) / 2;
		if (!compare_requests(heap->reqs[pos], heap->reqs[parent], scheduler))
			break;
		tmp = heap->reqs[parent];
		heap->reqs[parent] = heap->reqs[pos];
		heap->reqs[pos] = tmp;
		pos = parent;
	}
}
#include "codexion.h"


void	log_action(t_coder *coder, const char *msg)
{
	long time;

	pthread_mutex_lock(&coder->data->print_mutex);
	time = get_timestamp(coder->data->start_time);
	if (!simulation_stopped(coder->data))
		printf("%ld %d %s\n", time, coder->id, msg);
	pthread_mutex_unlock(&coder->data->print_mutex);

}

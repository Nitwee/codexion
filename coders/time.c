#include "codexion.h"

long	get_time_ms(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000L);
}

void	ms_to_timespec(long ms, struct timespec *ts)
{
	ts->tv_sec = ms / 1000;
	ts->tv_nsec = (ms % 1000) * 1000000;
}


long	get_timestamp(long start_time)
{
	long now;

	now = get_time_ms();
	return (now - start_time);
}
void	ms_sleep(long duration, t_data *data)
{
	long	start;

	start = get_time_ms();
	while (!simulation_stopped(data))
	{
		if (get_time_ms() - start >= duration)
			break ;
		usleep(500);
	}
}

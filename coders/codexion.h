#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>
# include <limits.h>

# define FIFO 1
# define EDF 2

typedef struct s_data	t_data;
typedef struct s_coder	t_coder;

typedef struct s_request
{
	t_coder	*coder;
	long	request_time;
	long	deadline;
	long	req_nb;
}	t_request;

typedef struct s_heap
{
	t_request	*reqs;
	int			size;
	int			capacity;
	long		reqs_count;
}	t_heap;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	int				taken;
	long			cooldown_until;
	t_heap			heap;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	pthread_t		thread;
	pthread_mutex_t	mutex;
	int				compile_count;
	long			last_compile_start;
	t_dongle		*left;
	t_dongle		*right;
	t_data			*data;
}	t_coder;

typedef struct s_data
{
	int				number_of_coders;
	long			time_to_burnout;
	long			time_to_compile;
	long			time_to_debug;
	long			time_to_refactor;
	int				number_of_compiles_required;
	long			dongle_cooldown;
	int				scheduler;
	long			start_time;
	int				stop;
	int				dongles_created;
	int				coders_created;
	pthread_t		monitor;
	pthread_mutex_t	stop_mutex;
	pthread_mutex_t	print_mutex;
	t_coder			*coders;
	t_dongle		*dongles;
}	t_data;



int		parse_args(int argc, char **argv, t_data *data);


int		init_data(t_data *data);
void	destroy_data(t_data *data, int param);


void	*coder_routine(void *arg);
int		create_threads(t_data *data);
void	*monitor_routine(void *arg);


int		take_dongles(t_coder *coder);
void	drop_dongles(t_coder *coder);


void	log_action(t_coder *coder, const char *msg);


long	get_time_ms(void);
long	get_timestamp(long start_time);
void	ms_sleep(long duration, t_data *data);


int		simulation_stopped(t_data *data);
void	stop_simulation(t_data *data);

int	edf_swapper(t_heap *heap, int pos);
int	heap_add(t_heap *heap, t_coder *coder);
t_request	heap_pop(t_heap *heap, int scheduler);


#endif

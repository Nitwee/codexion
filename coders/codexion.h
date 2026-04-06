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

int			parse_args(int argc, char **argv, t_data *data);
int			check_scheduler(char *arg);
int			is_valid_number(char *s);
int			check_numbers(char *s, int p);
int			check_args(int argc, char **argv, int *parsing_res);

int			init_data(t_data *data);
void		define_base_data(t_data *data);
void		destroy_data(t_data *data, int param);
int			create_single_coder(t_data *data, t_coder *coders, int i);
int			create_coders(t_data *data);
int			init_heap(t_heap *heap, int capacity);
int			create_dongles(t_data *data);

int			get_compile_count(t_coder *coder);
long		get_last_compile_start(t_coder *coder);
void		*coder_routine(void *arg);
int			create_threads(t_data *data);
void		coder_action(t_coder *coder, int action);
void		*monitor_routine(void *arg);
int			all_coders_done(t_data *data);
int			has_burnout(t_data *data);

int			check_add_heap(t_coder *coder, t_dongle *dongle, int *added);
int			check_end_dongle(t_coder *coder, t_dongle *dongle, int added);
int			coder_in_heap(t_heap *heap, t_coder *coder);
int			can_take_dongle(t_dongle *dongle, t_coder *coder);
void		set_dongle_order(t_coder *coder, t_dongle **first,
				t_dongle **second);
int			take_dongles(t_coder *coder);
int			get_dongle(t_coder *coder, t_dongle *dongle);
int			release_dongle(t_dongle *dongle, t_data *data);
void		drop_dongles(t_coder *coder);

void		log_action(t_coder *coder, const char *msg);

long		get_time_ms(void);
void		ms_to_timespec(long ms, struct timespec *ts);
long		get_timestamp(long start_time);
void		ms_sleep(long duration, t_data *data);

int			simulation_stopped(t_data *data);
void		stop_simulation(t_data *data);

int			heap_add(t_heap *heap, t_coder *coder);
int			remove_from_heap(t_heap *heap, t_coder *coder);
int			compare_requests(t_request req_1, t_request req_2, int scheduler);
t_request	heap_pop(t_heap *heap, int scheduler);

void		repair_heap_down(t_heap *heap, int scheduler, int pos);
void		repair_heap_up(t_heap *heap, int scheduler, int pos);

#endif

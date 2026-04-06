#include "codexion.h"

int	main(int argc, char **argv)
{
	t_data	data;

	if (!parse_args(argc, argv, &data))
		return (1);
	if (!init_data(&data))
		return (1);
	data.start_time = get_time_ms();
	if (!create_threads(&data))
	{
		destroy_data(&data, 3);
		return (1);
	}
	destroy_data(&data, 3);
	return (0);
}

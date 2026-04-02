#include "codexion.h"

int	check_scheduler(char *arg)
{
	if (strcmp(arg, "fifo") == 0)
		return (FIFO);
	if (strcmp(arg, "edf") == 0)
		return (EDF);
	fprintf(stderr, "Invalid arg %s\n", arg);
	return (-1);
}

int	is_valid_number(char *s)
{
	int	i;

	if (!s || s[0] == '\0')
		return (0);
	i = 0;
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	if (strlen(s) > 10)
		return (0);
	if (strlen(s) == 10 && strcmp(s, "2147483647") > 0)
		return (0);
	return (1);
}

int	check_numbers(char *s, int p)
{
	int	nb;

	nb = 0;
	if (is_valid_number(s))
	{
		nb = atoi(s);
		if ((nb > 0 && p < 6) || (nb >= 0 && p == 6))
			return (nb);
	}
	fprintf(stderr, "Invalid arg %s\n", s);
	return (-1);
}

int	check_args(int argc, char **argv, int *parsing_res)
{
	int	i;
	int	arg_check;

	i = 0;
	while (i < argc - 1)
	{
		if (i == 7)
			arg_check = check_scheduler(argv[i + 1]);
		else
			arg_check = check_numbers(argv[i + 1], i);
		if (arg_check == -1)
			return (0);
		parsing_res[i] = arg_check;
		i++;
	}
	return (1);
}

int	parse_args(int argc, char **argv, t_data *data)
{
	int	parsing_res[8];
	int	res;

	if (argc != 9)
	{
		fprintf(stderr, "Missing args %d\n", argc);
		return (0);
	}
	res = check_args(argc, argv, parsing_res);
	if (!res)
		return (0);
	data->number_of_coders = parsing_res[0];
	data->time_to_burnout = parsing_res[1];
	data->time_to_compile = parsing_res[2];
	data->time_to_debug = parsing_res[3];
	data->time_to_refactor = parsing_res[4];
	data->number_of_compiles_required = parsing_res[5];
	data->dongle_cooldown = parsing_res[6];
	data->scheduler = parsing_res[7];
	return (1);
}

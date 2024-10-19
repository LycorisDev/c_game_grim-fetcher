#include "grim_fetcher.h"

int			get_ber_fd(int argc, char *path);
char		**copy_ber(int *fd, char *path);

static int	are_symbols_valid(char **ber);
static int	are_symbols_in_right_amount(char **ber);
static int	is_map_rectangular_and_closed_off(char **ber);
static int	is_map_closed_off_by_walls(char **ber);

char	**get_ber_data(int argc, char *path)
{
	int		fd;
	char	**ber;

	fd = get_ber_fd(argc, path);
	if (fd < 0)
		return (0);
	ber = copy_ber(&fd, path);
	close(fd);
	if (!are_symbols_valid(ber) || !is_map_rectangular_and_closed_off(ber))
	{
		free_ber_data(ber);
		return (0);
	}
	return (ber);
}

static int	are_symbols_valid(char **ber)
{
	size_t	i;
	size_t	j;

	if (!ber)
		return (0);
	i = 0;
	while (ber[i])
	{
		j = 0;
		while (ber[i][j])
		{
			if (!ft_strchr("01BCEGP", ber[i][j], 0))
			{
				ft_dprintf(2, "Error: Unknown symbol '%c' (-> \"%s\")\n",
					ber[i][j], "01BCEGP");
				return (0);
			}
			++j;
		}
		++i;
	}
	return (are_symbols_in_right_amount(ber));
}

static int	are_symbols_in_right_amount(char **ber)
{
	int	amount[3];

	amount[0] = 0;
	amount[1] = 0;
	amount[2] = 0;
	while (*ber)
	{
		amount[0] += ft_strchr(*ber, 'C', 0) != 0;
		amount[1] += ft_strchr(*ber, 'E', 0) != 0;
		amount[2] += ft_strchr(*ber, 'P', 0) != 0;
		++ber;
	}
	if (amount[0] >= 1 && amount[1] == 1 && amount[2] == 1)
		return (1);
	if (!amount[0])
		ft_dprintf(2, "Error: No collectible found ('C')\n");
	if (!amount[1])
		ft_dprintf(2, "Error: No exit found ('E')\n");
	else if (amount[1] > 1)
		ft_dprintf(2, "Error: More than one exit found ('E')\n");
	if (!amount[2])
		ft_dprintf(2, "Error: No player start found ('P')\n");
	else if (amount[2] > 1)
		ft_dprintf(2, "Error: More than one player start found ('P')\n");
	return (0);
}

static int	is_map_rectangular_and_closed_off(char **ber)
{
	size_t	i;
	size_t	len;
	size_t	curr_len;

	i = 0;
	len = 0;
	while (ber[i])
	{
		curr_len = ft_strlen(ber[i]);
		if (!len)
			len = curr_len;
		else if (curr_len && len != curr_len)
		{
			ft_dprintf(2, "Error: Map is not rectangular\n");
			return (0);
		}
		++i;
	}
	if (!is_map_closed_off_by_walls(ber))
	{
		ft_dprintf(2, "Error: Map is not closed_off_by_walls\n");
		return (0);
	}
	return (1);
}

static int	is_map_closed_off_by_walls(char **ber)
{
	size_t	i;
	size_t	j;
	size_t	width;

	i = 0;
	width = ft_strlen(ber[0]);
	while (ber[i])
	{
		if (ber[i][0] != '1' || ber[i][width - 1] != '1')
			return (0);
		if (i == 0 || !ber[i + 1])
		{
			j = 0;
			while (j < width)
			{
				if (ber[i][j] != '1')
					return (0);
				++j;
			}
		}
		++i;
	}
	return (1);
}

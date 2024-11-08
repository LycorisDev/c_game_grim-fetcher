#include "grim_fetcher.h"

static int		get_ber_height(int *fd, char *path);
static size_t	ft_strlen_without_whitespaces(char *s);
static char		*ft_strdup_without_whitespaces(char *s);

int	get_ber_fd(int argc, char *path)
{
	int		fd;
	size_t	len;

	if (path)
		fd = open(path, O_RDONLY);
	len = ft_strlen(path);
	if (argc == 1)
		ft_dprintf(2, "Error: No argument specified -> *.ber file needed\n");
	else if (argc > 2)
		ft_dprintf(2, "Error: Too many arguments specified\n");
	else if (fd < 0)
		perror("Error");
	else if (len < 4 || ft_strcmp(&path[len - 4], ".ber"))
		ft_dprintf(2, "Error: File doesn't have *.ber extension\n");
	else
		return (fd);
	if (path)
		close(fd);
	return (-1);
}

char	**copy_ber(int *fd, char *path)
{
	char	*s;
	char	**ber;
	size_t	i;
	size_t	height;

	i = 0;
	height = get_ber_height(fd, path);
	if (*fd < 0)
		return (0);
	ber = malloc((height + 1) * sizeof(char *));
	if (!ber)
		return (0);
	ft_bzero(ber, (height + 1) * sizeof(char *));
	s = get_next_line(*fd);
	while (s)
	{
		ber[i] = ft_strdup_without_whitespaces(s);
		if (ber[i])
			i++;
		free(s);
		s = get_next_line(*fd);
	}
	ber[i] = 0;
	return (ber);
}

static int	get_ber_height(int *fd, char *path)
{
	char	*s;
	int		height;

	height = 0;
	s = get_next_line(*fd);
	while (s)
	{
		if (ft_strlen_without_whitespaces(s))
			++height;
		free(s);
		s = get_next_line(*fd);
	}
	close(*fd);
	*fd = open(path, O_RDONLY);
	return (height);
}

static size_t	ft_strlen_without_whitespaces(char *s)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = 0;
	while (s[i])
	{
		if (!ft_isspace(s[i]))
			++len;
		++i;
	}
	return (len);
}

static char	*ft_strdup_without_whitespaces(char *s)
{
	char	*dup;
	size_t	i;
	size_t	len;

	len = ft_strlen_without_whitespaces(s);
	if (!len)
		return (0);
	dup = malloc((len + 1) * sizeof(char));
	if (!dup)
		return (0);
	i = 0;
	while (*s)
	{
		if (!ft_isspace(*s))
			dup[i++] = *s;
		++s;
	}
	dup[i] = 0;
	return (dup);
}

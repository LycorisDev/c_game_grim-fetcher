#include "grim_fetcher.h"

static int    get_map_height(int *fd, char *path);
static size_t strlen_without_whitespaces(char *s);
static char   *strdup_without_whitespaces(char *s);

int get_map_fd(char *path)
{
	int    fd;
	size_t len;

	if (path)
		fd = open(path, O_RDONLY);
	len = path ? strlen(path) : 0;
	if (!path)
		fprintf(stderr, "Error: No argument specified -> *.map file needed\n");
	else if (fd < 0)
		perror("Error");
	else if (len < 4 || strcmp(&path[len - 4], ".map"))
		fprintf(stderr, "Error: File doesn't have *.map extension\n");
	else
		return fd;
	if (path)
		close(fd);
	return -1;
}

char **copy_map(int *fd, char *path)
{
	char   *s;
	char   **map;
	size_t i;
	size_t height;

	i = 0;
	height = get_map_height(fd, path);
	if (*fd < 0)
		return 0;
	map = malloc((height + 1) * sizeof(char *));
	if (!map)
		return 0;
	memset(map, 0, (height + 1) * sizeof(char *));
	s = gnl(*fd);
	while (s)
	{
		map[i] = strdup_without_whitespaces(s);
		if (map[i])
			i++;
		free(s);
		s = gnl(*fd);
	}
	map[i] = 0;
	return map;
}

static int get_map_height(int *fd, char *path)
{
	char *s;
	int  height;

	height = 0;
	s = gnl(*fd);
	while (s)
	{
		if (strlen_without_whitespaces(s))
			++height;
		free(s);
		s = gnl(*fd);
	}
	close(*fd);
	*fd = open(path, O_RDONLY);
	return height;
}

static size_t strlen_without_whitespaces(char *s)
{
	size_t i;
	size_t len;

	i = 0;
	len = 0;
	while (s[i])
	{
		if (!isspace(s[i]))
			++len;
		++i;
	}
	return len;
}

static char *strdup_without_whitespaces(char *s)
{
	char   *dup;
	size_t i;
	size_t len;

	len = strlen_without_whitespaces(s);
	if (!len)
		return 0;
	dup = malloc((len + 1) * sizeof(char));
	if (!dup)
		return 0;
	i = 0;
	while (*s)
	{
		if (!isspace(*s))
			dup[i++] = *s;
		++s;
	}
	dup[i] = 0;
	return dup;
}

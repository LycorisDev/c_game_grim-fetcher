#include "grim_fetcher.h"

char		*findc(char *s, char c);
void		allocate_segment_cycle(t_xpm_seg *seg);

static int	cycle(char *line, t_xpm_seg *seg);
static void	field(t_xpm *file, char *line, int seg, int *in_cycle);
static int	set_info_vars(char *line, char **field, char **data, char **dup);
static int	segment(t_xpm_seg *seg, char *field, char *data, char *dup);

int	set_xpm_file_obj(t_xpm *file, int fd)
{
	int		seg;
	int		in_cycle;
	char	*line;

	seg = 0;
	in_cycle = 0;
	line = get_next_line(fd);
	while (line)
	{
		if (in_cycle)
			in_cycle = cycle(line, &file->seg[seg]);
		else if (findc(line, '}'))
			++seg;
		else if (*findc(line, -1) == ']')
			break ;
		field(file, line, seg, &in_cycle);
		free(line);
		line = get_next_line(fd);
	}
	free(line);
	line = get_next_line(fd);
	free(line);
	return (line != 0);
}

static int	cycle(char *line, t_xpm_seg *seg)
{
	static int	index;
	char		*data;

	data = findc(line, '[');
	if (!data || index >= seg->cycle_len)
	{
		index = 0;
		return (0);
	}
	++data;
	set_ivec2(&seg->cycle[index], ft_atoi(data), ft_atoi(findc(data, ',') + 1));
	++index;
	return (1);
}

static void	field(t_xpm *file, char *line, int seg, int *in_cycle)
{
	char	*field;
	char	*data;
	char	*dup;

	if (!set_info_vars(line, &field, &data, &dup))
		return ;
	else if (!ft_strncmp(field, "path_shadow", 11))
		file->path_shadow = dup;
	else if (!ft_strncmp(field, "path", 4))
		file->path = dup;
	else if (!ft_strncmp(field, "segment_len", 11))
	{
		file->segment_len = ft_atoi(data);
		file->seg = malloc(file->segment_len * sizeof(t_xpm_seg));
	}
	else if (file->seg)
	{
		if (!segment(&file->seg[seg], field, data, dup)
			&& !ft_strncmp(field, "cycle", 5))
		{
			*in_cycle = 1;
			allocate_segment_cycle(&file->seg[seg]);
		}
	}
	return ;
}

static int	set_info_vars(char *line, char **field, char **data, char **dup)
{
	char	*bracket;

	*field = 0;
	*data = 0;
	*dup = 0;
	*field = findc(line, '"');
	if (!*field)
		return (0);
	*data = findc(findc(++*field, ':') + 1, -1);
	bracket = findc(*data, '[');
	if (bracket)
		*data = bracket + 1;
	if ((!ft_strncmp(*field, "id", 2) || !ft_strncmp(*field, "path", 4))
		&& *(*data + 1) != '"')
	{
		*dup = ft_strdup(*data + 1);
		if (*dup)
			(*dup)[ft_strlen(*dup) - 3] = 0;
	}
	return (1);
}

static int	segment(t_xpm_seg *seg, char *field, char *data, char *dup)
{
	if (!ft_strncmp(field, "id", 2))
		seg->id = dup;
	else if (!ft_strncmp(field, "size", 4))
		set_ivec2(&seg->size, ft_atoi(data), ft_atoi(findc(data, ',') + 1));
	else if (!ft_strncmp(field, "shadow_offset", 13))
		set_ivec2(&seg->shadow_offset, ft_atoi(data),
			ft_atoi(findc(data, ',') + 1));
	else if (!ft_strncmp(field, "still_frame", 11))
		seg->still_frame = ft_atoi(data);
	else if (!ft_strncmp(field, "cycle_time_in_ms", 16))
		seg->cycle_time_in_ms = ft_atoi(data);
	else if (!ft_strncmp(field, "cycle_len", 9))
		seg->cycle_len = ft_atoi(data);
	else
		return (0);
	return (1);
}

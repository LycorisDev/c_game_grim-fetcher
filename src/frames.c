#include "grim_fetcher.h"

static t_frame *create_frame(void);
static void    free_frame(t_frame **f);

int create_frames(void)
{
	int i;
	int success;

	success = 1;
	man.curr_frame = 0;
	i = 0;
	while (i < NBR_FRAMES)
	{
		man.frame[i] = create_frame();
		if (!man.frame[i])
			success = 0;
		++i;
	}
	man.frame[i] = 0;
	if (!success)
		free_frames();
	return success;
}

void use_frame(t_frame *f)
{
	glBindTexture(GL_TEXTURE_2D, f ? f->id : 0);
	return;
}

void clear_drawing(t_frame *f, GLubyte alpha)
{
	size_t i;
	size_t len;

	len = f->real_size.x * f->real_size.y;
	memset(f->buf, 0, len * 4 * sizeof(GLubyte));
	if (!alpha)
		return;
	i = 0;
	while (i < len)
	{
		f->buf[i * 4 + 3] = alpha;
		++i;
	}
	return;
}

void save_drawing(t_frame *f)
{
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, f->real_size.x, f->real_size.y, 0, 
			GL_RGBA, GL_UNSIGNED_BYTE, f->buf);
	return;
}

void free_frames(void)
{
	unsigned int i;

	use_frame(0);
	i = 0;
	while (i < NBR_FRAMES && man.frame[i])
	{
		free_frame(&man.frame[i]);
		++i;
	}
	return;
}

static t_frame *create_frame(void)
{
	t_frame *f;
	long    buf_length;

	f = malloc(sizeof(t_frame));
	if (!f)
	{
		fprintf(stderr, "Error: Couldn't allocate memory for frame buffer");
		return 0;
	}
	f->id = -1;
	f->real_size.x = man.res.monitor_size.x;
	f->real_size.y = man.res.monitor_size.y;
	f->thickness = f->real_size.x / man.res.window_size_default.x;
	f->size.x = f->real_size.x / f->thickness;
	f->size.y = f->real_size.y / f->thickness;
	buf_length = f->real_size.x * f->real_size.y * 4 * sizeof(GLubyte);
	f->buf = malloc(buf_length);
	if (!f->buf)
	{
		fprintf(stderr, "Error: Couldn't allocate enough memory for a frame "
				"buffer (= %ld bytes for a %dx%d resolution)\n", 
				buf_length, f->real_size.x, f->real_size.y);
		free(f);
		return 0;
	}
	glGenTextures(1, &f->id);
	glBindTexture(GL_TEXTURE_2D, f->id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
	return f;
}

static void free_frame(t_frame **f)
{
	if (!f)
		return;
	glDeleteTextures(1, &(*f)->id);
	free((*f)->buf);
	free(*f);
	*f = 0;
	return;
}

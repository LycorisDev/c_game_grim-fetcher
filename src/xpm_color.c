#include "grim_fetcher.h"

static int	get_color_8888(t_img *img, size_t i, int is_shadow);
static int	get_color_0888(t_img *img, size_t i, int is_shadow);
static int	get_color_0565(t_img *img, size_t i, int is_shadow);
static int	get_color_0332(t_img *img, size_t i, int is_shadow);

int	get_color_bpp(t_img *img, size_t i, int is_shadow)
{
	if (img->bpp == 32)
		return (get_color_8888(img, i, is_shadow));
	else if (img->bpp == 24)
		return (get_color_0888(img, i, is_shadow));
	else if (img->bpp == 16)
		return (get_color_0565(img, i, is_shadow));
	else if (img->bpp == 8)
		return (get_color_0332(img, i, is_shadow));
	return (0);
}

static int	get_color_8888(t_img *img, size_t i, int is_shadow)
{
	t_ubyte	tmp;
	t_ubyte	alpha;
	t_ubyte	red;
	t_ubyte	green;
	t_ubyte	blue;

	if (!img->image->byte_order)
	{
		tmp = img->data[i * 4 + 0];
		img->data[i * 4 + 0] = img->data[i * 4 + 3];
		img->data[i * 4 + 3] = tmp;
		tmp = img->data[i * 4 + 1];
		img->data[i * 4 + 1] = img->data[i * 4 + 2];
		img->data[i * 4 + 2] = tmp;
	}
	alpha = 255 - img->data[i * 4 + 0];
	red = img->data[i * 4 + 1];
	green = img->data[i * 4 + 2];
	blue = img->data[i * 4 + 3];
	if (is_shadow)
		alpha = 255 - red;
	return (get_color(alpha, red, green, blue));
}

static int	get_color_0888(t_img *img, size_t i, int is_shadow)
{
	t_ubyte	tmp;
	t_ubyte	alpha;
	t_ubyte	red;
	t_ubyte	green;
	t_ubyte	blue;

	if (!img->image->byte_order)
	{
		tmp = img->data[i * 3 + 0];
		img->data[i * 3 + 0] = img->data[i * 3 + 2];
		img->data[i * 3 + 2] = tmp;
	}
	alpha = 255;
	red = img->data[i * 3 + 0];
	green = img->data[i * 3 + 1];
	blue = img->data[i * 3 + 2];
	if (is_shadow)
		alpha = 255 - red;
	return (get_color(alpha, red, green, blue));
}

static int	get_color_0565(t_img *img, size_t i, int is_shadow)
{
	t_ubyte	tmp;
	t_ubyte	alpha;
	t_ubyte	red;
	t_ubyte	green;
	t_ubyte	blue;

	if (!img->image->byte_order)
	{
		tmp = img->data[i * 2 + 0];
		img->data[i * 2 + 0] = img->data[i * 2 + 1];
		img->data[i * 2 + 1] = tmp;
	}
	alpha = 255;
	red = (img->data[i * 2 + 0] >> 3) & 0x1F;
	green = ((img->data[i * 2 + 0] & 0x07) << 3) | ((img->data[i * 2 + 1] >> 5)
			& 0x07);
	blue = img->data[i * 2 + 1] & 0x1F;
	if (is_shadow)
		alpha = 255 - red;
	return (get_color(alpha, red, green, blue));
}

static int	get_color_0332(t_img *img, size_t i, int is_shadow)
{
	t_ubyte	alpha;
	t_ubyte	red;
	t_ubyte	green;
	t_ubyte	blue;

	alpha = 255;
	red = (img->data[i] >> 5) & 0x07;
	green = (img->data[i] >> 2) & 0x07;
	blue = img->data[i] & 0x03;
	if (is_shadow)
		alpha = 255 - red;
	return (get_color(alpha, red, green, blue));
}

#include "grim_fetcher.h"

t_ubyte	get_alpha(t_uint color)
{
	return ((color >> 24) & 0xFF);
}

void	set_alpha(t_uint *color, t_ubyte value)
{
	*color = (*color & 0x00FFFFFF) | (value << 24);
	return ;
}

t_ubyte	get_red(t_uint color)
{
	return ((color >> 16) & 0xFF);
}

void	set_red(t_uint *color, t_ubyte value)
{
	*color = (*color & 0xFF00FFFF) | (value << 16);
	return ;
}

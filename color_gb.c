#include "grim_fetcher.h"

t_ubyte	get_green(t_uint color)
{
	return ((color >> 8) & 0xFF);
}

void	set_green(t_uint *color, t_ubyte value)
{
	*color = (*color & 0xFFFF00FF) | (value << 8);
	return ;
}

t_ubyte	get_blue(t_uint color)
{
	return (color & 0xFF);
}

void	set_blue(t_uint *color, t_ubyte value)
{
	*color = (*color & 0xFFFFFF00) | value;
	return ;
}

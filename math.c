#include "grim_fetcher.h"

double	ft_abs(double number)
{
	if (number < 0)
		return (-number);
	return (number);
}

double	ft_clamp(double number, double min, double max)
{
	if (number < min)
		return (min);
	else if (number > max)
		return (max);
	return (number);
}

double	ft_pow(double base, double exp)
{
	double	res;
	int		i;

	res = 1.0;
	i = 0;
	while (i < exp)
	{
		res *= base;
		++i;
	}
	return (res);
}

double	ft_sqrt(double number)
{
	float	x;
	float	xhalf;
	int		i;

	x = number;
	xhalf = 0.5 * x;
	i = *(int *)&x;
	i = 0x5f375a86 - (i >> 1);
	x = *(float *)&i;
	x = x * (1.5 - xhalf * x * x);
	x = x * (1.5 - xhalf * x * x);
	x = x * (1.5 - xhalf * x * x);
	return (x * number);
}

// Simple 32-bit Xorshift pseudo-RNG
int	rng_minmax(int *seed, int min, int max)
{
	unsigned int	random;

	if (!*seed)
		*seed = 1;
	random = *seed;
	random ^= random << 13;
	random ^= random >> 17;
	random ^= random << 5;
	*seed = random;
	return (random % (max - min + 1) + min);
}

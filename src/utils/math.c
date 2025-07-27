#include "grim_fetcher.h"

double f_clamp(double number, double min, double max)
{
	return number < min ? min : number > max ? max : number;
}

int i_clamp(int number, int min, int max)
{
	return number < min ? min : number > max ? max : number;
}

int i_min(int a, int b)
{
	return a < b ? a : b;
}

double f_sqrt(double number)
{
	double	x;
	double	xhalf;
	int64_t	i;

	x = number;
	xhalf = 0.5 * x;
	memcpy(&i, &x, sizeof(int64_t));
	i = 0x5fe6ec85e7de30da - (i >> 1);
	memcpy(&x, &i, sizeof(double));
	x = x * (1.5 - xhalf * x * x);
	x = x * (1.5 - xhalf * x * x);
	x = x * (1.5 - xhalf * x * x);
	return x * number;
}

/* Euclidean distance (all directions) */
double f_dist_euclidean(double ax, double ay, double bx, double by)
{
	return f_sqrt(pow(ax - bx, 2) + pow(ay - by, 2));
}

/* Manhattan distance (no diagonal) */
double f_dist_manhattan(double ax, double ay, double bx, double by)
{
	return fabs(ax - bx) + fabs(ay - by);
}

/* Simple 32-bit Xorshift pseudo-RNG */
int rng_minmax(int *seed, int min, int max)
{
	unsigned int random;

	if (!*seed)
		*seed = 1;
	random = *seed;
	random ^= random << 13;
	random ^= random >> 17;
	random ^= random << 5;
	*seed = random;
	return random % (max - min + 1) + min;
}

void set_vec2(t_vec2 *vector, double x, double y)
{
	vector->x = x;
	vector->y = y;
	return;
}

void set_ivec2(t_ivec2 *vector, int x, int y)
{
	vector->x = x;
	vector->y = y;
	return;
}

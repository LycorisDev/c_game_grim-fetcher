#include "grim_fetcher.h"

double f_abs(double number)
{
    return number < 0 ? -number : number;
}

double f_clamp(double number, double min, double max)
{
    return number < min ? min : number > max ? max : number;
}

double f_pow(double base, double exp)
{
    double res;
    int    i;

    res = 1.0;
    i = 0;
    while (i < exp)
    {
        res *= base;
        ++i;
    }
    return res;
}

double f_sqrt(double number)
{
    float x;
    float xhalf;
    int   i;

    x = number;
    xhalf = 0.5 * x;
    i = *(int *)&x;
    i = 0x5f375a86 - (i >> 1);
    x = *(float *)&i;
    x = x * (1.5 - xhalf * x * x);
    x = x * (1.5 - xhalf * x * x);
    x = x * (1.5 - xhalf * x * x);
    return x * number;
}

// Euclidean distance (all directions)
double f_dist_euclidean(double ax, double ay, double bx, double by)
{
    return f_sqrt(f_pow(ax - bx, 2) + f_pow(ay - by, 2));
}

// Manhattan distance (no diagonal)
double f_dist_manhattan(double ax, double ay, double bx, double by)
{
    return f_abs(ax - bx) + f_abs(ay - by);
}

int i_abs(int number)
{
    return number < 0 ? -number : number;
}

int i_max(int a, int b)
{
    return a > b ? a : b;
}

// Simple 32-bit Xorshift pseudo-RNG
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

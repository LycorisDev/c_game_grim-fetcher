#include "grim_fetcher.h"

// Euclidean distance (all directions)
double	get_dist_euclidean(double ax, double ay, double bx, double by)
{
	return (ft_sqrt(ft_pow(ax - bx, 2) + ft_pow(ay - by, 2)));
}

// Manhattan distance (no diagonal)
double	get_dist_manhattan(double ax, double ay, double bx, double by)
{
	return (ft_abs(ax - bx) + ft_abs(ay - by));
}

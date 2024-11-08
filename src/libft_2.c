#include "grim_fetcher.h"

static int	get_num_len(long n);

int	ft_strcmp(char *s1, char *s2)
{
	size_t	i;

	if (!s1 || !s2)
		return (-1);
	i = 0;
	while (s1[i] && s1[i] == s2[i])
		++i;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	ft_strncmp(char *s1, char *s2, size_t n)
{
	size_t	i;

	if (!n)
		return (0);
	else if (!s1 || !s2)
		return (-1);
	i = 0;
	while (i < n - 1 && s1[i] && s1[i] == s2[i])
		++i;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

char	*ft_itoa(int number)
{
	long	n;
	int		num_len;
	char	*str;

	n = number;
	num_len = get_num_len(n);
	str = malloc((num_len + 1) * sizeof(char));
	if (!str)
		return (0);
	str[num_len--] = 0;
	str[num_len] = '0';
	if (n < 0)
	{
		str[0] = '-';
		str[num_len--] = '0' - n % 10;
		n /= 10;
		n = -n;
	}
	while (n > 0)
	{
		str[num_len--] = '0' + n % 10;
		n /= 10;
	}
	return (str);
}

static int	get_num_len(long n)
{
	int	num_len;

	num_len = 0;
	if (!n)
		return (1);
	else if (n < 0)
	{
		if (n == -2147483648)
			return (11);
		++num_len;
		n = -n;
	}
	while (n > 0)
	{
		++num_len;
		n /= 10;
	}
	return (num_len);
}

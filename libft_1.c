#include "grim_fetcher.h"

/* Modified */
char	*ft_strchr(char *s, int c, int reverse)
{
	size_t	i;

	if (!s)
		return (0);
	else if (reverse)
	{
		i = ft_strlen(s);
		while (i > 0)
		{
			if (s[i] == c)
				return ((char *)s + i);
			--i;
		}
		if (s[i] == c)
			return ((char *)s + i);
		return (0);
	}
	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			return ((char *)s + i);
		++i;
	}
	return (0);
}

size_t	ft_strlcat(char *dest, char *src, size_t size)
{
	size_t	i;
	size_t	j;
	size_t	src_len;
	size_t	original_dest_len;

	src_len = ft_strlen(src);
	original_dest_len = ft_strlen(dest);
	if (size)
	{
		i = original_dest_len;
		j = 0;
		while (i < size - 1 && src[j])
			dest[i++] = src[j++];
		dest[i] = 0;
	}
	if (size < original_dest_len)
		return (size + src_len);
	return (original_dest_len + src_len);
}

char	*ft_strdup(char *s)
{
	char	*dup;
	size_t	len;
	size_t	i;

	len = ft_strlen(s);
	if (!len)
		return (0);
	dup = malloc((len + 1) * sizeof(char));
	if (!dup)
		return (0);
	i = 0;
	while (i < len)
	{
		dup[i] = s[i];
		++i;
	}
	dup[i] = 0;
	return (dup);
}

void	ft_bzero(void *s, size_t n)
{
	size_t	i;
	char	*dest;

	if (!s)
		return ;
	i = 0;
	dest = s;
	while (i < n)
	{
		dest[i] = 0;
		++i;
	}
	return ;
}

int	ft_atoi(char *n)
{
	int		i;
	int		sign;
	long	integer;

	i = 0;
	integer = 0;
	sign = 1;
	while (ft_isspace(n[i]))
		++i;
	if (n[i] == '+' || n[i] == '-')
	{
		if (n[i] == '-')
			sign = -sign;
		++i;
	}
	while (n[i] >= '0' && n[i] <= '9')
	{
		integer = integer * 10 + (n[i] - '0');
		++i;
	}
	return (integer * sign);
}

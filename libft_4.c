#include "grim_fetcher.h"

static void	print_char(int fd, unsigned char c);
static void	print_string(int fd, char *s);
static void	print_int(int fd, long n);

void	ft_dprintf(int fd, char *format, ...)
{
	va_list	args;

	if (fd < 0 || !format)
		return ;
	va_start(args, format);
	while (*format)
	{
		if (*format != '%')
			print_char(fd, *format);
		else
		{
			++format;
			if (*format == '%')
				print_char(fd, *format);
			else if (*format == 'c')
				print_char(fd, (unsigned char)va_arg(args, int));
			else if (*format == 's')
				print_string(fd, va_arg(args, char *));
			else if (*format == 'd')
				print_int(fd, va_arg(args, int));
		}
		++format;
	}
	va_end(args);
	return ;
}

static void	print_char(int fd, unsigned char c)
{
	if (c)
		write(fd, &c, 1);
	return ;
}

static void	print_string(int fd, char *s)
{
	write(fd, s, ft_strlen(s));
	return ;
}

static void	print_int(int fd, long n)
{
	char	*s;
	int		i;

	s = malloc(12 * sizeof(char));
	if (!s)
		return ;
	ft_bzero(s, 12 * sizeof(char));
	if (!n)
		s[0] = '0';
	if (n < 0)
	{
		s[0] = '-';
		n = -n;
	}
	i = 11;
	while (n > 0)
	{
		s[i--] = '0' + n % 10;
		n /= 10;
	}
	i = 0;
	while (i < 12)
		print_char(fd, s[i++]);
	free(s);
	return ;
}

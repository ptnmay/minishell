/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csantivi <csantivi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/26 15:40:12 by csantivi          #+#    #+#             */
/*   Updated: 2023/06/14 20:04:32 by csantivi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count(long int n)
{
	int	i;

	i = 0;
	if (n <= 0)
		i++;
	while (n != 0)
	{
		n = n / 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(long int nbr)
{
	int		c;
	int		neg;
	char	*str;

	c = count(nbr);
	neg = 1;
	if (nbr != 0)
		str = (char *)malloc(sizeof(char) * (c + 1));
	else
		return (ft_strdup("0"));
	str[c--] = '\0';
	if (nbr < 0)
	{
		neg *= -1;
		str[0] = '-';
	}
	while (c >= 0)
	{
		str[c--] = '0' + (neg * (nbr % 10));
		nbr /= 10;
		if (neg < 0 && c <= 1)
			break ;
	}
	return (str);
}

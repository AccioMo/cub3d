/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzeggaf <mzeggaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 15:47:55 by mzeggaf           #+#    #+#             */
/*   Updated: 2024/12/02 19:04:28 by mzeggaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	ft_free(void **ptr)
{
	void	**hold;

	hold = ptr;
	if (!ptr)
		return ;
	while (*ptr)
		free(*ptr++);
	free(hold);
}

int	ft_end(void *param)
{
	t_env	*env;

	env = (t_env *)param;
	mlx_clear_window(env->mlx, env->win);
	mlx_destroy_image(env->mlx, env->img.img);
	mlx_destroy_window(env->mlx, env->win);
	exit(0);
	return (1);
}

void	ft_exit(int code, char *msg)
{
	ft_putstr_fd("Error\n", 2);
	if (code == 1)
		ft_putstr_fd("Error opening file: ", 2);
	else if (code == 2)
		ft_putstr_fd("Invalid file extension: ", 2);
	else if (code == 3)
		ft_putstr_fd("Error allocating memory: ", 2);
	else if (code == 4)
		ft_putstr_fd("Error reading file: ", 2);
	else if (code == 5)
		ft_putstr_fd("mlx: ", 2);
	ft_putstr_fd(msg, 2);
	exit(EXIT_FAILURE);
}

void	*ft_realloc(void *ptr, size_t size)
{
	void	*new;

	new = malloc(size);
	if (new == NULL)
		ft_exit(0, "insufficient memory\n");
	ft_memmove(new, ptr, ft_strlen(ptr));
	((char *) new)[ft_strlen(ptr)] = '\0';
	return (new);
}

int	free_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbatteux <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 16:03:09 by tbatteux          #+#    #+#             */
/*   Updated: 2023/07/13 11:04:45 by tbatteux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_H
# define TEST_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

int	ft_strlen(char *str);
int	get_taille(char **env);
int	ft_strncmp(const char *s1, const char *s2, size_t len);
char	*ft_strdup(const char *s);
char	**get_env(char **new_env, char **env);

#endif

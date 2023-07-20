#include "minishell.h"

char    *ms_getenv(char *envname)
{
    size_t  en_len;
    size_t  i;

    en_len = ft_strlen(envname);
    envname[en_len] = '=';

    i = 0;
    while (g_minishell.envs[i])
    {
        if (!ft_strncmp(envname, g_minishell.envs[i], en_len))
        {
            envname[en_len] = '\0';
            return (g_minishell.envs[i] + en_len + 1);
        }
        ++i;
    }
    envname[en_len] = '\0';
    return (NULL);
}
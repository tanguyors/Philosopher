#include "philo.h"

int	start_simulation(t_data *data)
{
    pthread_t	*threads;
    int		i;

    threads = malloc(sizeof(pthread_t) * data->num_philos);
    if (!threads)
        return (1);
    i = 0;
    while (i < data->num_philos)
    {
        if (pthread_create(&threads[i], NULL, &routine, &data->philos[i]) != 0)
            return (1);
        i++;
    }
    while (!data->finished)
    {
        i = 0;
        while (i < data->num_philos)
        {
            if (checkdeath(&data->philos[i]))
            {
                pthread_mutex_lock(&data->write_mutex);
                ft_printf("%d philo %d died\n", (int)(get_time() - data->start_time), data->philos[i].id);
                data->finished = 1;
                pthread_mutex_unlock(&data->write_mutex);
                return (0);
            }
            i++;
            usleep(100);
        }
    }
    return (0);
}

void	clean_simulation(t_data *data)
{
    int	i;

    i = 0;
    while (i < data->num_philos)
    {
        pthread_mutex_destroy(&data->forks[i]);
        i++;
    }
    pthread_mutex_destroy(&data->write_mutex);
    free(data->forks);
    free(data->philos);
}

#include "philo.h"

int main(int argc, char **argv)
{
    t_data data;
    pthread_t *threads;
    int i;

    if (argc != 5 && argc != 6)
    {
        ft_putstr_fd("Usage: ./philo number_of_philosophers time_to_die ", 2);
        ft_putstr_fd("time_to_eat time_to_sleep ", 2);
        ft_putendl_fd("[number_of_times_each_philosopher_must_eat]", 2);
        return (1);
    }
    if (init_data(&data, argc, argv))
        return (1);

    // Allocation des threads
    threads = malloc(sizeof(pthread_t) * data.num_philos);
    if (!threads)
    {
        ft_printf("Error: malloc threads failed\n");
        return (1);
    }

    // Création des threads
    i = 0;
    while (i < data.num_philos)
    {
        if (pthread_create(&threads[i], NULL, &routine, &data.philos[i]) != 0)
        {
            ft_printf("Error: pthread_create failed\n");
            return (1);
        }
        i++;
    }

    // Attente de la fin des threads
    i = 0;
    while (i < data.num_philos)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            ft_printf("Error: pthread_join failed\n");
            return (1);
        }
        i++;
    }

    // Nettoyage
    free(threads);
    free(data.forks);
    free(data.philos);
    pthread_mutex_destroy(&data.write_mutex);
    i = 0;
    while (i < data.num_philos)
    {
        pthread_mutex_destroy(&data.forks[i]);
        i++;
    }

    return (0);
}


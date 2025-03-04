#include "philo.h"

void *routine(void *arg)
{
    t_philo *philo;

    philo = (t_philo *)arg;
    
    // Pour l'instant, on affiche juste que le philosophe a commencé
    pthread_mutex_lock(&philo->data->write_mutex);
    ft_printf("%lld %d has started\n", get_time() - philo->data->start_time, philo->id);
    pthread_mutex_unlock(&philo->data->write_mutex);

    // On attend un peu pour simuler une activité
    usleep(1000000); // Attend 1 seconde

    // On affiche que le philosophe a terminé
    pthread_mutex_lock(&philo->data->write_mutex);
    ft_printf("%lld %d has finished\n", get_time() - philo->data->start_time, philo->id);
    pthread_mutex_unlock(&philo->data->write_mutex);

    return (NULL);
} 
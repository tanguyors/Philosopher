#include "philo.h"

void *routine(void *arg)
{
    t_philo *philo;

    philo = (t_philo *)arg;

    // Première vérification avant de commencer
    if (!philo->data->finished)
    {
        pthread_mutex_lock(&philo->data->write_mutex);
        ft_printf("%d  philo %d has started\n", (int)(get_time() - philo->data->start_time), philo->id);
        pthread_mutex_unlock(&philo->data->write_mutex);
    }

    if(!philo->data->finished)
        takeforks(philo);

    // Vérification avant d'attendre
    if (!philo->data->finished)
        usleep(1000000);

    // Vérification avant de finir
    if (!philo->data->finished)
    {
        pthread_mutex_lock(&philo->data->write_mutex);
        ft_printf("%d philo %d has finished\n", (int)(get_time() - philo->data->start_time), philo->id);
        pthread_mutex_unlock(&philo->data->write_mutex);
    }
    return (NULL);
}

int checkdeath(t_philo *philo)
{
    long long actual_time;
    long long time_no_eat;

    actual_time = get_time();
    time_no_eat = actual_time - philo->last_meal;

    if(time_no_eat > philo->data->time_to_die)
        return(1);
    return(0);
}

void    print_fork_message(t_philo *philo)
{
    pthread_mutex_lock(&philo->data->write_mutex);
    ft_printf("%d philo %d has taken a fork\n", (int)(get_time() - philo->data->start_time), philo->id);
    pthread_mutex_unlock(&philo->data->write_mutex);
}

void    takeforks(t_philo *philo)
{
    if (philo->data->finished)
        return;

    if (philo->id % 2 == 0)
    {
        pthread_mutex_lock(&philo->data->forks[philo->right_fork]);
        print_fork_message(philo);
        if(philo->data->finished)
        {
            pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
            return;
        }
        pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
        print_fork_message(philo);
    }
    else
    {
        pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
        print_fork_message(philo);
        if(philo->data->finished)
        {
            pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
            return;
        }
        pthread_mutex_lock(&philo->data->forks[philo->right_fork]);
        print_fork_message(philo);
    }

    if(philo->data->finished)
    {
        pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
        pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
    }
}
#include "philo.h"

void *routine(void *arg)
{
    t_philo *philo;

    philo = (t_philo *)arg;

    // Message de démarrage
    if (!philo->data->finished)
    {
        pthread_mutex_lock(&philo->data->write_mutex);
        ft_printf("%d  philo %d has started\n", (int)(get_time() - philo->data->start_time), philo->id);
        pthread_mutex_unlock(&philo->data->write_mutex);
    }

    // Petit délai pour les philosophes pairs
    if (philo->id % 2 == 0)
        usleep(100);

    // Boucle principale
    while (!philo->data->finished)
    {
        takeforks(philo);
        if (philo->can_eat)
            eat(philo);
        usleep(100); // Petit délai entre les tentatives
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
        philo->can_eat = 1;
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
        philo->can_eat = 1;
    }

    if(philo->data->finished)
    {
        pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
        pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
    }
}

void    release_forks(t_philo *philo)
{
    pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
    pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
    philo->can_eat = 0;  // Le philosophe ne peut plus manger
}

void eat(t_philo *philo)
{
    if(!philo->data->finished)
    {
        if(philo->can_eat)
        {
            pthread_mutex_lock(&philo->data->write_mutex);
            ft_printf("%d philo %d is eating ...\n", (int)(get_time() - philo->data->start_time), philo->id);
            pthread_mutex_unlock(&philo->data->write_mutex);
            philo->last_meal = get_time();
            usleep(philo->data->time_to_eat * 1000);
            release_forks(philo);
        }
    }
}
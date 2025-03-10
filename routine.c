#include "philo.h"

void *routine(void *arg)
{
    t_philo *philo;

    philo = (t_philo *)arg;

    pthread_mutex_lock(&philo->data->finished_mutex);
    if (philo->data->finished)
    {
        pthread_mutex_unlock(&philo->data->finished_mutex);
        return (NULL);
    }
    pthread_mutex_unlock(&philo->data->finished_mutex);

    pthread_mutex_lock(&philo->data->finished_mutex);
    if (!philo->data->finished)
    {
        pthread_mutex_unlock(&philo->data->finished_mutex);
        pthread_mutex_lock(&philo->data->write_mutex);
        ft_printf("%d  philo %d has started\n", (int)(get_time() - philo->data->start_time), philo->id);
        pthread_mutex_unlock(&philo->data->write_mutex);
    }
    else
        pthread_mutex_unlock(&philo->data->finished_mutex);

    if (philo->id % 2 == 0)
        usleep(100);

    pthread_mutex_lock(&philo->data->finished_mutex);
    while (!philo->data->finished)
    {
        pthread_mutex_unlock(&philo->data->finished_mutex);
        takeforks(philo);
        if (philo->can_eat)
            eat(philo);
        usleep(100);
        pthread_mutex_lock(&philo->data->finished_mutex);
    }
    pthread_mutex_unlock(&philo->data->finished_mutex);

    release_forks(philo);
    return (NULL);
}

int checkdeath(t_philo *philo)
{
    long long actual_time;
    long long time_no_eat;
    long long last_meal_time;

    actual_time = get_time();
    pthread_mutex_lock(&philo->data->meal_mutex);
    last_meal_time = philo->last_meal;
    pthread_mutex_unlock(&philo->data->meal_mutex);
    
    time_no_eat = actual_time - last_meal_time;
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
    // 1. Vérification rapide de finished
    pthread_mutex_lock(&philo->data->finished_mutex);
    if (philo->data->finished)
    {
        pthread_mutex_unlock(&philo->data->finished_mutex);
        return;
    }
    pthread_mutex_unlock(&philo->data->finished_mutex);

    // 2. Prise des fourchettes dans un ordre cohérent
    if (philo->id % 2 == 0)
    {
        pthread_mutex_lock(&philo->data->forks[philo->right_fork]);
        print_fork_message(philo);
        pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
        print_fork_message(philo);
    }
    else
    {
        pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
        print_fork_message(philo);
        pthread_mutex_lock(&philo->data->forks[philo->right_fork]);
        print_fork_message(philo);
    }

    // 3. Vérification finale de finished
    pthread_mutex_lock(&philo->data->finished_mutex);
    if (philo->data->finished)
    {
        pthread_mutex_unlock(&philo->data->finished_mutex);
        pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
        pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
        return;
    }
    philo->can_eat = 1;
    pthread_mutex_unlock(&philo->data->finished_mutex);
}

void    release_forks(t_philo *philo)
{
    pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
    pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
    philo->can_eat = 0;  // Le philosophe ne peut plus manger
}

void eat(t_philo *philo)
{
    // 1. Vérification rapide de l'état
    pthread_mutex_lock(&philo->data->finished_mutex);
    if (philo->data->finished || !philo->can_eat)
    {
        pthread_mutex_unlock(&philo->data->finished_mutex);
        return;
    }
    pthread_mutex_unlock(&philo->data->finished_mutex);

    // 2. Mise à jour du dernier repas
    pthread_mutex_lock(&philo->data->meal_mutex);
    philo->last_meal = get_time();
    pthread_mutex_unlock(&philo->data->meal_mutex);

    // 3. Affichage du message
    pthread_mutex_lock(&philo->data->write_mutex);
    ft_printf("%d philo %d is eating ...\n", 
        (int)(get_time() - philo->data->start_time), philo->id);
    pthread_mutex_unlock(&philo->data->write_mutex);
    
    // 4. Attente et libération des fourchettes
    usleep(philo->data->time_to_eat * 1000);
    release_forks(philo);
}
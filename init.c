#include "philo.h" // qui contient la déclaration de t_data, t_philo, etc.

// Fonction qui renvoie le temps actuel en ms (nécessaire pour start_time, par ex.)
long long get_time(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int init_data(t_data *data)
{
    int i;

    // 1) Démarrer la simulation (ou on peut le faire ailleurs)
    //    On stocke le temps de départ, qui servira à calculer
    //    "combien de ms se sont écoulées" pour l'affichage
    data->start_time = get_time();

    // 2) data->finished = 0 pour dire que la simulation n'est pas terminée
    data->finished = 0;

    // 3) Allouer la mémoire pour le tableau de fourchettes
    data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
    if (!data->forks)
    {
        ft_printf("Error: malloc forks failed\n");
        return (1); // ou ta gestion d'erreur
    }

    // 4) Initialiser chaque mutex "fork"
    i = 0;
    while (i < data->num_philos)
    {
        // pthread_mutex_init renvoie 0 en cas de succès, sinon != 0
        if (pthread_mutex_init(&data->forks[i], NULL) != 0)
        {
            ft_printf("Error: pthread_mutex_init forks[%d] failed\n", i);
            return (1);
        }
        i++;
    }

    // 5) Initialiser le mutex d'écriture
    if (pthread_mutex_init(&data->write_mutex, NULL) != 0)
    {
        ft_printf("Error: pthread_mutex_init write_mutex failed\n");
        return (1);
    }

    // 6) Allouer le tableau de philosophes
    data->philos = malloc(sizeof(t_philo) * data->num_philos);
    if (!data->philos)
    {
        ft_printf("Error: malloc philos failed\n");
        return (1);
    }

    // 7) Initialiser chaque philosophe du tableau
    i = 0;
    while (i < data->num_philos)
    {
        data->philos[i].id = i;
        data->philos[i].meals_eaten = 0;
        data->philos[i].last_meal = 0; // on peut mettre 0, ou get_time() ultérieurement
        data->philos[i].data = data;
        
        // -> Indice de la fourchette gauche
        data->philos[i].left_fork = i;
        // -> Indice de la fourchette droite
        data->philos[i].right_fork = (i + 1) % data->num_philos;
        
        i++;
    }
    // 8) Si tout est bon, on renvoie 0 pour dire "Succès"
    return (0);
}


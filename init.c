#include "philo.h" // qui contient la déclaration de t_data, t_philo, etc.

// Fonction qui renvoie le temps actuel en ms (nécessaire pour start_time, par ex.)
long long get_time(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int init_data(t_data *data, int argc, char **argv)
{
    int i;

    // Parsing des arguments
    data->num_philos = ft_atoi(argv[1]);
    data->time_to_die = ft_atoi(argv[2]);
    data->time_to_eat = ft_atoi(argv[3]);
    data->time_to_sleep = ft_atoi(argv[4]);
    data->num_must_eat = -1;  // Valeur par défaut si non spécifié
    if (argc == 6)
        data->num_must_eat = ft_atoi(argv[5]);

    // Vérification des valeurs
    if (data->num_philos <= 0 || data->time_to_die <= 0 
        || data->time_to_eat <= 0 || data->time_to_sleep <= 0 
        || (argc == 6 && data->num_must_eat <= 0))
    {
        ft_printf("Error: Invalid arguments\n");
        return (1);
    }

    // Initialisation du temps de départ et de l'état
    data->start_time = get_time();
    data->finished = 0;

    // Allocation et initialisation des mutex pour les fourchettes
    data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
    if (!data->forks)
    {
        ft_printf("Error: malloc forks failed\n");
        return (1);
    }

    i = 0;
    while (i < data->num_philos)
    {
        if (pthread_mutex_init(&data->forks[i], NULL) != 0)
        {
            ft_printf("Error: pthread_mutex_init forks[%d] failed\n", i);
            return (1);
        }
        i++;
    }

    // Initialisation du mutex d'écriture
    if (pthread_mutex_init(&data->write_mutex, NULL) != 0)
    {
        ft_printf("Error: pthread_mutex_init write_mutex failed\n");
        return (1);
    }

    // Allocation et initialisation des philosophes
    data->philos = malloc(sizeof(t_philo) * data->num_philos);
    if (!data->philos)
    {
        ft_printf("Error: malloc philos failed\n");
        return (1);
    }

    i = 0;
    while (i < data->num_philos)
    {
        data->philos[i].id = i + 1;  // On commence à 1 pour l'affichage
        data->philos[i].meals_eaten = 0;
        data->philos[i].last_meal = data->start_time;  // Initialisation au temps de départ
        data->philos[i].data = data;
        data->philos[i].left_fork = i;
        data->philos[i].right_fork = (i + 1) % data->num_philos;
        i++;
    }

    return (0);
}


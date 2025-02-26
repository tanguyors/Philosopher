#include "philo.h"

#include "philo.h"

int main(int argc, char **argv)
{
    t_data data;

    if (argc != 5 && argc != 6)
    {
        ft_putstr_fd("Usage: ./philo number_of_philosophers time_to_die ", 2);
        ft_putstr_fd("time_to_eat time_to_sleep ", 2);
        ft_putendl_fd("[number_of_times_each_philosopher_must_eat]", 2);
        return (1);
    }
    if (init_data(&data, argc, argv))
        return (1);
    // ... reste du code
    return (0);
}


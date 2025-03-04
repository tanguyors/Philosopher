#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include "libft/libft.h"

typedef struct s_philo
{
    int id;
    int left_fork;
    int right_fork;
    long long last_meal;
    int meals_eaten;
    struct s_data *data;
    pthread_t thread;
} t_philo;

typedef struct s_data
{
    int num_philos;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int num_must_eat;
    long long start_time;
    int finished;
    pthread_mutex_t *forks;
    pthread_mutex_t write_mutex;
    t_philo *philos;
} t_data;

// Prototypes des fonctions
int init_data(t_data *data, int argc, char **argv);
void *philosopher(void *arg);
long long get_time(void);
void smart_sleep(long long time, t_data *data);
void print_status(t_data *data, int id, char *status);
void *routine(void *arg);

#endif

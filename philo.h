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
    int id;                     // Numéro unique du philosophe (1 à N)
    int left_fork;             // Index de sa fourchette gauche
    int right_fork;            // Index de sa fourchette droite
    long long last_meal;       // Timestamp du dernier repas
    int meals_eaten;           // Nombre de repas mangés
    struct s_data *data;       // Pointeur vers les données globales
    pthread_t thread;
    int can_eat;          // Thread du philosophe
} t_philo;

typedef struct s_data
{
    int num_philos;            // Nombre total de philosophes
    int time_to_die;           // Temps maximum sans manger
    int time_to_eat;           // Temps nécessaire pour manger
    int time_to_sleep;         // Temps de sommeil
    int num_must_eat;          // Nombre de repas à manger (-1 si infini)
    long long start_time;      // Timestamp du début de la simulation
    int finished;              // Flag pour arrêter la simulation
    pthread_mutex_t *forks;    // Tableau des mutex pour les fourchettes
    pthread_mutex_t write_mutex; // Mutex pour l'affichage
    t_philo *philos;          // Tableau des philosophes
} t_data;

// Prototypes des fonctions
int init_data(t_data *data, int argc, char **argv);
void *philosopher(void *arg);
long long get_time(void);
void smart_sleep(long long time, t_data *data);
void print_status(t_data *data, int id, char *status);
void *routine(void *arg);
int checkdeath(t_philo *philo);
void print_fork_message(t_philo *philo);
void    takeforks(t_philo *philo);
int	start_simulation(t_data *data);
void	clean_simulation(t_data *data);
void    eat(t_philo *philo);
void    release_forks(t_philo *philos);
#endif

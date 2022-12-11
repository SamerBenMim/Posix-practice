/* Définition des bibliothéques */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>


/* Déclaration des camions et des voitures */
#define Camions 10
#define Voitures 10
#define Vehicules  (Camions+Voitures)


/* Déclaration des méthodes */
void* camion(void* arg);
void* voiture(void* arg);


/* Déclaration du verrou */
pthread_mutex_t mut_sc;


/* Déclaration des sémaphores */
sem_t sem_camion;
sem_t sem_voiture;


/* Déclaration des variables qui mémorisent le nombre de camions et de voitures en attente d'accés au pont */
int nb_camion=0;
int nb_voiture=0;


/* Déclaration d'une variable seuil qui mémorise le poids encore disponible */
int s=0;


/* Gestion du pont */
/* Allouer le pont */
void utiliser_le_pont(int tonne)
{
pthread_mutex_lock(&mut_sc);
        if(s+tonne <= 15)
        {
                s=s+tonne;
                if (tonne==5)
                        sem_post(&sem_voiture);
                else
                        sem_post(&sem_camion);
        }
        else
        {
                if (tonne==5)
                        nb_voiture++;
                else
                        nb_camion++;
        }
        pthread_mutex_unlock(&mut_sc);
        if(tonne==5)
                sem_wait(&sem_voiture);
        else
                sem_wait(&sem_camion);
}
/* Libérer le pont */
void liberer_le_pont(int tonne)
{
        pthread_mutex_lock(&mut_sc);
        s=s-tonne;
        if ((s==0) && (nb_camion!=0))
        {
                nb_camion--;
                sem_post(&sem_camion);
                s=15;
        }
        else
                while ((s < 15) && (nb_voiture>0) && (nb_camion==0))
                {
                        nb_voiture--;
                        s=s+5;
                        sem_post(&sem_voiture);
                }
        pthread_mutex_unlock(&mut_sc);
}


/* Programme principal */
int main(int argc, char* argv[])
{
        int i;
        pthread_t id;
        /* Initialisation du verrou */
        pthread_mutex_init(&mut_sc, NULL);
        /* Initialisation des sémaphore */
        sem_init(&sem_voiture,0,0);
        sem_init(&sem_camion,0,0);

        for(i=0; i<Vehicules; i++)
        {
                int* j=(int*)malloc(sizeof(int));
                *j=i;
                if (i<Camions)
                        pthread_create(&id, NULL, camion, j);
                else
                        pthread_create(&id, NULL, voiture, j);
        }
        pthread_exit(NULL);
}


/* Implémentation des méthodes */
void* camion(void* arg)
{
        int pid=*((int*)arg);
        sleep(3);
        utiliser_le_pont(15);
        printf("Le camion %d traverse le pont \n",pid);
        sleep(3);
        printf("Le camion %d quitte le pont \n",pid);
        liberer_le_pont(15);
        pthread_exit(NULL);

}

void* voiture(void* arg)
{
int pid=*((int*)arg);
        sleep(3);
        utiliser_le_pont(5);
        printf("La voiture %d traverse le pont \n",pid);
        sleep(3);
        printf("La voiture %d quitte le pont \n",pid);
        liberer_le_pont(5);
        pthread_exit(NULL);
}
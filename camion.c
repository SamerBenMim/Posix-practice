/* Définition des bibliothéques */
#include <stdio.h>
#include <stdlib.h>
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
int tirage_aleatoire(double max);
void attendre(double max);


/* Déclaration du verrou */
pthread_mutex_t mut_sc;


/* Déclaration du sémaphore */
sem_t evt;


/* Gestion du pont */
/* Allouer le pont */
void utiliser_le_pont(int tonne)
{
        if (tonne==15)
        {
                pthread_mutex_lock(&mut_sc);
                while(tonne>0)
                {
                        sem_wait(&evt);
                        tonne=tonne-5;
                };
                pthread_mutex_unlock(&mut_sc);
        }
        else sem_wait(&evt);
}
/* Libérer le pont */
void liberer_le_pont(int tonne)
{
        while(tonne>0)
        {
                sem_post(&evt);
                tonne=tonne-5;
        };
}


/* Programme principal */
int main(int argc, char* argv[])
{
        int i;
        pthread_t id;
        /* Initialisation du verrou */
        pthread_mutex_init(&mut_sc, NULL);
        /* Initialisation du sémaphore */
        sem_init(&evt,0,3);
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
        utiliser_le_pont(15);
        printf("Le camion %d traverse le pont \n",pid);
        attendre(6);
        printf("Le camion %d quitte le pont \n",pid);
        liberer_le_pont(15);
        pthread_exit(NULL);

}

void* voiture(void* arg)
{
        int pid=*((int*)arg);
        utiliser_le_pont(5);
        printf("La voiture %d traverse le pont \n",pid);
        attendre(6);
        printf("La voiture %d quitte le pont \n",pid);
        liberer_le_pont(5);
        pthread_exit(NULL);
}

int tirage_aleatoire(double max)
{
        int j=(int) (max*rand()/(RAND_MAX+1.0));
        if(j<1)
                j=1;
        return j;
}

void attendre(double max)
{
        struct timespec delai;
        delai.tv_sec=tirage_aleatoire(max);
        delai.tv_nsec=0;
        nanosleep(&delai,NULL);
}

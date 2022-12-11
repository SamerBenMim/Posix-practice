#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

typedef struct
{
    int id;
    char point_depart;
    char point_arrive;
}train;

pthread_mutex_t AB, BC, CD, EB, FA;
//
//
void *move_AB(int id)
{
    pthread_mutex_lock(&AB);
    printf("Train %d : utilise segment AB \n", id);
    usleep(6000000);
    pthread_mutex_unlock(&AB);
}

void *move_BC(int id)
{
    pthread_mutex_lock(&BC);
    printf("Train %d : utilise segment BC \n", id);
    usleep(6000000);
    pthread_mutex_unlock(&BC);
}

void *move_CD(int id)
{
    pthread_mutex_lock(&CD);
    printf("Train %d : utilise segment CD \n", id);
    usleep(6000000);
    pthread_mutex_unlock(&CD);
}

void *move_EB(int id)
{
    pthread_mutex_lock(&EB);
    printf("Train %d : utilise segment EB \n", id);
    usleep(6000000);
    pthread_mutex_unlock(&EB);
}

void *move_FA(int id)
{
    pthread_mutex_lock(&FA);
    printf("Train %d : utilise segment FA \n", id);
    usleep(6000000);
    pthread_mutex_unlock(&FA);
}

void *commencer_par_A(train* t)
{
    if (t->point_arrive == 'B')
        move_AB(t->id);
    else if (t->point_arrive == 'C')
    {
        move_AB(t->id);
        move_BC(t->id);
    }
    else
    {
        move_AB(t->id);
        move_BC(t->id);
        move_CD(t->id);
    }
    
}
void *commencer_par_F(train* t)
{
    if (t->point_arrive == 'A')
        move_FA(t->id);
    else
    {
        move_FA(t->id);
        commencer_par_A(t->point_arrive);
    }
}
void *commencer_par_E(train* t)
{
    if (t->point_arrive == 'B')
        move_EB(t->id);
    else if (t->point_arrive == 'C')
    {
        move_EB(t->id);
        move_BC(t->id);
    }
    else 
    {
        move_EB(t->id);
        move_BC(t->id);
        move_CD(t->id);
    }
}
void * partir(void *arg){
	train* t = (train*) arg;
	if (t->point_depart=='A')
    commencer_par_A(t);
    else if (t->point_depart=='F')
    commencer_par_F(t);
    else 
    commencer_par_E(t);
}

 train createTrain(int id,char A , char B ){
 	train t ;
 	t.id= id;
 	t.point_arrive = B;
	t.point_depart = A;
	return t;
}

int main()
{
    pthread_t train_id;
	pthread_mutex_init(&AB, 0);
    pthread_mutex_init(&BC, 0);
    pthread_mutex_init(&CD, 0);
    pthread_mutex_init(&FA, 0);
    pthread_mutex_init(&EB, 0);
	int nbr_train = 2;
	int t=nbr_train;

//	 	srand(time(NULL));
//	int nbr_train = 10;
//	 while(t--){
//	 	train t0 ;
//	 	int x =rand()%3; 
//	 	if(x==0) {
//	 		int y =rand()%3; 
//	 							 t0 = createTrain(1,'A',(char) ('A'+ y)) ;
//		 }
//		 else if(x==1) {
//		 	int y =rand()%3; 
//	 			 				 t0 = createTrain(2,'E',(char) ('A'+y)) ;
//		 }
//		else if(x==2) {
//			int y =rand()%3; 
//	 			 				 t0 = createTrain(3,'F',(char) ('A'+ y)) ;
//		 }
//	 	
//		pthread_create(&train_id, NULL, partir,&t0);
//
//	 } 

int x= 3 ;
while(x--){
	 	train t0,t1,t2,t3,t4,t5 ,t6;

	 	t0 = createTrain(1,'B',(char) ('D')) ;
	 	t1 = createTrain(2,'F',(char) ('A')) ;	
	 	t2 = createTrain(3,'F',(char) ('D')) ;	
	 	t3 = createTrain(4,'A',(char) ('C')) ;	
	 	t4 = createTrain(5,'E',(char) ('C')) ;	
	 	t5 = createTrain(6,'F',(char) ('A')) ;	
		t6 = createTrain(7,'A',(char) ('D')) ;
		pthread_create(&train_id, NULL, partir,&t1);
		pthread_create(&train_id, NULL, partir,&t2);
		pthread_create(&train_id, NULL, partir,&t0);
		pthread_create(&train_id, NULL, partir,&t3);
		pthread_create(&train_id, NULL, partir,&t4);
		pthread_create(&train_id, NULL, partir,&t5);
		pthread_create(&train_id, NULL, partir,&t6);


	 } 

	
	int i;
    for ( i = 0; i < 7; i++)
    {
        pthread_join(train_id, NULL);
    }
    return 0;
}
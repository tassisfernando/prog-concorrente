#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>

#define NTHREADSH 10
#define NTHREADSO 5

sem_t em, hidro, oxi, bar; //semáforos de exclusão mútua e de condição
int contH = 0, contO = 0; //contadores para número de threads H e O esperando na barreira
int h = 0; //contador das threads de H na formação da molécula

void fazLigacao(){
    printf("Ligacao H{%d} e O{%d}\n", contH, contO);
}

void *H(void * a) { //código das threads de hidrogênio
    sem_wait(&em);
    contH++;
    if ((contH > 1)&&(contO > 0)) { //formar molécula
        contH = contH - 2;
        contO--; //decrementa os contadores de O e H
        sem_post(&hidro); //libera a outra thread de H
        sem_post(&oxi); //libera a thread de O
    } else {
        sem_post(&em); //libera a exclusão mútua
        sem_wait(&hidro); //bloqueia para esperar as outras threads
    }
    h++; //contabiliza as threads de H da molécula
    if (h == 2) 
        sem_post(&bar); //libera a barreira
    fazLigacao();
    return 0;
}

void *O(void * a) { //código das threads de oxigênio
    sem_wait(&em);
    contO++;
    if (contH > 1) { //formar molécula
        contH = contH - 2;
        contO--; //decrementa os contadores de O e H
        sem_post(&hidro);
        sem_post(&hidro); //libera duas threads de H
    } else {
        sem_post(&em); //libera a exclusão mútua
        sem_wait(&oxi); //bloqueia para esperar as outras threads
    }
    sem_wait(&bar); //espera as duas threads de H para liberar a barreira
    h = 0;
    sem_post(&em); //libera a exclusão mútua (permite a formação de novas moléculas)
    fazLigacao();
    return 0;
}

//função principal
int main(int argc, char *argv[]) {
    sem_init(&em, 0, 1);
    sem_init(&hidro, 0, 0);
    sem_init(&oxi, 0, 0);
    sem_init(&bar, 0, 0);

    pthread_t tidH[NTHREADSH];
    pthread_t tidO[NTHREADSO];

    int t, *id;
    for(t=0; t<NTHREADSH; t++) {
        //aloca espaço para o identificador da thread
        if ((id = (int *) malloc(sizeof(int))) == NULL) {
            pthread_exit(NULL); 
            return 1;
        }
        *id=t;
        printf("--Cria a thread H %d\n", t);
        if (pthread_create(&tidH[t], NULL, H, (void*) id)) {
            printf("--ERRO: pthread_create()\n"); 
            return -1;
        }
    }
    for(t=0; t<NTHREADSO; t++) {
        //aloca espaço para o identificador da thread
        if ((id = (int *) malloc(sizeof(int))) == NULL) {
            pthread_exit(NULL); 
            return 1;
        }
        *id=t;
        printf("--Cria a thread O %d\n", t);
        if (pthread_create(&tidO[t], NULL, O, (void*) id)) {
            printf("--ERRO: pthread_create()\n"); 
            return -1;
        }
    }

    //--espera todas as threads terminarem
    for (t=0; t<NTHREADSH; t++) {
        if (pthread_join(tidH[t], NULL)) {
            printf("--ERRO: pthread_join() \n"); 
            return -1;
        } 
    } 
    for (t=0; t<NTHREADSO; t++) {
        if (pthread_join(tidO[t], NULL)) {
            printf("--ERRO: pthread_join() \n"); 
            return -1;
        } 
    } 

    printf("Terminou!\n");
    return 0;
}
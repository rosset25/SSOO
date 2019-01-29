#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_HILOS 11

//pthread_mutex_t mutex;
int principio, final, n_hilos;


/* HILO SUB9RDINADO COGÍA VARIABLE V,
 * S = número del inicio y i= num hilo
 * final = final
 * variable v empieza num hilo (i) -> s+i
 * 
 * 
 */
void *hilo_sub(void *arg)
{
 
  int i = (int)arg;
  int v = principio + i;
 
  while(v <= final)
  {
     q_write(v);
     printf("Soy el hilo %d, Escribo en la cola: %d.\n",i,v);
     v = v + n_hilos; 
  }
  
  pthread_exit(0);
  
}


main(int argc, char *argv[]){
  
  if (argc < 3){printf("Argumentos invalidos.\n");exit(-1);} /*Si no se introducen los argumentos necesarios salimos.*/
  
  int distancia;
  n_hilos = atoi(argv[1]);
  principio = atoi(argv[2]);
  final = atoi(argv[3]); 
  distancia = (final - principio)+1;
  
  if (final <= principio && n_hilos >= distancia)  
  {
    printf("Error en los argumentos.\n");
    exit(-2);
  }
    
  int i,val;
  q_create(distancia); 
  
  pthread_t hilo[MAX_HILOS];
  //pthread_mutex_init(&mutex, NULL);
  
  
  for (i=0; i<n_hilos; i++)
    pthread_create(&hilo[i], NULL, hilo_sub, (void *)i);
 
  
  for (i=0; i<n_hilos; i++)
    pthread_join(hilo[i],NULL);
  
  
  printf("\n");
  for (i=0; i < distancia; i++){
    q_read(&val,i);
    printf("Leo de posicion %d del buffer: %d\n",i,val);
  }
  q_destroy();
  pthread_exit(0);
}

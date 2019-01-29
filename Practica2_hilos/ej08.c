#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_J 8 /*columna*/
#define MAX_I 5 /*fila*/
#define MAX_HILOS 10

int n_hilos; /*Numero de hilos que va a utilizar.*/
int totalOperaciones=0;
int B[MAX_J] = {0,1,0,1,0,1,0,1};

int A[MAX_I][MAX_J] = 
  {{0,1,2,3,4,5,6,7},
   {1,2,3,4,5,6,7,8},
   {2,3,4,5,6,7,8,9},
   {3,4,5,6,7,8,9,0},
   {4,5,6,7,8,9,0,1}};

//Resultado C: {16,20,24,18,22}
int C[MAX_I] = {0,0,0,0,0};

/*Lugar reservado para los parámetros que usaremos en multiplica.
  Es como una clase dentro de una clase que guarda la información,
  y esa clase se llama parametros*/

typedef struct {
    int n_filas; /*número de filas que le toca a cada hilo.*/
    int fila; /*Fila donde empieza el hilo.*/ 
}parametros;


void *multiplica(void *arg) {

    int fila, n_filas, i, j, total, contador;
    
    contador=0;
    parametros * argum; 	/*Reservamos una zona de tipo parametros.*/
    argum=(parametros *)arg;    /*Lo asignamos a el argumento de la función.*/
    
    fila = argum->fila;         /*fila en la que empieza.*/
    n_filas = argum->n_filas;   /*Cuantas filas tiene que realizar el hilo.*/
    
    for(j=0; j < n_filas; j++,fila++)  /*Cada iteración es una fila.*/
    {
    	total=0;
    	
        for(i=0; i < MAX_J; i++) {      /*Cada iteración es un elemento de la columna.*/
           total += A[fila][i] * B[i]; /*Realizamos la multiplicación.*/
           contador++;
        }
     	C[fila]=total;
     }
     pthread_exit((void *)contador);
  }


int main(int argc, char **argv){
    
    int i;
    parametros * argum;
    
    if(argc != 2){printf("Se esperaba el número de hilos.\n");return(-1);}
    
    n_hilos = atoi(argv[1]);
    
    if(n_hilos > MAX_HILOS || n_hilos > MAX_I){printf("Número de hilos excesivo o existen mas hilos que filas.\n");return(-1);}
    
    pthread_t hilo[MAX_HILOS]; /*Vector de hilos.*/
    
    int num_filas = MAX_I/n_hilos;
    int fila = 0;
    
    for(i = 0; i < n_hilos-1; i++){ 	    /*vamos inicializando hilos.*/
   
        argum=(parametros *)malloc(sizeof(argum));
        argum->n_filas=num_filas;
        argum->fila=fila;
        fila += num_filas;
        
        pthread_create(&hilo[i],NULL,&multiplica,(void *)argum);
    }
    
    /*Ultimo */
    argum=(parametros *)malloc(sizeof(argum));
    argum->n_filas= (MAX_I - num_filas*n_hilos)+1;
    argum->fila=fila;
    
    pthread_create(&hilo[i],NULL,&multiplica,(void *)argum);


    int retornoDatos;
    int total_hilos = n_hilos;
    for(i = 0; i < total_hilos; i++){ 	/* join a los hilos, los recojemos.*/
        pthread_join(&hilo[i],(void *)&retornoDatos);
        printf("El hilo %d ha realizado %d operaciones.\n",i,retornoDatos);
        totalOperaciones = totalOperaciones + retornoDatos;
    }
    
   printf("Se han realizado %d operaciones.\n",totalOperaciones);
   pthread_exit(0);
}


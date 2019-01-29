#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_J 8 /*columna*/
#define MAX_I 5 /*fila*/
#define MAX_HILOS 10

int n_hilos; /*Numero de hilos que va a utilizar.*/

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
   //Código anterior
  /* int n_filas; /*número de filas que le toca a cada hilo.*/
   /* int fila; /*Fila donde empieza el hilo.*/ 
   
   //Código modificado
   int iini;
   int ifin;
   int nhilo;
   /* En este caso tenemos 3 variables universales, que hacen más fácil el
    * cálculo y comprensión de la multiplicación y suma entre matriz y vector,
    * además de saber de dónde a dónde van los hilos creados
    */
    
}parametros;


void *multiplica(void *arg) {

    int i, j, total=0, ini, fin, nhilo, mult, elem = 0;
    
    parametros * argum; 	/*Reservamos una zona de tipo parametros.*/
    argum=(parametros *)arg;    /*Lo asignamos a el argumento de la función.*/
    
    ini=argum->iini;
    fin=argum->ifin;
    nhilo=argum->nhilo;
  
    //Multiplicación de los elementos del vector correcpondientes a cada hilo
    for(i=ini; i <= fin; i++) {
      for(j=0; j < MAX_J; j++) {
	printf("Hilo %d, A[%d][%d]= %d\n", nhilo, i, j, A[i][j]);
	mult= A[i][j]*B[j];
	total += mult;
	elem++;
      }
      printf("suma: %d \n", total);
      C[i]= total;
      total = 0;
    }
  
     pthread_exit(0);
  }


int main(int argc, char **argv){
    
    int i, elem;
    parametros * argum;
    
    if(argc != 2){printf("Se esperaba el número de hilos.\n");return(-1);}
    
    n_hilos = atoi(argv[1]);
    
    if(n_hilos > MAX_HILOS || n_hilos > MAX_I){printf("Número de hilos excesivo o existen mas hilos que filas.\n");return(-1);}
    
    pthread_t hilo[MAX_HILOS]; /*Vector de hilos.*/
    
    int num_filas = MAX_I/n_hilos;
    //int fila = 0;
    
    for(i = 0; i < n_hilos-1; i++){ 	    /*vamos inicializando hilos.*/
   
        argum=(parametros *)malloc(sizeof(argum));
		argum->iini=i*num_filas;
		argum->ifin=((argum->iini)+ num_filas-1);
		argum->nhilo=i;
       
        pthread_create(&hilo[i],NULL,&multiplica,(void *)argum);
    }
    
    /*Asignación de elementos al útimo hilo*/
    i= n_hilos -1;
    argum=(parametros *)malloc(sizeof(argum));
    argum->iini=i*num_filas;
    argum->ifin=MAX_I-1; 
    argum->nhilo=i;
  
    pthread_create(&hilo[i],NULL,&multiplica,(void *)argum);


    
    int total_hilos = n_hilos;
    for(i = 0; i < total_hilos; i++){ 	/* join a los hilos, los recojemos.*/     
        pthread_join(hilo[i],NULL);
    }
    
    for(i=0;i<MAX_I-1;i++)  // Imprimimos la matriz (menos el último elemento) para comprobar el resultado
        printf("%d, ",C[i]);
    printf("%d", C[MAX_I-1]); /* Se imprime el último elemento (es simplemente estético, para que no 
				 no salga la última coma*/
   
   printf("\n");
   pthread_exit(0);
}


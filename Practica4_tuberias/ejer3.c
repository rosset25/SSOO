/*
 * Programa al que se le pasan 3 argumentos: dirctorio, fichero de errores y fichero donde sacar el wc
 * Además, tanto ls como wc se sacarán por pantalla 
 * El proceso replicador que se encarga de leer de la tubería t4,será el Abuelo
 * El proceso replicador al cual lee de t1 será el Padre, es decir, hijo del proceso Abuelo
 * Los hijos del proceso Padre serán ls, grep y wc
 *
 *  ls -al argv[1] 2> argv[2] | grep ^d | wc -l
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main( int argc, char *argv[]) {
  
  int t1[2], t2[2], t3[2], t4[2];
  int fd1, fdError;
  char dato;
  
  //Comprobación de número de parámetros correcto:
  if ( argc != 4) {
    printf("Debes introducir 3 argumentos: directorio, fichero de errores, fichero para ls");
    exit(-1);
  }
  
  pipe(t1);
  pipe(t2);
  pipe(t3);
  pipe(t4);
  
  fd1 = open(argv[3], O_WRONLY | O_CREAT, 0666);
  
  if(fork() != 0) { //Abuelo Proceso replicador 2
    close(t1[0]), close(t1[1]);
    close(t2[1]), close(t2[1]);
    close(t3[0]), close(t3[1]);		
    close(t4[1]);

    wait(NULL);
   

    while(read(t4[0], &dato, sizeof(dato)) > 0) {
	    write(1, &dato, sizeof(dato));
	    write(fd1, &dato, sizeof(dato));
    }
    
   close(t4[0]);
   exit(0);
    
  }else{ //Padre proceso replicador 1
    
    if (fork() == 0) { //Hijo1 ls -l
      close(t1[0]);
      close(t2[0]), close(t2[1]);
      close(t3[0]), close(t3[1]);		
      close(t4[0]), close(t4[1]);

      close(1);
      dup(t1[1]);
      close(t1[1]);
      
      close(2);
      fdError = open(argv[2], O_WRONLY | O_CREAT, 0666);

      execlp("ls", "ls", "-al", argv[1], NULL);
      perror("Fallo al ejecutar ls");
      exit(-1);
      
    }
      
    if (fork() == 0) { //Hijo grep ^d
	close(t1[0]), close(t1[1]);
	close(t2[1]);
	close(t3[0]);	
	close(t4[0]), close(t4[1]);

	close(0);
	dup(t2[0]);
	close(t2[0]);

	close(1);
	dup(t3[1]);
	close(t3[1]);

	execlp("grep", "grep", "^d", NULL);
	perror("Error en ejecución del grep");
	exit(-1);	
    }
      
      
    if( fork() == 0) { //Hijo3 wc -l
      close(t1[0]), close(t1[1]);
      close(t2[1]), close(t2[1]);
      close(t3[1]);	
      close(t4[0]);

      close(0);
      dup(t3[0]);
      close(t3[0]);

      close(1);
      dup(t4[1]);
      close(t4[1]);
	  
      execlp("wc", "wc", "-l", NULL);
      perror("Error al ejecutar wc");
      exit(-1);
      
    }
    
    //Padre proceso replicador:
    
    close(t1[1]);
    close(t2[0]);
    close(t3[0]), close(t3[1]);		
    close(t4[0]), close(t4[1]);
    
    while(read(t1[0], &dato, sizeof(dato)) > 0) {
	    write(t2[1], &dato, sizeof(dato));
	    write(fd1, &dato, sizeof(dato));	
	    write(1, &dato, sizeof(dato));
	        
    }
   
     close(t1[0]), close(t2[1]);
     exit(0);
        
  }
}  

  

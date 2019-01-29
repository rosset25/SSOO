/*
 * En este ejercicio pasamos através de tuberías la información para ejecutar 
 * una serie de comandos:
 * cat < argv[1] | grep a | wc -l >> argv[2] 
 * 
 * La jerarquía consta de H1 (cat), H2 (grep a), y el padre (wc)
 * Prodría haber sido otra jerarquía como H1 (cat), Padre (grep a) y abuelo (wc)
 * 
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main( int argc, char *argv[]) {
  
  int t1[2], t2[2];
  /*int fd; realmente no hace falta guardarnos la entrada de 
   * los ficheros, se han redireccionado a la EE y la SE
   */
  //Comprobación de número de parámetros correcto:
  if ( argc != 3) {
    printf("Debes introducir 2 argumentos");
    exit(-1);
  }
  
  pipe(t1); //AÑADIR comprobación de tuberías
  pipe(t2);

  if(fork() == 0) { //Hijo grep a

    if(fork() == 0) { //Hijo cat < fichero
      close(t1[0]);
      close(t2[0]), close(t2[1]);
    
      close(0);
      open(argv[1],O_RDONLY);
   
      close(1);
      dup(t1[1]);
      close(t1[1]);

      execlp("cat","cat",NULL);
      perror("Error en ejecución de cat");
      exit(-1);
   }
   else{ //Padre grep a 
      close(t1[1]), close(t2[0]);
      
      close(0);
      dup(t1[0]);
      close(t1[0]);
      
      close(1);
      dup(t2[1]);
      close(t2[1]);
      
      execlp("grep","grep","a",NULL);
      perror("Error en ejecución de grep");
      exit(-1);
      
    }
  } 
  else{	//Abuelo wc -l
    close(t1[0]), close(t1[1]);
    close(t2[1]);
      
    close(0);
    dup(t2[0]);
    close(t2[0]);
      
    close(1);
    open(argv[2],O_WRONLY|O_CREAT|O_APPEND,0600);  // >> argv[2] (append)
      
    execlp("wc","wc","-l",NULL);      
    perror("Error en ejecución de wc");
    exit(-1);
 }

}
  
    

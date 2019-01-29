#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX 255

int main( int argc, char *argv[]) {
  
  int t1[2], t2[2], t3[2];
  int fd, testigo, i;
  char dato;
  
  //Comprobación de número de parámetros correcto:
  if ( argc != 2) {
    printf("Debes introducir el fichero a leer");
    exit(-1);
  }
  
  pipe(t1);
  pipe(t2);  
  pipe(t3);
  testigo=1;
  
  fd=open(argv[1],O_RDONLY);
  
  
  if(fork() != 0) { 	//Padre 
    
    write(t3[1],&testigo,sizeof(int));
    
  }else{ //Hijo1 P1
  
    close(t2[1]), close(t2[0]);
    close(t1[0]), close(t3[1]);
    
    i=1;
    while((read(t3[0],&testigo,sizeof(int))>0) && i>0){
    	printf("P1: ");
    	do{
    		i=(read(fd,&dato,sizeof(char)));
    		write(1,&dato,sizeof(char));	
    	}while(dato!='\n');
    	
    	write(t1[1],&testigo,sizeof(int));
    }
    close(t1[1]);
    close(t3[0]);   
  }
  
  if(fork()== 0) {	//Hijo2 P2
  
    close(t2[0]), close(t1[1]);
    close(t3[0]), close(t3[1]);
    
    i=1;
    while((read(t1[0],&testigo,sizeof(int))>0) && i>0){
    	printf("P2: ");
    	do{
    		i=(read(fd,&dato,sizeof(char)));
    		write(1,&dato,sizeof(char));	
    	}while(dato!='\n');
    	
    	write(t2[1],&testigo,sizeof(int));
    }
    close(t1[0]);
    close(t2[1]); 
   } 
      
  if(fork() == 0) //Hijo P3
  {  
  	close(t1[0]),close(t1[1]);
  	close(t2[1]);
  	close(t3[0]);
    
    i=1;
    while((read(t3[0],&testigo,sizeof(int))>0) && i>0){
    	printf("P3: ");
    	do{
    		i=(read(fd,&dato,sizeof(char)));
    		write(1,&dato,sizeof(char));	
    	}while(dato!='\n');
    	
    	write(t1[1],&testigo,sizeof(int));
    }
    close(t2[0]);
    close(t3[1]); 
  }

  close(t1[0]), close(t1[1]);
  close(t2[0]), close(t2[1]);
  close(t3[0]), close(t3[1]);

  wait(NULL);
  exit(0); 
}

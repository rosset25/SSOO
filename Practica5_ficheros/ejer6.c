#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<dirent.h>
#include<sys/file.h>


int main(int argc, char *argv[]) {
  
  int t1[2], t2[2];
  
  struct stat datos;   
  char fichero[256];
  DIR *dir;
  struct dirent *dp;
  
  
  if( argc != 2) {
	printf("Debes introducir el nombre de un directorio\n");
	exit(-1);
  }

  if (stat(argv[1],&datos) != 0){       
    printf("Error. Error al obtener atributos del directorio %s\n", argv[1]);
    exit(1);
  }
  
  if(!S_ISDIR(datos.st_mode)) { 
    printf("El parámetro introducido no es un directorio\n");
    exit(-1);
  }
    
    
  if((dir=opendir(argv[1])) == NULL) {
      printf("Error al abrir el fichero\n");
      exit(-1);
   }
  
   
   while((dp=readdir(dir))!=NULL) {
     if((strcmp(dp->d_name, ".") && strcmp(dp->d_name,".."))) {
	
       sprintf(fichero,"%s/%s",argv[1],dp->d_name);  //se tiene que hacer o dará error  
       if(lstat(fichero, &datos) != 0) {
	    printf("Error en el stat\n");
	    exit(-1);
       }
       
	if(S_ISDIR(datos.st_mode)) {
	   printf("Files and directories of %s: ",fichero);
	   fflush(stdout);
	    
	    if(fork() != 0) {
	      wait(NULL);
	      printf("\n");
	      
	    }
	    else{
	      
	      pipe(t1), pipe(t2);
	      
	      if(fork() != 0) {  //Padre wc
		  close(t2[1]);
		  close(t1[1]), close(t1[0]);
		 
		  close(0);
		  dup(t2[0]);
		  close(t2[0]);
		  
		  execlp("wc","wc","-l",NULL);
		  perror("Error al ejecutar wc\n");
		  exit(-1);
		
	      }else{ 
		  if(fork() != 0) { //Hijo tail
		      close(t1[1]);
		      close(t2[0]);
		      
		      close(0);
		      dup(t1[0]);
		      close(t1[0]);
		      
		      close(1);
		      dup(t2[1]);
		      close(t2[1]);
		      
		      execlp("tail","tail","-n","+2",NULL);
		      perror("Error al ejecutar tail\n");
		      exit(-1);
		      
		  }else{ //Nieto ls
		    
		      close(t2[1]), close(t2[0]);
		      close(t1[0]);
		      
		      close(1);
		      dup(t1[1]);
		      close(t1[1]);
		      
		      execlp("ls","ls","-l",fichero,NULL);
		  }
	      } 
	    }
	}
       
     }
  
  
   }
  
  
  
  
  
  
  
  
  
}
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<dirent.h>
#include<sys/file.h>



func(char *direc, int espacio) {
  int i;
  char fichero[256];
  struct stat datos;
  DIR *dir;
  struct dirent *dp;
  
  if((dir=opendir(direc)) == NULL) {
    printf("Error al abrir el fichero\n");
    exit(-1);
  }
  
  while((dp=readdir(dir))!=NULL)  {
   
    if((strcmp(dp->d_name, ".") && strcmp(dp->d_name,".."))) {
       
      for(i=0; i<espacio; i++){ //creación de espacio
          printf(" ");
      }

     sprintf(fichero, "%s/%s", direc, dp->d_name);
     if(lstat(fichero, &datos) != 0) {
       printf("Error en el stat\n");
       exit(-1);
     }
     
     if(S_ISDIR(datos.st_mode)) {
       printf("Directory %s:\n", fichero);
       func(fichero, espacio+2);
     } else {
       printf("%s\n", fichero);
     }
     
   }
  }

  
}



int main(int argc, char *argv[]) {
    
    struct stat datos;
    
    lstat(argv[1],&datos);
    
    if(!S_ISDIR(datos.st_mode)) {
        if(!S_ISREG(datos.st_mode) && !S_ISFIFO(datos.st_mode) && !S_ISLNK(datos.st_mode)) { // Con esto sabemos que no existe lo indicado.
            printf("El directorio no existe, creamos el directorio con nombre -%s-\n",argv[1]);
	    mkdir(argv[1],0755);
            exit(0);
	    
        }else{
            printf("%s no es un directorio\n", argv[1]); // Sabemos que existe y que es otro tipo de elemento.
            exit(-1);
        }
    }
    //Si el argumento pasado como parámetro es un directorio que ya existe
    func(argv[1],2); //el entero que se le pasa es para calcular el número de espacios

    exit(0);
}
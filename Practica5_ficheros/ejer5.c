/*
 * NOTA: 
 *   Este programa no contempla el caso de que el fichero original pasado como
 *   parámetro esté en una de las carpetas buscadas, lo que quiere decir que 
 *   lo contará como un enlace duro más del fichero
 */

#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<dirent.h>
#include<sys/file.h>

int n=0;

func(char *direc, int inodo) {
  
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

     sprintf(fichero, "%s/%s", direc, dp->d_name);
     if(lstat(fichero, &datos) != 0) { //lstat para detectar los enlaces simbólicos
       printf("Error en el stat\n");
       exit(-1);
     }
     
     if(S_ISDIR(datos.st_mode)) {
       func(fichero, inodo);
       
     } else {
       
        if(S_ISREG(datos.st_mode)){ 
	    int ino = datos.st_ino;
	    if ( ino == inodo) {
		printf("(%d) %s\n", ino, fichero);
		n = n + 1;
	    }
        }
       
     }
     
   }
  }

  
}



int main(int argc, char *argv[]) {
    
    struct stat datos;
    struct stat datos2;
    DIR *dir;
    int ino;
    
    lstat(argv[1],&datos); 
    if(!S_ISDIR(datos.st_mode)) {
        if(!S_ISREG(datos.st_mode) && !S_ISFIFO(datos.st_mode) && !S_ISLNK(datos.st_mode)) { 
	    //Comprobamos que no existe el directorio pasado como primer parámetro 
            printf("El directorio no existe, creamos el directorio con nombre -%s-\n",argv[1]);
	    mkdir(argv[1],0755); //se crea y termina
            exit(0);
	    
        }else{
            printf("%s no es un directorio\n", argv[1]); // Sabemos que existe pero no es un directorio
            exit(-1);
        }
    }
    
    lstat(argv[2],&datos2); //debe ser lstat para detectar enlaces blandos
    if(!S_ISREG(datos2.st_mode)){
      printf("No es un fichero regular o no existe\n");
      exit(-1);
    }
    
    ino = datos2.st_ino; //nos guardamos el valor del inodo del fichero pasado como segundo argumento
    printf("inode of %s: %d\n\n", argv[2], ino);
    fflush(stdout);
    
    //Si el argumento pasado como parámetro es un directorio que ya existe
    func(argv[1], ino); //se le pasa el valor del inodo buscado
    
    printf("	Total number of hard links: %d\n", n);    
    
    exit(0);
}
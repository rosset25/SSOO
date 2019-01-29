#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<dirent.h>
#include<sys/file.h>
   

   
void func(char *directorio) {
  
   struct stat datos;   
   char fichero[256];
   DIR *dir;
   struct dirent *dp;
  
   char tipo; //tipo de fichero (regular o directorio)
   
   if((dir=opendir(directorio)) == NULL) {
      printf("Error al abrir el fichero\n");
      exit(-1);
   }
  
   
   while((dp=readdir(dir))!=NULL) {
      sprintf(fichero,"%s/%s",directorio,dp->d_name);  //se tiene que hacer o dará error
      
      if(stat(fichero,&datos) != 0) {	//aquí no se contempla si es un enlace simbólico
	printf("Error en el stat");
	exit(-1);
      }  
     
      switch(datos.st_mode & S_IFMT) {
	case S_IFREG:
	    tipo='R';
	    break;
	case S_IFDIR:
	    tipo='D';
	    break;
      }
  
      printf("%c %s\n",tipo,fichero);   
  }
  
  closedir(0);
  
}


      

int main(int argc, char *argv[])
{
  
    struct stat datos;  
    
    if( argc != 2) {
	printf("Debes introducir el nombre de un directorio\n");
	exit(-1);
    }
  
    lstat(argv[1],&datos); //hacemos el stat independientemente de si se ha tenido éxito o no
    
    if(!S_ISDIR(datos.st_mode)) { /* si se entra en este if, o bien no ha tenido éxito 
				   * el stat o bien argv[1] no es un directorio */
        if(!S_ISREG(datos.st_mode) && !S_ISFIFO(datos.st_mode) && !S_ISLNK(datos.st_mode)) { 
	  // Con esta comprobación nos aseguramos de saber si el directorio existe o no
            printf("El directorio no existe, creamos el directorio con nombre -%s-\n",argv[1]);
	    mkdir(argv[1],0755);
            
	    exit(0); //una vez creado se cierra el programa
	  
	}else{ //en caso de existir, pero no es un directorio se saca un mensaje de error
            printf("%s no es un directorio\n", argv[1]);
            exit(-1);
        }
    }
    
    //Si el directorio argv[1] existe llamamos a la función que sacará por pantalla la información
    func(argv[1]);  

    exit(0);
}
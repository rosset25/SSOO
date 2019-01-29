#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
  
  struct stat datos; //stat
  struct stat datos2; //lstat
  
  char *fichero;
  
  if(argc != 2) {
    printf("Necesario poner como parámetro un fichero o directorio\n");
    exit(-1);
  }
  
  
  if(stat(argv[1],&datos) != 0) { //stat para sacar la información del fichero original
      perror("Error en el stat");
      exit(-1);
  }
      
  if(lstat(argv[1],&datos2) != 0) { //lstat para sacar la información del enlace blando
      perror("Eror en el lstat");
      exit(-1);
  }
      
  if ((S_IFMT & datos2.st_mode) != S_IFLNK ){
      printf("No has introducido un enlace simbólico\n");
      exit(-1); 
  }
  
  char *f;
  f = malloc(datos2.st_size + 1);
  readlink(argv[1],f,datos2.st_size);
  f[datos2.st_size ]='\0';
  
  printf("FILE: %s\n\n",argv[1]);
  
  printf("PERMISIONS:		%o\n",(datos2.st_mode) & (00777)); 
  printf("HARD LINKS:		%d\n",datos2.st_nlink);
  printf("SIZE:			%d\n",datos2.st_size);
  printf("INODE:			%d\n",datos2.st_ino);
  printf("LAST MODIFICATION:	%s\n",asctime(localtime(&datos2.st_mtime)));
  
    
  printf("\n\n");
  
  printf("FILE WHERE %s LINKS: %s\n\n",argv[1],f);
  
  printf("PERMISIONS:		%o\n",(datos.st_mode) & (00777)); 
  printf("HARD LINKS:		%d\n",datos.st_nlink);
  printf("SIZE:			%d\n",datos.st_size);
  printf("INODE:			%d\n",datos.st_ino);
  printf("LAST MODIFICATION:	%s\n",asctime(localtime(&datos.st_mtime)));
  
  exit(0);
  
}   
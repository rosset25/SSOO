#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
  
  struct stat datos;
  struct tm *fecha;
  
  char *fichero;
  
  if(argc != 2) {
    printf("Necesario poner como parámetro un fichero o directorio\n");
    exit(-1);
  }
  
  
  if(stat(argv[1],&datos) != 0) {
      perror("Error en el stat");
      exit(-1);
  }
  printf("\n");
  printf(" %s\n\n", argv[1]);
  
  switch(datos.st_mode & S_IFMT) {
    case S_IFREG:
      printf("TYPE:			Regular File\n");
      break;
    case S_IFDIR:
      printf("TYPE: 			Directory\n");
      break;
    case S_IFLNK:
      printf("TYPE: 			Soft Link\n");
      break;
  }
  
  printf("PERMISIONS:		%o\n",(datos.st_mode) & (00777)); 
  printf("HARD LINKS:		%d\n",datos.st_nlink);
  printf("SIZE:			%d\n",datos.st_size);
  printf("INODE:			%d\n",datos.st_ino);
  printf("LAST MODIFICATION:	%s\n",asctime(localtime(&datos.st_mtime)));
  
  exit(0);
  
}  
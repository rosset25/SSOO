/* producer_consumer.c */

#include <pthread.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#define LINES 200           // Maximum number of lines of the file
#define CHARS_PER_LINE 100  // Maximum length of a line

char array_lines[LINES][CHARS_PER_LINE]; // Vector to store the lines of the file
int file;                                // File descriptor of the file to be read
int num_lines;                           // Number of lines of the file to be read

void *produce_lines(void *arg)
{
  char c;
  int  i , j;
  
  j = 0;
  do {
    i = 0;
    do {                            
       read(file, &c, 1);               // Each line is read char by char
       array_lines[j][i++] = c;
     } while(c != '\n');                // The line ends with "\n" 
    array_lines[j][i-1] = '\0';         // String in C ends with "\0"
       printf("Thread_produce_line: Read line %d= %s \n",j,array_lines[j]);
    j = j + 1;
  }
  while(j < num_lines);                // The thread ends when it reads all the lines of the file
  pthread_exit(0);
}

void *count_lines(void *arg)
{
  int i, j, total_chars=0, total_vowels=0, lenght, vowels;
  
  for (i=0; i<num_lines; i++){        //  The thread ends when it processes all the lines of the file
     lenght = strlen(array_lines[i]);
     vowels = 0; 
     j = 0;                           // The line is read char by char in order to look for the vowels
     while (j < lenght) { 
       if (array_lines[i][j] == 'a' || array_lines[i][j] == 'e' || array_lines[i][j] == 'i' || array_lines[i][j] =='o' || array_lines[i][j] == 'u')
            vowels = vowels+1;
       j = j + 1;
     }
     total_chars  = total_chars  + lenght;
     total_vowels = total_vowels + vowels;
     printf("Thread_count_lines: Read line %i = %s, lenght = %d vowels = %d \n",i,array_lines[i],lenght,vowels);
  }
  printf("Thread_count_lines: Total characters = %d Total vowels = %d\n", total_chars, total_vowels);
  pthread_exit(0);
}

int main(int argc, char *argv[])
{
    int i, j, total_chars, total_vowels;
    char c, line[CHARS_PER_LINE];

    pthread_t thread_produce_lines; // Producer thread 
    pthread_t thread_count_lines;   // Consumer thread


    if (argc != 2) {
      printf ("Expected sintax: %s file \n",argv[0]);
      exit(-1);
    }
  
    if ((file = open(argv[1], O_RDONLY)) < 0) {
      perror("Error while opening the file");
      exit(-1);
    }

    // The main thread reads the number of lines to be processed of the file. 
    i=0;
    do {                      
       read(file, &c, 1);
       line[i++] = c;
    } while (c != '\n');    
    line[i-1] = '\0';
    num_lines = atoi(line) - 1;
    printf("Numero de lineas de %s: %d\n",argv[1],num_lines);

    // The thread_count_lines is created when the thread_produce_lines ends, so that syncronization is not required
    if  ( pthread_create(&thread_produce_lines, NULL, produce_lines, NULL) ) {
      perror("Error creating producer thread.");
      exit(-1);
    }
    if  ( pthread_join(thread_produce_lines,NULL)) {
      perror("Error joining producer thread.");
      exit(-1);
    }

    if  ( pthread_create(&thread_count_lines, NULL, count_lines, NULL) ) {
      perror("Error creating producer thread.");
      exit(-1);
    }
    if  ( pthread_join(thread_count_lines,NULL) ) {
      perror("Error joining producer thread.");
      exit(-1);
    }

    close(file); 

    // The main thread computes the total characters and vowels of the file in order
    // to check that the subordinate threads have been completed the task correctly
    file = open(argv[1], O_RDONLY);

    do {                      
       read(file, &c, 1);
    } while (c != '\n');    

    j = 0; total_chars = 0; total_vowels = 0;
    do {
      do {                              
         read(file, &c, 1);  
         if (c == 'a' || c == 'e' || c == 'i' || c =='o' || c == 'u') total_vowels++;
         total_chars++;
      } while (c != '\n');
      total_chars--;
      j = j + 1;
    }
    while(j < num_lines);
    printf("Main thread:        Total characters = %d Total vowels = %d\n", total_chars, total_vowels);
    close(file); 
    exit (0);
}




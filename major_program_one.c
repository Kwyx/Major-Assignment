#include <stdio.h>
#include <string.h>
#include "unistd.h"

//************************************************************************
// Program Name: major_program_one.c
// Authors: Casey Carroll, Henry Flowers, Jeff Dunning, Matthew Holladay
// To Compile: gcc major_program_one.c -o major_program_one.out
// To Execute: ./major_program_one.out [batch file]
//
// This program will act as a shell allowing input from either the user
// or a batch file.  The program will exit upon receiving a quit command, 
// CTRL-D, or reaching the end of the batch file.
//***********************************************************************
const int max_line_length = 512;

void runInteractiveMode();

int main(int argc, char *argv[]) {
  FILE *batch_file;  //file pointer for batch file
  char buffer[max_line_length];  //buffer to store each line from batch file
  
  switch(argc) {
    case 1 :  //interactive mode
      printf("Interactive Mode %s\n",argv[0]+2);
      runInteractiveMode();
      break;
    case 2 :  //batch mode
      printf("Batch Mode\nFile - %s\n****************************\n",argv[1]);
      
      //MOVE THIS TO A FUNCTION***********************
      batch_file = fopen(argv[1], "r");
      if (!batch_file) {  //check for successful file open
        fprintf(stderr, "Unable to open %s for reading\n",argv[1]);
        return 1;
      }
      while (fgets(buffer, max_line_length, batch_file)) {
        printf("%s",buffer);
      }
      fclose(batch_file);
      //***********************
      break;
    default :  //invalid input
      fprintf(stderr,"Too many arguments\n");
      return 1;
  }
  return 0;
}

void runInteractiveMode () {
  char userInput[100];

  while(1)
  {
    printf("prompt> ");
    fgets(userInput, 100, stdin); //read in user input
    strtok(userInput, "\n"); //removes newline from char array

    //PARSE CHAR ARRAY
    //FORK PROCESS FOR EACH COMMAND

    printf("%s\n", userInput);
  }
}


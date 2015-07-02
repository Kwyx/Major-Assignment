#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

////////////////////////////////////////////////////////////////////////////////
// Program Name: major_program_one.c
// Authors: Casey Carroll, Henry Flowers, Jeff Dunning, Matthew Holladay
// To Compile: gcc major_program_one.c -o major_program_one.out
// To Execute: ./major_program_one.out [batch file]
//
// This program will act as a shell allowing input from either the user
// or a batch file.  The program will exit upon receiving a quit command, 
// CTRL-D, or reaching the end of the batch file.
////////////////////////////////////////////////////////////////////////////////

const int max_line_length = 512;

void runInteractiveMode();
void runBatchMode();
int parseInput(char* input_line);
void runCommand(char* input_command);

int main(int argc, char* argv[]) 
{
  switch(argc) {
    case 1 :  //interactive mode
      runInteractiveMode();
      break;
    case 2 :  //batch mode
      runBatchMode(argv[1]);
      break;
    default :  //invalid input
      fprintf(stderr,"Too many arguments\n");
      return 1;
  }
  return 0;
}

void runInteractiveMode () 
{
  char userInput[max_line_length];
  int exit_flag = 0;
  printf("Interactive Mode\n");
  while(!exit_flag)
  {
    printf("prompt> ");
    fgets(userInput, max_line_length, stdin); //read in user input
    exit_flag = parseInput(userInput);
  }
}

void runBatchMode (char* file) 
{
  char buffer[max_line_length];  //buffer to store each line from batch file
  FILE *batch_file;  //file pointer for batch file
  printf("Batch Mode\nFile: %s\n",file);

  batch_file = fopen(file, "r");
  if (!batch_file)  //check for successful file open
  {  
    fprintf(stderr, "Unable to open %s for reading\n",file);
    //switch to interactive mode here?
    //return 1;
    return;
  }
  while (fgets(buffer, max_line_length, batch_file)) 
  {
//    strtok(buffer, "\n"); //removes newline from char array
    parseInput(buffer);
  }
  fclose(batch_file);
}

int parseInput(char* input_line)
{
  char* token_buffer;
  int pid;
  strtok(input_line, "\n"); //removes newline from char array
  printf("Parse: %s\n",input_line);

  token_buffer = strtok(input_line, ";");
  if(!strcmp(token_buffer, "quit"))
    return 1;
  while(token_buffer != NULL)
  {
    pid = fork();  //fork a child to handle client requests
    if (pid < 0) error("ERROR on fork");
    if (pid == 0)  //child
    {
      printf("Child forked\n");
      runCommand(token_buffer);
    }
    token_buffer = strtok(NULL, ";");
  }
  if (pid > 0) //parent
    {
      printf("Parent waiting\n");
      wait(NULL);
      printf("Parent resuming\n");
    }
  return 0;
}

void runCommand(char* input_command)  //child process
{
  char* args[20];  //hold the arguments
  char** next = args;  //pointer starting at beginning of args array
  char* token_buffer;
  printf("Run: %s\n",input_command);
  token_buffer = strtok(input_command, " ");
  while(token_buffer != NULL)
  {
    *next++ = token_buffer;
    token_buffer = strtok(NULL, " ");
  }
  *next = NULL;
  
  execvp(args[0], args);
}

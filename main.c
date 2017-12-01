#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>










// Loop of a shell : Read, Parse, Execute
void lsh_loop(void){
  char *line;
  char **args;
  int status;
  do{
    printf("> ");
    line = lsh_readline();
    args = lsh_splitline(line);
    status = lsh_exec(args);

    free(line);
    free(args);

  } while(status);
}











//Read a line
#define LSH_RL_BUFSIZE 1024
char *lsh_readline(void){ /*
  //start with a block then reallocmemory if needed

  //declarations
  int bufsize = LSH_RL_BUFSIZE;
  int position = 0;
  char *buffer = alloc(sizeof(char) * bufsize):
  int c;
  if(!buffer){
    fprintf(strderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while(1){
    //Read a character
    //EOF is an int, so we need to store getchar() as int to compare it
    c = getchar();

//if newline or EOF , null terminate the string and return it
    if(c == EOF || c == '\n'){
      buffer[position] = '\0';
      return buffer;
    }

    //else, add the character to our existing string
    else{
      buffer[position] = c;
    }
    position++;

//do we need more memory if yes then realloc
    if(position >= bufsize){
      bufsize += LSH_RL_BUFSIZE;
      buffer = reaaloc(buffer, bufsize);
      if(!buffer){
        fprintf(Strderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  } */
  //instead of doing all that work we use get line;
  char *line = NULL;
  ssize_t bufsize = 0;
  //getline allocates a buffer with a line from stdin
  getline(&line, &bufsize, stdin);
  return line;
}











//Parsing the line
//Take in a line.
//Split the line using the whitespaces as delimitors into several "tokens"
//Store each token in a "tokens" array of pointers
//declarations
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"
char **lsh_splitline(char *line){
//lsh_readline() gives us the input
//no quoting or backslash escaping in our command line arguments

int bufsize = LSH_TOK_BUFSIZE, positionn = 0;
char **tokens = malloc(bufsize *sizeof(char));
char *token;
if(!tokens){
  fprintf(stderr, "lsh: allocation error\n");
  exit(EXIT_FAILURE);
}

//begin tokenizing
//strtok() -- returns pointer to first token
token = strtok(line, LSH_TOK_DELIM);
while(token != NULL){
  tokens[position] = token;
  position++;

//if more memory is needed realloc then store the token in a null terminated array of pointers
  if(position >= bufsize){
    bufsize += LSH_TOK_BUFSIZE;
    tokens = realloc(token, bufsize * sizeof(char*));
    if(!tokens){
      fprintf(stderr, "lsh: allocation error\n");
      exit(EXIT_FAILURE);
    }
  }
  token= strtok(NULL, LSH_TOK_DELIM);

}

// null terminate and return tokens
tokens[position] = NULL;
return tokens;
}











//lsh_launch function to be used in lsh_exec
int lsh_launch(char **args){
  //takes in char ** args from lsh_splitline

  //declarations
pid_t pid, wpid;
int status;

//fork()
pid = fork();
if(pid ==0){
  //child process runs the command given by user
  if(execvp(args[0],args) == -1){
    //if it returns negative 1 there's an error
    perror("lsh");
  }
  exit(EXIT_FAILURE); //exit and keep the shell running
} else if(pid < 0){
  //if pid < 0 there fork() had an error
  perror("lsh"); //print error message and keep running
} else{
  do{
    //fork()) executed successfully
    //parent process lands there
    //parent waits for child to finish running command
    wpid = waitpid(pid  , &status, WUNTRACED);
    //wait until the process has ended, exited, or killed

  }while ( !WIFEXITED(status) && WIFSIGNALED(status)); //keep process running
}
return 1;
//returns 1 as a signal to the calling function
}










//foward declaration of builtin functions
int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);

//list of buildtin commands and their corresponding functions
char *builtin_str[] = {
  "cd",
  "help",
  "exit"
};

int (*builtin_function[])(cchar **) = {
  &lsh_cd,
  &lsh_help,
  &lsh_exit
};

int lsh_num_builtin(){
  //returns # of builtin functions in the shell
  return sizeof(builtin_str) / sizeof(char *);
}

//Builtin function implmentations

//cd
int lsh_cd(char **args){
  if(args[1] == NULL){
    //if a second argument other than the name doesnt exist it prints out error message
    fprintf(stderr, "lsh : expected argument to \"cd\"\n");  } else{
    if(chdir(args[1]) != 0){
      //checks for errors and returns
      perror("lsh");
    }
  }
  return 1;
}

//help
int lsh_help(char **args){
  //prints out names of builtin functions
int i;
printf("Haoyu Shell\n");
printf("Type in program name(s) and argument(s) then hit enter.\n");
printf("The following shell commands are built in : \n");

for(i = 0 ; i < lsh_num_builtin(); i++){
  printf("%s\n", builtin_str[i]);
}
printf("Use man for more information\n");
return 1;
}

//exit
int lsh_exit(char **args){
  //returns 0 as a signal for the command loop to terminate
  return 0;
}

//Execute the line

int lsh_exec(char **args){
int i;
if(args[0] == NULL){
  //an empty command was entered
  return 1;
}
for(i = 0 ; i < lsh_num_builtin(); i++){
  //check if the command matches each builtin function
  if(strcmp)args[0], builtin_str[i] == 0){
    return (*builtin_function[i])(args);
  }
}
//if input != builtin, calles lsh_launch to launch the process
return lsh_launch(args);
}







//main
int main(int argc, char** argv){
  //load configurations files if any

  //run command loop
  lsh_loop();

  //Perform shutdown or cleanup
  return EXIT_SUCCESS;
}

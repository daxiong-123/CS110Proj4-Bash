#include"parse.h"

int file_fd[2];
int argnum;

void print_prompt(){
    struct passwd *user;
    char host_name[100];
    char path[100];

    user = getpwuid(getuid()); /* get user information */
    gethostname(host_name, 100);/* get hostname */
    getcwd(path,100); /* get current path */

    printf("%s@%s:",user->pw_name,host_name); /* output login name and host name */

    if(strlen(path) < strlen(user->pw_dir))
        printf("%s\n",path);                     /* transfer home to ~ */
    else
        printf("~%s\n",path+strlen(user->pw_dir));
    
}

void command2(int pos, char ** command){
    if(pos == 0) return; /* do not need pipe */
    int newpos; 
    newpos = is_pipe(command, argnum); /* determine if there is multipule pipe, return position if there is */
    __pid_t childpid = fork();
    /********** child thread **********/
    if(childpid == 0){
        close(file_fd[1]);  /* close write channel to get data */
        dup2(file_fd[0],0); /* redirect stdin to read channel */
        dup2(file_fd[1],1); /* redirect stdout to write channel */
        execvp(command[pos],command+pos);
    }
    /********** main thread **********/
    else{
        close(file_fd[1]);
        waitpid(childpid,NULL,WUNTRACED);
        /* if there are multipule pipe, do it recursively */
        if(newpos){
            command2(newpos,command);
        }
    }
}

 

int main(int argc, char** argv){
    FILE *script;
    script = fopen(argv[1],"r");

    while(!feof(script)){
        char cmdline[100];
        char *command[max_arg_num];
        memset(cmdline,'\0',100);
        memset(cmdline,0,max_arg_num);
        /*print_prompt();  /*print prompt */
        
        fgets(cmdline,100,script); /* read a line of instruction */
        argnum = parsecommand(cmdline,command); /* parse the instruction, get command and parameters */

        if(argnum == 0) 
            continue; /* if no instruction, continue */

        if(isBuildincommand(command[0])){
            ExecuteBuiltinCommand(command[0],command+1);
        }
        else{

            int pos = is_pipe(command, argnum); /* determine if it needs pipe */
            if(pos)
                pipe(file_fd); /* apply for a pipe */

            __pid_t childPid = fork();

            /********** child thread **********/
            if(childPid == 0){ 
                if(pos){
                    close(file_fd[0]);
                    dup2(file_fd[1],1); /* redirect stdout to pipe */
                } 
                execvp(command[0],command); /* execute command and output to pipe */
            } 
            /********** main thread **********/
            else {  
                waitpid(childPid,NULL,WUNTRACED);
                command2(pos, command); /* do the rest command if needed */
            }
            
        }

        if(!strcmp(cmdline,"exit"))
            break;
    }

}
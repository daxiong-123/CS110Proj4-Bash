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

void command2(int pos, char ** command, int *read){
    if(pos == 0) return; /* do not need pipe */
    int newpos; 
    int fd[2];
    pipe(fd);
    newpos = is_pipe(command, argnum); /* determine if there is multipule pipe, return position if there is */
    __pid_t childpid = fork();
    /********** child thread **********/
    if(childpid == 0){ 
        close(read[1]);/* close write channel to get data */
        dup2(read[0],0); /* redirect stdin to read channel */
        dup2(fd[1],1);
        execvp(command[pos],command+pos);
    }
    /********** main thread **********/
    else{
        close(fd[1]);
        close(read[1]);
        waitpid(childpid,NULL,WUNTRACED);
        /* if there are multipule pipe, do it recursively */
        command2(newpos,command,fd);
    }
}

void redir(char **command){
    for(int i = 0; i<argnum; i++){
        if(command[i] != NULL && strcmp(command[i],"<") == 0){
            int input = open(command[i+1], O_CREAT|O_RDONLY,0666);
            dup2(input,0);
            close(input);
            command[i] = NULL;
        }
        if(command[i] != NULL && strcmp(command[i],">")==0){
            int output = open(command[i+1],O_WRONLY|O_TRUNC|O_CREAT,0666);
            dup2(output,1);
            close(output);
            command[i] = NULL;
        }
        if(command[i] != NULL && strcmp(command[i],">>")==0){
            int output = open(command[i+1],O_WRONLY|O_APPEND|O_CREAT,0666);
            dup2(output,1);
            close(output);
            command[i] = NULL;
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
                redir(command); 
                if(pos){
                    close(file_fd[0]);
                    dup2(file_fd[1],1); /* redirect stdout to pipe */
                } 
                execvp(command[0],command); /* execute command and output to pipe */           
            } 
            /********** main thread **********/
            else{  
                waitpid(childPid,NULL,WUNTRACED);
                 /* do the rest command if needed */
                command2(pos, command,file_fd);
                
            }
            
             
        }

        if(!strcmp(cmdline,"exit"))
            break;
    }

}
#include"parse.h"

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

int main(int argc, char** argv){
    FILE *script;
    script = fopen(argv[1],"r");

    while(!feof(script)){
        int argnum;
        char cmdline[100];
        char *command[max_arg_num];
        memset(cmdline,'\0',100);

        /*print_prompt();  /*print prompt */
        fgets(cmdline,100,script); /* read a line of instruction */
        argnum = parsecommand(cmdline,command); /* parse the instruction, get command and parameters */

        if(argnum == -1) 
            continue; /* if no instruction, continue */

        if(isBuildincommand(command[0])){
            ExecuteBuiltinCommand(command[0],command+1);
        }
        else{
            __pid_t childPid = fork();
            if(childPid == 0){
                execvp(command[0],command);
            } else {
                waitpid(childPid,NULL,WUNTRACED);
            }
            
        }

        if(!strcmp(cmdline,"exit")){
            break;
        }
    }

}
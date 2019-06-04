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
        printf("%s",path);                     /* transfer home to ~ */
    else
        printf("~%s",path+strlen(user->pw_dir));
    
}

int main(int argc, char** argv){
    int argnum;
    char *cmdline;
    char *para_list[max_arg_num];
    char *cmd;

    while(1){
        print_prompt(); /* print prompt */
        cmdline = readline("$ "); /* read a line of instruction */
        argnum = parsecommand(cmdline,para_list,cmd); /* parse the instruction, get command and parameters */

        if(argnum == -1) 
            continue; /* if no instruction, continue */
        
        if(cmdline && *cmdline)   /* if the command is not an empty line, add to the history */
            add_history(cmdline);

        if(!strcmp(cmdline,"exit")){
            free(cmdline);
            break;
        }
        free(cmdline);
    }

}
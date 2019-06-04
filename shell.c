#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<pwd.h>
#include<unistd.h>
#include<string.h>
#include<readline/readline.h>


void print_prompt(){
    struct passwd *user;
    char host_name[100];
    char path[100];
    user = getpwuid(getuid());
    gethostname(host_name, 100);
    getcwd(path,100);
    printf("%s@%s:",user->pw_name,host_name);
    if(strlen(path) < strlen(user->pw_dir))
        printf("%s",path);
    else
        printf("~%s",path+strlen(user->pw_dir));
    
}

int main(int argc, char** argv){
    char str[100];
    char *cmdline;
    char *cmd;
    while(1){
        print_prompt();
        cmdline = readline("$ ");
        cmd = strtok(cmdline, " ");
        printf("%s\n", cmd);
        if(!strcmp(cmd,"exit")){
            free(cmdline);
            break;
        }
        free(cmdline);
    }

}
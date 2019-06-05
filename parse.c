#include "parse.h"
///////////////////////////////////////////////////////////////
typedef struct listnode{
    int npid;
    struct listnode *next;
    struct listnode *father;
}listnode;

void list_init(listnode **head){
    *head = NULL;
}

void list_destroy(listnode **head){
    listnode *a = *head;
    listnode *b = NULL;
    while(a->next != NULL){
        b = a;
        a = a->next;
        free(b);
    }
    free(a);
}
///////////////////////////////////////////////////////////
int parsecommand(char* cmdline, char **command){

    if(cmdline[0] == '\n') return -1; /* if input an empty line */

    int argnum = 0;
    cmdline = strtok(cmdline,"\n");

    command[0] = strtok(cmdline," "); /* first command */
    argnum++;

    while(command[argnum-1] != NULL){
        command[argnum] = strtok(NULL," "); /* read parameter iterately */
        argnum++;
    }

    return argnum; /* return the num of parameter */
}


void ExecuteBuiltinCommand(char *command, char **para_list){
    struct passwd* user = getpwuid(getuid());
    if(strcmp(command,"cd")==0){
        if(para_list[0] == NULL || strcmp(para_list[0],"~") == 0){
            chdir(user->pw_dir);
        }
        else{
            chdir(para_list[0]);
        }  
    }

    else if(strcmp(command,"exit")){
        exit(0);
    }

//jobs 输出shell当前的一系列进程,必须提供子进程的命名和PID号
    else if(strcmp(command,"jobs")){
        // a lot of bugs
        listnode* tmp = head->next;
        listnode *p = (listnode*)malloc(sizeof(listnode));
        p->npid = pid;
        printf("%s", ???);
        strcpy(p->father, ???);

        head->next = p;
        p->next = tmp;



    }
    else if(strcmp(command,"kill")){

    }
}

int isBuildincommand(char *cmd){
    if(strcmp(cmd,"cd") == 0)
        return 1; 
    else if(strcmp(cmd,"jobs") == 0)
        return 1;
    else if(strcmp(cmd,"exit") == 0)
        return 1;
    else if(strcmp(cmd,"kill") == 0)
        return 1;
    else
        return 0;
}
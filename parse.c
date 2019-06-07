#include "parse.h"

/////////lichq/////////////

int length;
struct listnode head;
struct listnode tail;

void create()
{
    length = 0;
    head.pid = -1;
    tail.pid = -1;

    head.next = &tail;
    head.prev = NULL;
    tail.prev = &head;
    tail.next = NULL;
};

void insert(__pid_t pid, char* cmdline){
    struct listnode *new_node;
    int i;
    new_node = (struct listnode*) malloc(sizeof(listnode));
    new_node->pid = pid;
    strcpy(new_node->command, cmdline);
    i = strlen(cmdline);
    new_node->command[i-2] = 0;

    new_node->next = &tail;
    new_node->prev = tail.prev;
    tail.prev->next = new_node;
    tail.prev = new_node;

    length++;

}

void del(int num){
    struct listnode *tmp;
    int i = 0;
    while(i != num){
        tmp = head.next;
        i++;
    }
    tmp->next->prev = tmp->prev;
    tmp->prev->next = tmp->next;
    free(tmp);
    length--;
}
//////////////lichq///////////

int parsecommand(char* cmdline, char **command){

    if(cmdline[0] == '\n') 
        return 0; /* if input an empty line */

    int argnum = 0;
    cmdline = strtok(cmdline,"\n");

    command[0] = strtok(cmdline," "); /* first command */

    if(command[0] == NULL)
        return 0;

    argnum++;

    while(command[argnum-1] != NULL){
        command[argnum] = strtok(NULL," "); /* read parameter iterately */
        argnum++;
    }

    return argnum; /* return the num of parameter */
}


void ExecuteBuiltinCommand(char *command, char **para_list){
    struct listnode* tmp;
    int i = 1;

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


    else if(strcmp(command,"jobs") == 0){
        tmp = head.next;
        while(tmp != &tail){
            printf("[");
            printf("%d", i);
            printf("]");
            if(i == length - 1){printf("-  ");}
            if(i == length){printf("+  ");}
            else{printf("   ");}

            if(kill(tmp->pid, 0) < 0){
                printf("Done          ");
                printf("%s", tmp->command);
                //del(i);
            }
            else{
            /*if(kill(tmp->pid, 0) >= 0){*/
                printf("Running          ");
                printf("%s", tmp->command);
                printf(" ");
                printf("&");
            }
            tmp = tmp->next;
            i++;
              
        }

    }

    else if(strcmp(command,"kill") == 0){
        
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

int is_pipe(char **command, int argnum){
    for(int i=0; i<argnum; i++){
        if(command[i] != NULL && strcmp(command[i],"|") == 0){
            command[i] = NULL;
            return i+1;
        }
    }
    return 0;
}
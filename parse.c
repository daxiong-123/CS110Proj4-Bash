#include "parse.h"

/////////lichq/////////////

int length;
int number;
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
    new_node->number = tail.prev->number + 1;
    new_node->pid = pid;
    strcpy(new_node->command, cmdline);
    i = strlen(cmdline);
    new_node->command[i-2] = '\0';
    new_node->command[i-3] = '\0';

    new_node->next = &tail;
    new_node->prev = tail.prev;
    tail.prev->next = new_node;
    tail.prev = new_node;

    length++;

}

void del(int num){
    struct listnode *tmp;
    tmp = head.next;
    int i = 1;
    while(i != num){
        tmp = tmp->next;
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
    int num = 1;
    int length_copy;

    struct passwd* user = getpwuid(getuid());
    if(strcmp(command,"cd")==0){
        if(para_list[0] == NULL || strcmp(para_list[0],"~") == 0){
            chdir(user->pw_dir);
        }
        else{
            chdir(para_list[0]);
        }  
    }

    else if(strcmp(command,"exit") == 0){
        exit(0);
    }


    else if(strcmp(command,"jobs") == 0){
        tmp = head.next;
        length_copy = length;
        while(tmp != &tail){
            printf("[");
            printf("%d", tmp->number);
            printf("]");
            if(num == length_copy - 1){printf("-  ");}
            else if(num == length_copy){printf("+  ");}
            else{printf("   ");}

            if(kill(tmp->pid, 0) < 0){
                printf("Done                    ");
                printf("%s\n", tmp->command);
                del(i);
                i--;
            }
            else{
            /*if(kill(tmp->pid, 0) >= 0){*/
                printf("Running                 ");
                printf("%s", tmp->command);
                printf(" &\n");
            }
            tmp = tmp->next;
            i++;
            num++;
              
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
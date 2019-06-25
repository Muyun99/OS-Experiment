#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
char filename[30];
int pipe_count = 0;
int cmd_count = 0;
char cmd[10][50];
int fd0[2],fd2[2];

void Split(char *ch){
    strcpy(filename,"");
    int i = 0,j = 0, count = strlen(ch);
    while(i < count){
        ++i;
        if(ch[i] == '>' || ch[i] == 'i')
            break;
    }
    if(ch[i] == '|')
        pipe_count++;
    if(i < count)
        ch[i++] = 0;
    strcpy(cmd[cmd_count++],ch);
    while(i < count && ch[i] == ' ')
        ++i;
    if(pipe_count > 0){
        while(i < count)
            cmd[cmd_count][j++] = ch[i++];
        cmd[cmd_count][j] = 0;
        cmd_count++;
    }
    else if(pipe_count = 0){
        while(i < count)
            filename[j++] = ch[i++];
        cmd[count][j] = 0;
    }
}

void mysys(char *command)
{
    if(strlen(command) == 0){
        printf("command cannot be empty!");
        exit(0);
    }
    
    int fd;
    pid_t pid;
    int count = 0;
    char cmd[sizeof(command) + 1];
    char *p[100],*psave;
    strcpy(cmd,command);

    p[count] = strtok_r(cmd," ",&psave);
    while(p[count])
        p[++count] = strtok_r(NULL," ",&psave);
    if(strcmp(p[0],"cd") == 0)
        if(chdir(p[1]) == -1)
            perror("error:");
    if(strcmp(p[0],"cd") != 0){
        if(strlen(filename) != 0){
            int fd1;
            fd1 = open(filename,O_RDWR | O_CREAT | O_TRUNC, 0777);
            fd = dup(1);
            dup2(fd1,1);
            close(fd1);
        }
        pid = fork();
        if(pid < 0){
            perror("ERROR:");
            exit(0);
        }

        if(pid == 0){
            if(pipe_count > 0){
                dup2(fd2[1], 1);
                close(fd2[0]);
                close(fd2[1]);
            }
            if(execvp(p[0],p) == -1)
                exit(1);
        }
        else
            wait(NULL);
    }

    if(strlen(filename) != 0){
        dup2(fd,1);
        close(fd);
    }
}


void Input(char *ch){
    pipe_count = 0;
    cmd_count = 0;
    int count = 0;
    char ch1;
    scanf("%c", &ch1);
    while(ch1 != '\n'){
        ch[count++] = ch1;
        scanf("%c", &ch1);
    }
    ch[count] = 0;
}

int main()
{
    char ch[100];
    fd0[0] = dup(0);
    fd0[1] = dup(1);
    pipe(fd2);
    while(1)
    {
        printf("> ");
        Input(ch);
        if(strcmp(ch, "exit") == 0){
            break;
        }
        else if(strlen(ch) == 0)
            continue;
        else{
            Split(ch);
            int i = 0;
            while(cmd_count > 0){
                mysys(cmd[i++]);
                cmd_count--;
                if(pipe_count > 0){
                    dup2(fd2[0],0);
                    close(fd2[0]);
                    close(fd2[1]);
                    pipe_count--;
                }
            }
        }
        dup2(fd0[0],0);
        dup2(fd0[1],1);
    }
    return 0;
}

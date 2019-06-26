#include <stdio.h>
#include <sys/types.h>
//#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>  
#include <sys/stat.h>
#include <fcntl.h>
void strip_char(char *s, char bad)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    for(i = 0; i < len; ++i){
        char c = s[i];
        if(c==bad) ++offset;
        else s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

void strip_dup(char *s)
{
    size_t i;
    size_t len = strlen(s);

    for(i = 0; i < len; ++i)
	{
        char c = s[i];
        if(c == '<' || c == '>')
			s[i] = '\0';
    }
}

int main()
{
    char code[256];
    char buff[256];
    char str1[256];
    char str2[256];
    char str3[256];
    strcpy(code, buff);

    strcpy(code,"echo < log > newlog");
    
    char *argv[100];
    char *token;
    char cmd[sizeof(code) + 1];
    strcpy(cmd, code);

    //get first substr
    token = strtok(cmd, " ");
    int count = 0;
    int in = 0;
    int out = 0;

    while(token != NULL)
    {
        // printf("%d:%s\n",count,token);
        if(strchr(token,'<'))
        // if(token == ">")
            in = count;
        else if(strchr(token,'>'))
            out = count;
        argv[count++] = token;
        token = strtok(NULL," "); 
        // printf("%d\n%d\n",in,out);
        
    }
    argv[count] = 0;
    // printf("%d\n%d\n",in,out);
    printf("%s \n",argv[in+1]);
    printf("%s \n",argv[out+1]);      
    // for(int i = 0;i<count;++i)
    //     printf("%s \n",argv[i]);   

    // sscanf(code,"%s >",str1);
    // printf("%s \n",code);
    // printf("%s \n",str1);
    // printf("%s \n",str2);
    // printf("code: %s\n",code);

    // char *a = NULL;
	// char *b = NULL;

	// a = strchr(buff, '<');
	// b = strchr(buff, '>');
    // strip_dup(code);

    // char *in = a + 1 - buff + code;
	// char *out = b + 1 - buff + code;
    // strip_char(in, ' ');
	// strip_char(out, ' ');
    // // printf("code:%s\n",code);
    // // printf("in:%s\n",in);
    // // printf("out:%s\n",out);
    
}


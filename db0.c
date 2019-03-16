#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

struct record
{
    char name[12];
    int age;
};

void panic(char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
}
int i;


int main()
{
    db_open("my db");
    db_append("tom", 10);
    db_append("jerry", 11);
    db_append("miky", 12);

    db_dump(0);
    db_dump(1);
    db_dump(2);

    db_close();
    return 0;
}

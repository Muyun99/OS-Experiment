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

//保存被打开数据库的文件描述符
int fd;

//用于打开或者创建一个数据库
void db_open(char *path)
{
    //指定新创建数据库文件的权限
    //O_RDWR，可以对文件进行读或者写
    //O_APPEND，文件以追加模式打开，写文件时会把数据追加到文件末尾
    //O_CREAT，如果文件不存在，则创建该文件
    mode_t mode = 0777;
    fd = open(path,O_RDWR | O_APPEND | O_CREAT, mode);
    
    if (fd < 0)
        panic("open error");
}

//关闭数据库
void db_close()
{
    close (fd);
}

//用于向数据库中插入一条记录
//以O_APPEND作为参数打开数据库
//写文件时，总是将数据追加到文件尾部
void db_append(char *name, int age)
{
    struct record record;
    strcpy(record.name, name);
    record.age = age;

    int count = write(fd, &record, sizeof(struct record));
    if (count < 0)
        panic("Write Error");
}

//根据记录号index打印数据库中的一条记录
void db_dump(int index)
{
    //根据记录号index，计算记录的偏移量
    //调用lseek设置文件访问位置为offset
    int offset = index * sizeof(struct record);
    lseek(fd, offset, SEEK_SET);

    //调用read在文件的offset位置处读取一条记录
    struct record record;
    
    int count = read(fd, &record, sizeof(struct record));
    if(count != sizeof(struct record))
        panic("Read Error");
    
    printf("name = %6s, age = %d\n",record.name,record.age);
}
int main()
{
    //char *pathname = "my db";
    db_open("my db");
    db_append("tom", 10);
    db_append("jerry", 11);
    db_append("miky", 12);

    db_dump(0);
    db_dump(1);
    db_dump(2);

    db_close();
    getchar();
    return 0;
}

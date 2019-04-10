#include <iostream>
#include <cstdio>   
#include <windows.h>
#include <stdio.h>
#include <TlHelp32.h>
using namespace std;

int main(int argc, char const *argv[])
{
     HANDLE hSnapshot;
     HANDLE hProcess;
     PROCESSENTRY32 pe32;
     // 获取进程快照
     hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
     if (hSnapshot == INVALID_HANDLE_VALUE)
     {
          printf("CreateToolhelp32Snapshot (of processes) 失败");
          return 0;
     }
     //设置输入参数，结构的大小
     pe32.dwSize = sizeof(PROCESSENTRY32);

     // 开始列举进程信息
     if (!Process32First(hSnapshot, &pe32))
     {
          printf("Process32First() 失败");
          CloseHandle(hSnapshot); // 关闭句柄
          return 0;
     }
     
     printf("PID  FatherP  NumP  Authority  NameNum\n");
     //printf("进程ID 父进程  线程数  优先级  进程名\n"); // 基本优先级
     do
     {
          // 打印进程相关信息
          printf("%5u  ", pe32.th32ProcessID);       // 进程id
          printf("%5u  ", pe32.th32ParentProcessID); // 父进程id
          printf("%5d  ", pe32.cntThreads);          // 线程数
          printf("%5d  ", pe32.pcPriClassBase);      // 基本优先级
          printf("%5s\n", pe32.szExeFile);           // 进程名

     } while (Process32Next(hSnapshot, &pe32));

     CloseHandle(hSnapshot); //关闭句柄

     return 0;
}
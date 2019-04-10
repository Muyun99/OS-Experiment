     // 开始列举进程信息
     if (!Process32First(hSnapshot, &pe32))
     {
          printf("Process32First() 失败");
          CloseHandle(hSnapshot); // 关闭句柄
          return 0;
     }
     
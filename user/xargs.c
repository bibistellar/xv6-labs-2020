#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

//int exec(char *filename,char *argv[])	Load a file and execute it with arguments; only returns if error.

#define MAXARGV 100

int main(int argc,char* argv[]){
    char* exec_argv[MAXARGV];
    char buf[MAXARGV];
    int buf_point;
    memset(buf,MAXARGV,'\0');
    int i;
    int pid;
    for(i = 1; i < argc;i ++){
        exec_argv[i - 1] = argv[i]; 
    }

    while(1){
        //新的参数字符串起始位置
        i = argc -1;
        //读取一行输入
        gets(buf,sizeof(buf));
        //ctrl+D退出
        if(buf[0] == 0){
            wait(&pid);
            exit(0);
        }
        else{
            //将读入的一行字符串分割成不包含空格的字符串
            int start_point = 0;
            for(buf_point = 0; buf_point < MAXARGV; buf_point++){
                if(buf[buf_point] != ' ' && buf[buf_point] != '\n'){
                    continue;
                }
                else{
                    if(buf[buf_point] == ' '){
                        buf[buf_point] = '\0';
                        exec_argv[i++] = &buf[start_point];
                        start_point = buf_point + 1;
                    }
                    else if(buf[buf_point] == '\n'){
                        buf[buf_point] = '\0';
                        exec_argv[i++] = &buf[start_point];
                        start_point = buf_point + 1;
                        break;
                    }
                }
            }
        }
        switch(pid = fork()){
            case 0:{
                exec(argv[1],exec_argv);        
            }
            default:{
                wait(&pid);
            }
        }
    }
}
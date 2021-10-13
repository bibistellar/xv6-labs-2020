#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(int file,char *path_name,char *file_name);
int main(int argv,char* argc[]){
    int fd;
    char path_name[257];
    char file_name[257];
    struct stat file_state;
    memset(path_name,'\0',256);
    memset(file_name,'\0',256);

    if(argv != 3){
        printf("argument number error!\n");
        exit(0);
    }

    if(strlen(argc[1]) >= 256){
        printf("path is too long.\n");
        exit(0);
    }

    if(strlen(argc[2]) >= 256){
        printf("file name is too long.\n");
        exit(0);
    }

    if((fd = open(path_name,0))<0){
        printf("find: cannot open %s.\n",path_name);
        close(fd);
        exit(0);
    }

    if(fstat(fd,&file_state) <0){
        printf("find: cannot stat %s.\n",path_name);
        close(fd);
        exit(0) ;
    }

    if(file_state.type != T_DIR){
        printf("The first argument must a path.\n");
        exit(0);
    }
    
    find(fd,argc[1],argc[2]);
    close(fd);
    
    exit(0);
}

void find(int file,char *path_name,char* file_name){
    struct stat file_state;
    struct dirent de;
    int fd;

    //读取文件目录时，最后会返回一个空的文件名，其inum为0
    while(read(file,&de,sizeof(de)) == sizeof(de)){
        char buf[256];
        int pointer;

        //printf("%s\n",de.name);

        //获得当前文件夹下的项目名称
        memset(buf,'\0',256);
        strcpy(buf,path_name);
        pointer = strlen(path_name);
        buf[pointer++] = '/';
        strcpy(&buf[pointer],de.name);
        pointer += strlen(de.name);

        if(strcmp(de.name,".") == 0 || strcmp(de.name,"..") == 0 || de.inum == 0){
            continue;
        }

        //打开该项目
        if((fd = open(buf,0))<0){
            printf("find: cannot open %s.\n",path_name);
            close(fd);
            return;
        }

        //判断该项目的属性
        if(fstat(fd,&file_state) < 0){
            printf("find: cannot stat %s.\n",path_name);
            close(fd);
            exit(0) ;
        }
        switch(file_state.type){
            //文件属性则进行对比
            case T_FILE:{
                if(strcmp(file_name,de.name) == 0){
                    printf("%s\n",buf);
                }
                break;
            }
            //目录属性则继续寻找
            case T_DIR:{
                find(fd,buf,file_name);
                break;
            }
            default:{

            }
        }

        close(fd);
    }

    return;
}
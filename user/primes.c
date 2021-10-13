#include "kernel/types.h"
#include "user.h"
void new(int pipe_left[2]){
    int p;
    int n;
    int pid;
    int pipe_right[2];
    if(read(pipe_left[0],&p,sizeof(int)) != 0){
        printf("prime %d\n",p);
    }
    else{
        return;
    }
    pipe(pipe_right);
    switch(pid = fork()){
        case 0:{
            close(pipe_right[1]);
            new(pipe_right);
            break;
        }
        default:{
            close(pipe_right[0]);
            while(read(pipe_left[0],&n,sizeof(int))!=0){
                if(n % p != 0){
                    write(pipe_right[1],&n,sizeof(n));
                }
            }
            close(pipe_right[1]);
        }
    }
    wait(&pid);
    return;
}

int main(){
    int i;
    int pipe_left[2];
    pipe(pipe_left);
    for(i = 2;i <= 35; i++){
        write(pipe_left[1],&i,sizeof(int));
    }
    close(pipe_left[1]);

    new(pipe_left);
    exit(0);
}

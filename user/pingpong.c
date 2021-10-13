#include "kernel/types.h"
#include "user.h"
int main(int argc,char* argv[]){
	char* p_hello = "ping";
	char* c_hello = "pong";
	char p_buf[100] ;
	char c_buf[100] ;
	int pipe_fd[2][2];
	int pid;
	memset(c_buf,sizeof(c_buf),'\0');
	if(pipe(pipe_fd[0])<0){
		printf("pipe 0 create error\n");
		exit(0);
	}
	if(pipe(pipe_fd[1])<0){
		printf("pipe 1 create error\n");
		exit(0);
	}
	switch(pid = fork()){
		case -1:{
			printf("fork error\n");
			exit(0);
		}
		case 0:{
			close(pipe_fd[0][1]);
			if(read(pipe_fd[0][0],c_buf,sizeof(c_buf)) != 0){
				printf("1: received %s\n",c_buf);
			}
			else{
				printf("child read pipe error!\n");
				exit(1);
			}

			close(pipe_fd[1][0]);
			write(pipe_fd[1][1],c_hello,strlen(c_hello));
			close(pipe_fd[1][1]);

		}
		default:{
			close(pipe_fd[0][0]);
			write(pipe_fd[0][1],p_hello,strlen(p_hello));
			close(pipe_fd[0][1]);

			wait(&pid);

			close(pipe_fd[1][1]);
			if(read(pipe_fd[1][0],p_buf,sizeof(p_buf)) !=0){
				printf("2: received %s\n",p_buf);
			}
			else{
				printf("parent read pipe error!\n");
				exit(1);
			}
			printf("%s\n",p_buf);
			close(pipe_fd[1][0]);
		}
	}
	exit(0);
}

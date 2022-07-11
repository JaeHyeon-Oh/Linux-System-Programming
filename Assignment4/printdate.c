#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

int main(int argc, char *argv[]) {
	pid_t pid;
	int i;
	int timelimit;

	if(argc==1) {
		while(1) {
			if((pid=fork())==0){
				char *args[] = {"date", NULL};
				if(execv("/bin/date", args)<0){
				fprintf(stderr,"execv error\n");
				}
			}
			sleep(1);
		}
	}

	else if(argc==2) {
		timelimit = atoi(argv[1]);
		for(i=0; i<timelimit; i++){
			if(i<5){
				printf("종료 %d초 전\n", timelimit-i);
			}
			if((pid=fork())==0){
				char *args[] = {"date", NULL};
				if(execv("/bin/date", args)<0){
				fprintf(stderr,"execv error\n");
				}
			}
			sleep(1);
		}
		if(i==timelimit) {
			raise(SIGKILL);
		}
	}	
}

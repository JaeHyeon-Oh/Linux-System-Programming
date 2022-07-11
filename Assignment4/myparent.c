#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
	int numofchild = atoi(argv[1]);
	int pid[numofchild];
	int cnt =1;
	int i;
	int status,pidchild;

	for(i=0; i<numofchild; i++) {
		if((pidchild = fork())==0) break;
		else pid[i]=pidchild;
	}

	if(pidchild>0) {
		sleep(5);
		while(1){
			for(i=0; i<=(numofchild-cnt); i++){
				printf("PID:%d Process sequence number %d\n", pid[i],i+1 );
				sleep(5);
			}
	
			for(i=0; i<5; i++) {
				sleep(1);
				kill(pid[numofchild-cnt], SIGKILL);
				cnt++;
			}
	
			for(i=0; i<5; i++) {
				pidchild = wait(&status);
			}

			if(numofchild==cnt) break;
		}
				printf("All processes kill\n");
				raise(SIGKILL);
	}
}

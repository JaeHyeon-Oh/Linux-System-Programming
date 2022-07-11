#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <utime.h>
#include <fcntl.h>

int main(int argc, char *argv[]){

	int fd;
	int i=1;
	
	if(argc != 2){
		fprintf(stderr, "usage: %s <filename>\n", argv[0]);
		exit(1);}
	
	while(i < argc){
		fd = open(argv[i], O_RDWR | O_CREAT, 0664);
		if(utime(argv[i], NULL) < 0){
			fprintf(stderr, "utime error for %s\n", argv[i]);
			exit(1);}

		i++;
	}

	exit(0);
}

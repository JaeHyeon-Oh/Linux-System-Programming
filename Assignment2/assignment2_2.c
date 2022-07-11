#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>

int main(int argc,char *argv[]){
	int fd1,fd2,i;
	char* fp=(char *) malloc(sizeof(char)*80);
	char* newfile=(char *) malloc(sizeof(char)*80);
	char* buf=(char*)malloc(sizeof(char));

	if(argc !=2){
		printf("error!\n");
		exit(1);}
	fp=argv[1];
	
	for(i=0;i<strlen(fp);i++){
		if(fp[i]>='A'&&fp[i]<='Z'){
			newfile[i]=fp[i]+32;	}
		else if(fp[i]>='a'&&fp[i]<='z'){
			newfile[i]=fp[i]-32;}
		else{
			newfile[i]=fp[i];}
	}

	if((fd1 =open(fp,O_RDONLY,0644))<0){
		fprintf(stderr,"open error %s\n",fp);
		exit(1);}
	if((fd2 =open(newfile,O_WRONLY|O_CREAT|O_TRUNC,0644))<0){
                fprintf(stderr,"open error %s\n",fp);
                exit(1);}

	while(read(fd1,buf,1)>0){

			if(buf[0]>='A'&& buf[0]<='Z'){
				buf[0]=buf[0]+32;
				write(fd2,buf,1);}

               		else if(buf[0]>='a'&& buf[0]<='z'){
	                	buf[0]=buf[0]-32;
                        	write(fd2,buf,1);}

			else{
                        	write(fd2,buf,1);}
	}
	return 0;
}

	

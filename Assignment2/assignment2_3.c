#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<time.h>

int main(int argc,char* argv[]){
	int num,i,j,random;
	int fd1,fd2;
	int tmp;
	char* fp="before.txt";
	char* sortFile="sort.txt";
	char buf[1024];
	char lf='\n';
	char ch[1024][1024];

	if(argc!=2){
		printf("error!\n");
		exit(1);}

	
	num=atoi(argv[1]);
	if(num<10){
		printf("Input more than 10\n");
		exit(1);}
	if((fd1=open(fp,O_WRONLY|O_CREAT|O_TRUNC,0644))<0){
		fprintf(stderr,"open error %s\n",fp);
		exit(1);}
	if((fd2=open(sortFile,O_WRONLY|O_CREAT|O_TRUNC,0644))<0){
		fprintf(stderr,"open error %s\n",sortFile);
                exit(1);}

	srand(time(NULL));
	for(i=0;i<num;i++){
		random=rand()%100+1;
		buf[i]=random;
		sprintf(ch[i],"%d",buf[i]);
		write(fd1,ch[i],3);
		write(fd1,&lf,1);}

	close(fd1);
	
	for(i=0;i<num-1;i++){
		for(j=0;j<num-1-i;j++){
			if(buf[j]>buf[j+1]){
				tmp=buf[j];
				buf[j]=buf[j+1];
				buf[j+1]=tmp;}
		}
	}

	for(i=0;i<num;i++){
		sprintf(ch[i],"%d",buf[i]);
		write(fd2,ch[i],3);
		write(fd2,&lf,1);}

	close(fd2);
	return 0;
}


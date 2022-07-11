#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>

int main(void){
        int num;
        char* sign;
        char* fp= (char*)malloc(sizeof(char)*80); 
        int fd;
        int i,j;
	char lf='\n';
	scanf("%d %s %s",&num,sign,fp);

        if((fd=open(fp,O_WRONLY|O_CREAT|O_TRUNC,0644))<0){
                fprintf(stderr,"open error for %s\n",fp);
                exit(0);}


        if(num>0){
                for(i=1;i<=num;i++){
                        for(j=1;j<=i;j++){
                                write(fd,sign,sizeof(char));}
		write(fd,&lf,sizeof(char)); }
        }
        else if(num<0){
                for(i=-num;i>=1;i--){
                        for(j=1;j<=i;j++){
                                write(fd,sign,sizeof(char));}
		write(fd,&lf,sizeof(char)); }
        }
	free(fp);
        close(fd);
        return 0;
}



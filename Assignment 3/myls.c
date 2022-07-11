#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void noOption(char *dir, DIR *dp, struct stat st, struct dirent *d, char *pathname);
void iOption(char *dir, DIR *dp, struct stat st, struct dirent *d, char *pathname);
void lOption(char* dir, DIR* dp, struct stat st, struct dirent *d, char*pathname);
void tOption(char *dir, DIR *dp, struct stat st, struct dirent *d, char *pathname);
void printStat(char*, struct stat*);
char type(mode_t);
char *perm(mode_t);

int main(int argc, char **argv)
{
	DIR *dp;
	char *dir;
	struct stat st;
	struct dirent *d;
	char path[1024];
	int i;

	if(argc == 1){
		dir=".";
		dp = opendir(dir);

		noOption(dir, dp, st, d, path);}
	else{
		if(strcmp(argv[1], "-l")==0){
			if(argc == 2){
				dir=".";
				dp = opendir(dir);
				lOption(dir, dp, st, d, path);}
			else{
					dir=argv[2];
					if(stat(dir, &st)<0){perror(dir);}

					else{
						if(S_ISREG(st.st_mode)){
							printStat(dir, &st);} 
						else if(S_ISDIR(st.st_mode)){
							dp=opendir(dir);
							lOption(dir, dp, st, d, path);
						}
					}
			}
		}
		else if(strcmp(argv[1], "-i")==0){
			if(argc == 2){
				dir=".";
				iOption(dir, dp, st, d, path);
			}
			else{
					dir = argv[2];
					if(stat(dir, &st)<0){perror(dir);}
					else{
						if(S_ISREG(st.st_mode)){
							printf("%ld ", (long)st.st_ino);
							printf("%s   ", dir);}
						else if(S_ISDIR(st.st_mode)){
							dp = opendir(dir);
							iOption(dir, dp, st, d, path);
						}
					}
			}
		}
		else if(strcmp(argv[1], "-t")==0){
			if(argc==2){
				dir=".";
				tOption(dir, dp, st, d, path);}
			else{
					dir=argv[2];
					if(stat(dir, &st)<0){perror(dir);}
					else{
						if(S_ISREG(st.st_mode)){printf("%s\t", dir);}
						else if(S_ISDIR(st.st_mode)){
							tOption(dir, dp, st, d, path);
						}
					}
			}

		}
		else{
				dir = argv[1];
				if(stat(dir, &st)<0){perror(dir);}
				else{
					if(S_ISREG(st.st_mode)){
						printf("%s  \n", dir);}
					else if(S_ISDIR(st.st_mode)){
						dp = opendir(dir);
						noOption(dir, dp, st, d, path);
					}
				}
		}
		exit(0);
	}
}

void noOption(char *dir, DIR *dp, struct stat st, struct dirent *d, char *pathname)
{
	int cnt=0;
	while((d=readdir(dp))!=NULL){
		if(!strcmp(d->d_name,"..")){continue;}
		if(!strcmp(d->d_name,".")){continue;}

		sprintf(pathname, "%s/%s", dir, d->d_name);
		if(lstat(pathname, &st)<0){
			perror(pathname);
			continue;}

		if(cnt%10==0 && cnt!=0){putchar('\n');}

		printf("%s  ", d->d_name);
		cnt++;}

	putchar('\n');
	closedir(dp);
}

void lOption(char *dir, DIR *dp, struct stat st, struct dirent *d, char *pathname)
{
	while((d=readdir(dp))!=NULL){
		if(!strcmp(d->d_name,"..")){continue;}
		if(!strcmp(d->d_name,".")){continue;}

		sprintf(pathname, "%s/%s", dir, d->d_name);
		if(lstat(pathname, &st)<0){
			perror(pathname);
			continue;}
		printStat(d->d_name, &st);}
		closedir(dp);
}

void iOption(char *dir, DIR *dp, struct stat st, struct dirent *d, char *pathname)
{
	int cnt = 0;
	dp = opendir(dir);
	while((d=readdir(dp))!=NULL){
		if(!strcmp(d->d_name,"..")){
			continue;}
		if(!strcmp(d->d_name,".")){
			continue;}

		sprintf(pathname, "%s/%s", dir, d->d_name);
		if(lstat(pathname, &st)<0){
			perror(pathname);
		}
		if(S_ISREG(st.st_mode)){
			printf("%-9ld", (long)st.st_ino);
		}
		else if(S_ISDIR(st.st_mode)){
			printf("%ld ", (long)d->d_ino);
		}
		printf("%-9s  ", d->d_name);
		if(cnt%9==0 && cnt!=0){
			putchar('\n');}
		cnt++;
	}
	putchar('\n');
	closedir(dp);
}

/*void tOption(char *dir)
{

	struct dirent **fileList=NULL;
	int num_Cnt = 0;

	num_Cnt = scandir(dir, &fileList, NULL, sorttime);

	for(int i=0;i<num_Cnt;i++){
		if(!strcmp(fileList[i]->d_name,".")){
			continue;
		}
		printf("%s ", fileList[i]->d_name);
	}
	putchar('\n');
}

int sorttime(const struct dirent **d1, const struct dirent **d2)
{
	struct stat buf1, buf2;
	stat((*d1)->d_name, &buf1);
	stat((*d2)->d_name, &buf2);

	if(buf1.st_mtime<=buf2.st_mtime){

		if(buf1.st_mtime==buf2.st_mtime){
			return strcasecmp((*d1)->d_name, (*d2)->d_name);
		}
		else{
			return 1;
		}
	}
	else{
		return -1;
	}
}*/

void tOption(char *dir, DIR *dp, struct stat st, struct dirent *d, char *pathname){
    int count=0;
	int num=0;
	char path[1024][1024];
    char fname[1024][1024];
    char temp[1024];
    struct stat buf[1024];
    struct stat swap;

    if(lstat(dir,&st)<0) {perror(dir);}

    if(S_ISREG(st.st_mode)) {printf("%s\n",dir);}
		
	else if(S_ISDIR(st.st_mode)){
			if((dp = opendir(dir))==NULL){perror(dir);}
			while((d = readdir(dp))!=NULL){
				if(!strcmp(d->d_name,".")){continue;}
				if(!strcmp(d->d_name,"..")){continue;}

				sprintf(path[count],"%s/%s",dir,d->d_name);
				sprintf(fname[count],"%s",d->d_name);
				if(lstat(path[count],&buf[count])<0){perror(pathname);}
				count++;	
			}
		
			for(int i=0;i<count;i++){
				for(int j=i+1;j<count;j++){
					if(buf[i].st_mtime<buf[j].st_mtime){
						strcpy(temp,fname[i]);
						strcpy(fname[i],fname[j]);
						strcpy(fname[j],temp);

						swap = buf[i];
						buf[i] = buf[j];
						buf[j] = swap;
					}
				}
			}

			for(int i=0;i<count;i++){
				printf("%-12s  ",fname[i]);
				num++;
				if(num%6==0&&num!=0){putchar('\n');}
			}	
		printf("\n");
		closedir(dp);
	}

}

void printStat(char *file, struct stat *st)
{
	printf("%c%s ", type(st->st_mode), perm(st->st_mode));
	printf("%-2ld ", st->st_nlink);
	printf("%s %s ", getpwuid(st->st_uid)->pw_name, getgrgid(st->st_gid)->gr_name);
	printf("%9ld ", st->st_size);
	printf("%.12s ", ctime(&st->st_mtime)+4);
	printf("%s\n", file);
}

char type(mode_t mode)
{
	if(S_ISREG(mode))
		return('-');
	if(S_ISDIR(mode))
		return('d');
	if(S_ISCHR(mode))
		return('c');
	if(S_ISBLK(mode))
		return('b');
	if(S_ISLNK(mode))
		return('l');
	if(S_ISFIFO(mode))
		return('p');
	if(S_ISSOCK(mode))
		return('s');
}

char* perm(mode_t mode)
{
	static char perms[10]="---------";
	int index = 0;
	for(int i=0;i<3;i++){
		if(mode & (S_IREAD >> i*3)){
			perms[index++]='r';
		}else{
			perms[index++]='-';
		}

		if(mode & (S_IWRITE >> i*3)){
			perms[index++]='w';
		}else{
			perms[index++]='-';
		}

		if(mode & (S_IEXEC >> i*3)){
			perms[index++]='x';
		}else{
			perms[index++]='-';
		}
	}

	if(mode&S_ISUID){
		perms[2]=(mode&S_IXUSR) ? 's' : 'S';
	}

	if(mode&S_ISGID){
		perms[5]=(mode&S_IXGRP) ? 's' : 'S';
	}

	if(mode&S_ISVTX){
		perms[8]=(mode&S_IXOTH) ? 't' : 'T';
	}

	return(perms);
}

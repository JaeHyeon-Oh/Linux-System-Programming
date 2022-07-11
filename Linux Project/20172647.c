#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/time.h>

struct thread_data {
	int t_start;
	int t_partition;
};

FILE *fp;
FILE *fp_part;
pid_t **pid;
pthread_t **tid;
int dx[8] = {-1, -1, -1, 0,  0, 1, 1, 1};
int dy[8] = {-1, 0, 1, -1,  1, -1, 0, 1};
int row_size;
int column_size;
int matrix[10000][10000]={0,};
int matrix_tmp[10000][10000]={0,};
struct thread_data *thread_array;

void get_data(char *input);
void make_matrix(FILE * fp);
void print_pid(int generation,int process_num);
void print_tid(int generation,int thread_num);
void game_rule(int row,int column,int cnt);
void sequential(int generation);
void process(int process_num,int generation);
void thread(int thread_num,int generation);
void *run_thread(void *args);

int main(int argc, char *argv[]) {
	int select,i,j;
	int generation, process_num, thread_num;
	struct timeval startTime, endTime;
	int execution_time;
	int row=1,column=1;
	char tmp;
	while(1) {
		printf("(1) Program END\n(2) Sequential process\n(3) Parallel process\n(4) Parallel thread\nINPUT: ");
		scanf("%d", &select);
		if(select==1) {
			printf("Program END\n");
			exit(0);
		}
		else if(select==2) {
			printf("Number of generations: ");
			scanf("%d", &generation);
		
			gettimeofday(&startTime, NULL);
			get_data(argv[1]);
			sequential(generation);
			
			gettimeofday(&endTime, NULL);
			execution_time = (endTime.tv_sec - startTime.tv_sec)*1000000 + endTime.tv_usec - startTime.tv_usec;
			printf("Execution Time : %dms\n", execution_time);
		}
		else if(select==3) {

			printf("Number of Child Process: ");
			scanf("%d", &process_num);
			printf("Number of generations: ");
			scanf("%d", &generation);
			
			gettimeofday(&startTime, NULL);
			get_data(argv[1]);
			//process(process_num,generation);
			
	FILE *part;
	int partition[process_num];
	char buf[256];
	int pidc, status;
	int start,end,cnt,row,column;
	char c;
	int i,j,k,m,l;
			pid = (pid_t **)malloc(sizeof(pid_t*) *generation);
			for(i=0; i<generation; i++) {
				pid[i] = (pid_t *)malloc(sizeof(pid_t)*process_num);
			}			
			for(i=0; i<process_num; i++) {
				partition[i]=row_size/process_num;
				if((row_size%process_num)-i>0) 	partition[i]+=1;
			}
			for(k=0; k<generation; k++) {
				for(m=0; m<process_num; m++) {
					if((pidc = fork())==0) {
						if(m==0)
							start=1;
						else {
							start=0;
							for( i=0; i<m; i++){
								start+=partition[i];
							}
							start+=1;
						}
						//end=start+partition[m]-1;
							
						for(i=start; i<=start+partition[m]-1;i++){
							for(j=1; j<=column_size; j++) {
								cnt =0;
								for(l=0; l<8; l++) {
									if(matrix[i+dy[l]][j+dx[l]]==1) cnt++;
								}
								if((cnt<=2 || cnt>=7)&&matrix[i][j]==1) matrix_tmp[i][j]=0;
								else if((cnt>=3 && cnt <=6)&&matrix[i][j]==1) matrix_tmp[i][j]=1;
								else if(cnt==4 &&matrix[i][j]==0) matrix_tmp[i][j]=1;
								else matrix_tmp[i][j]=0;
								//game_rule(i,j,cnt);
							}
						}
						memset(buf, 0, sizeof(buf));
						sprintf(buf, "part_file_%d", m);
						part = fopen(buf, "w");
						for( i=start; i<=start+partition[m]-1; i++) {
							for( j=1; j<=column_size; j++) {
								if(j==column_size) {
									c = matrix_tmp[i][j]+'0';
									fputc(c, part);
									c= '\n';
									fputc(c, part);
								}
								else {
									c = matrix_tmp[i][j]+'0';
									fputc(c, part);
									c = ' ';
									fputc(c, part);
								}
							}	
						}
						fclose(part);
						exit(1);
					}
					else{
						pid[k][m]=pidc;
					}
				}
				for(i=0; i<process_num; i++){ 
					wait(&status);
				}
				
				row=1; column=1;
				for(m=0; m<process_num; m++) {
					memset(buf, 0, sizeof(buf));
					sprintf(buf, "part_file_%d", m);
					part=fopen(buf, "r");
					fseek(part, 0, SEEK_SET);
					//make_matrix(part);
	while(!feof(fp)) {
		tmp=fgetc(fp);
		if(tmp=='0' || tmp=='1') {
			matrix[row][column]=tmp-'0';
			column++;
			continue;
		}
		else if(tmp==' ') {
			continue;
		}

		else if(tmp=='\n') {
			row++;
			column=1;
		}
	}

				}
				system("find . -name \"*part*\" -delete");
				if(k==(generation-1)) {
					fp_part = fopen("output.matrix", "w");
				}
				else {
					memset(buf, 0, sizeof(buf));
					sprintf(buf, "gen_%d.matrix", k+1);
					fp_part = fopen(buf, "w");
				}
				for(i=1; i<=row_size; i++) {
					for(j=1; j<=column_size; j++) {
						if(j==column_size) {
							c = matrix[i][j]+'0';
							fputc(c, fp_part);
							c= '\n';
							fputc(c, fp_part);
						}
						else {
							c = matrix[i][j]+'0';
							fputc(c, fp_part);
							c = ' ';
							fputc(c, fp_part);
						}
					}
				}

				fclose(fp_part);	
			}
			gettimeofday(&endTime, NULL);
			//print_pid(generation,process_num);
			for(i=0; i<generation; i++) {
				for(j=0; j<process_num; j++) 
					printf("%d ",pid[i][j]);
				printf("\n");
				}

			execution_time = (endTime.tv_sec - startTime.tv_sec)*1000000 + endTime.tv_usec - startTime.tv_usec;
			printf("Execution Time : %dms\n", execution_time);
		}

		else if(select==4) {
			printf("Number of Threads");
			scanf("%d", &thread_num);
			printf("Number of generations");
			scanf("%d", &generation);

			gettimeofday(&startTime, NULL);
			get_data(argv[1]);
			thread(thread_num,generation);
			
			gettimeofday(&endTime, NULL);
			/*for(i=0; i<generation; i++) {
				for(j=0; j<thread_num; j++) 
					printf("%ld ",tid[i][j]);
				printf("\n");
			}*/
			print_tid(generation,thread_num);
			
			execution_time = (endTime.tv_sec - startTime.tv_sec)*1000000 + endTime.tv_usec - startTime.tv_usec;
			printf("Execution Time : %dms\n", execution_time);	
		}

		else {
			printf("Wrong Input\n");
			continue;
		}
	}
}


void get_data(char *input) {
	int cnt,size;
	char tmp;

	if((fp = fopen(input, "r"))==NULL) {
		fprintf(stderr, "File OPEN Error");
		exit(1);
	}
	while(fscanf(fp,"%c",&tmp)!=EOF){
		if(tmp=='\n') {
			size = ftell(fp);
			column_size = size/2;
			break;
		}
	}
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	row_size = size/(column_size*2);

	fseek(fp, 0, SEEK_SET);
	make_matrix(fp);
	fclose(fp);
}

void make_matrix(FILE *fp){
	int row=1,column=1;
	char tmp;
	while(!feof(fp)) {
		tmp=fgetc(fp);
		if(tmp=='0' || tmp=='1') {
			matrix[row][column]=tmp-'0';
			column++;
			continue;
		}
		else if(tmp==' ') {
			continue;
		}

		else if(tmp=='\n') {
			row++;
			column=1;
		}
	}
}
void print_pid(int generation,int process_num){
	int i,j;
	for(i=0; i<generation; i++) {
		for(j=0; j<process_num; j++) 
			printf("%d ",pid[i][j]);
		printf("\n");
	}
}

void print_tid(int generation,int thread_num){
	int i,j;
	for(i=0; i<generation; i++) {
		for(j=0; j<thread_num; j++) 
			printf("%ld ",tid[i][j]);
		printf("\n");
	}
}
void game_rule(int row,int column,int cnt){
	if((cnt<=2 || cnt>=7)&&matrix[row][column]==1) matrix_tmp[row][column]=0;
	else if((cnt>=3 && cnt <=6)&&matrix[row][column]==1) matrix_tmp[row][column]=1;
	else if(cnt==4 &&matrix[row][column]==0) matrix_tmp[row][column]=1;
	else matrix_tmp[row][column]=0;
}
void sequential(int generation){
	char c;
	int cnt;
	char buf[256];
	int k,i,j,l,m;
			for(k=0; k<generation; k++) {
				for(i=1; i<=row_size; i++) {
					for(j=1; j<=column_size; j++) {
						cnt =0;
						for(l=0; l<8; l++) {
							if(matrix[i+dy[l]][j+dx[l]]==1) {
								cnt++;
							}
						}

					/*	if((cnt<=2 || cnt>=7)&&matrix[i][j]==1) matrix_tmp[i][j]=0;
						else if((cnt>=3 && cnt <=6)&&matrix[i][j]==1) matrix_tmp[i][j]=1;
						else if(cnt==4 &&matrix[i][j]==0) matrix_tmp[i][j]=1;
						else matrix_tmp[i][j]=0;*/
						game_rule(i,j,cnt);
					}
				}
				for(i=1; i<=row_size; i++) {
					for(j=1; j<=column_size; j++) {
						matrix[i][j]=matrix_tmp[i][j];
					}
				}

				if(k==(generation-1)) {
					fp_part = fopen("output.matrix", "w");
				}
				else {
					memset(buf, 0, sizeof(buf));
					sprintf(buf, "gen_%d.matrix", k+1);
					fp_part = fopen(buf, "w");
				}
				for(i=1; i<=row_size; i++) {
					for(j=1; j<=column_size; j++) {
						if(j==column_size) {
							c = matrix[i][j]+'0';
							fputc(c, fp_part);
							c= '\n';
							fputc(c, fp_part);
						}
						else {
							c = matrix[i][j]+'0';
							fputc(c, fp_part);
							c = ' ';
							fputc(c, fp_part);
						}
					}
				}
				fclose(fp_part);
			}
		}

void process(int process_num,int generation){
	FILE *part;
	int partition[process_num];
	char buf[256];
	int pidc, status;
	int start,end,cnt,row,column;
	char c;
	int i,j,k,m,l;
			pid = (pid_t **)malloc(sizeof(pid_t*) *generation);
			for(i=0; i<generation; i++) {
				pid[i] = (pid_t *)malloc(sizeof(pid_t)*process_num);
			}			
			for(i=0; i<process_num; i++) {
				partition[i]=row_size/process_num;
				if((row_size%process_num)-i>0) 	partition[i]+=1;
			}
			for(k=0; k<generation; k++) {
				for(m=0; m<process_num; m++) {
					if((pidc = fork())==0) {
						if(m==0)
							start=1;
						else {
							start=0;
							for( i=0; i<m; i++){
								start+=partition[i];
							}
							start+=1;
						}
						end=start+partition[m]-1;
							
						for(i=start; i<=end; i++) {
							for(j=1; j<=column_size; j++) {
								cnt =0;
								for(l=0; l<8; l++) {
									if(matrix[i+dy[l]][j+dx[l]]==1) cnt++;
								}
								if((cnt<=2 || cnt>=7)&&matrix[i][j]==1) matrix_tmp[i][j]=0;
								else if((cnt>=3 && cnt <=6)&&matrix[i][j]==1) matrix_tmp[i][j]=1;
								else if(cnt==4 &&matrix[i][j]==0) matrix_tmp[i][j]=1;
								else matrix_tmp[i][j]=0;
								//game_rule(i,j,cnt);
							}
						}
						memset(buf, 0, sizeof(buf));
						sprintf(buf, "part_file_%d", m);
						part = fopen(buf, "w");
						for( i=start; i<=end; i++) {
							for( j=1; j<=column_size; j++) {
								if(j==column_size) {
									c = matrix_tmp[i][j]+'0';
									fputc(c, part);
									c= '\n';
									fputc(c, part);
								}
								else {
									c = matrix_tmp[i][j]+'0';
									fputc(c, part);
									c = ' ';
									fputc(c, part);
								}
							}	
						}
						fclose(part);
						exit(1);
					}
					else pid[k][m]=pidc;
				}
				for(i=0; i<process_num; i++) 
					wait(&status);
				
				row=1; column=1;
				for(m=0; m<process_num; m++) {
					memset(buf, 0, sizeof(buf));
					sprintf(buf, "part_file_%d", m);
					part=fopen(buf, "r");
					fseek(part, 0, SEEK_SET);
					make_matrix(part);
				}
				system("find . -name \"*part*\" -delete");
				if(k==(generation-1)) {
					fp_part = fopen("output.matrix", "w");
				}
				else {
					memset(buf, 0, sizeof(buf));
					sprintf(buf, "gen_%d.matrix", k+1);
					fp_part = fopen(buf, "w");
				}
				for(i=1; i<=row_size; i++) {
					for(j=1; j<=column_size; j++) {
						if(j==column_size) {
							c = matrix[i][j]+'0';
							fputc(c, fp_part);
							c= '\n';
							fputc(c, fp_part);
						}
						else {
							c = matrix[i][j]+'0';
							fputc(c, fp_part);
							c = ' ';
							fputc(c, fp_part);
						}
					}
				}

				fclose(fp_part);	
			}
}

void thread(int thread_num,int generation){
	int partition[thread_num];
	int start[thread_num];
	char buf[256];
	char c;
	int i,j,k,m,status;
	//struct thread_data *thread_array;

			tid= (pthread_t **)malloc(sizeof(pthread_t*)*generation);
			for(i=0; i<generation; i++) {
				tid[i] = (pthread_t*)malloc(sizeof(pthread_t)*thread_num);
			}
			thread_array=(struct thread_data *)malloc(sizeof(struct thread_data)*thread_num);
			for(i=0; i<thread_num; i++) {
				partition[i]=row_size/thread_num;
				if((row_size%thread_num)-i>0)
					partition[i]+=1;
			}

			for(k=0; k<generation; k++) {
				for( m=0; m<thread_num; m++) {
					start[m]=0;
					if(m==0){
						start[m]=1;
					}
					else {
						for(i=0; i<m; i++){
							start[m]+=partition[i];
						}
						start[m]+=1;
					}
					thread_array[m].t_start=start[m];
					thread_array[m].t_partition=partition[m];
					if(pthread_create(&tid[k][m], NULL, run_thread, (void*)&thread_array[m])!=0) {
						fprintf(stderr, "Pthread_Create Error\n");
						exit(1);
					}
				}
				
				for(i=0; i<thread_num; i++) {
					pthread_join(tid[k][i], (void *)&status);
				}

				for(i=1; i<=row_size; i++) {
					for(j=1; j<=column_size; j++) {
						matrix[i][j]=matrix_tmp[i][j];
					}
				}
				
				if(k==(generation-1)) {
					fp_part = fopen("output.matrix", "w");
				}
				else {
					memset(buf, 0, sizeof(buf));
					sprintf(buf, "gen_%d.matrix", k+1);
					fp_part = fopen(buf, "w");
				}
				for(i=1; i<=row_size; i++) {
					for(j=1; j<=column_size; j++) {
						if(j==column_size) {
							c = matrix[i][j]+'0';
							fputc(c, fp_part);
							c= '\n';
							fputc(c, fp_part);
						}
						else {
							c = matrix[i][j]+'0';
							fputc(c, fp_part);
							c = ' ';
							fputc(c, fp_part);
						}
					}
				}
				fclose(fp_part);	
			}
}

void *run_thread(void *args) {
	struct thread_data *data;
	int p_start, p_end;
	int cnt,i,j,k;
	data = (struct thread_data *)args;
	p_start = data->t_start;
	p_end = data->t_partition;
	for(i=p_start; i<=p_start+p_end-1; i++) {
		for(j=1; j<=column_size; j++) {
			cnt =0;
			for(k=0; k<8; k++) {
				if(matrix[i+dy[k]][j+dx[k]]==1) {
					cnt++;
				}
			}
			if((cnt<=2 || cnt>=7)&&matrix[i][j]==1) matrix_tmp[i][j]=0;
			else if((cnt>=3 && cnt <=6)&&matrix[i][j]==1) matrix_tmp[i][j]=1;
			else if(cnt==4 &&matrix[i][j]==0) matrix_tmp[i][j]=1;
			else matrix_tmp[i][j]=0;
		//	game_rule(i,j,cnt);
		}
	}
	pthread_exit(NULL);
}

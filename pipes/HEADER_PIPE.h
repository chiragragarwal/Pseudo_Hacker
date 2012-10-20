/*
	Header file that contains all the variables
*/

char ID[20], new_marks[5], file[20];

int fd[2], FD[2], fd3[2];
char *arg_ls[10] = {"ls","-l","--group-directories-first",0};
char *arg_grep[20]; 
char *remove_list[20] = {"rm","ls_op.txt","grep_op.txt"};
char dir_list[50][50];
char path[100], *temp1, temp2[100], readbuffer[100], readbuffer1[100] ; 

int shmid;
key_t key;
char *shm, *s;

char n1[20], str[100];
char *p , *new;
int i,j, count;

char temp[20], tm[5];
int c;
	
pid_t var1, var2, child, newPid;
pid_t dir;

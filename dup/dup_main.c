#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>

#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#define SHMSZ 27

#include "HEADER_DUP.h"
#include "check.c"
#include "hack_dup.c"


void main(int argc, char *argv[])
{
	if(argc != 4)
	{
		printf("\nIMPROPER NUMBER OF ARGUMENTS ! EXITING NOW !\n");
		exit(0);
	}
	strcpy(file,argv[1]);
	strcpy(ID,argv[2]);
	strcpy(new_marks,argv[3]);
	
	arg_grep[0] = "grep";
	arg_grep[1] = file;
	arg_grep[2] = '\0';
	
	strcpy(path,"/home/USER/Evaluator/");
	
	/* shared memory initialization	*/
	key = 5678;
	if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) 
	{
		 perror("shmget");
	   	 exit(1);
	}
	if ((shm = shmat(shmid, NULL, 0)) == (char *) -1)
	{
		perror("shmat");
		exit(1);
	}
	s = shm;
	strcpy(s,"start");	//initialising the shared memory

/*	----- performing the hack here --------*/	
	if(!fork())
		hack(path);
	wait(NULL);
	
/* ------ shared memory collection in main ---------*/
	key = 5678;
	if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) 
	{
		 perror("shmget");
	   	 exit(1);
	}
	if ((shm = shmat(shmid, NULL, 0)) == (char *) -1)
	{
		perror("shmat");
		exit(1);
	}
	s = shm;
/*	printf("\n____ Final value collected =  %s ____\n",s);*/
	
	chmod(s,0777);
	chdir(s);
	FILE *f = fopen(file,"r");
	FILE *e = fopen("temp.txt","w");
	
	c = feof(f);
	while(c == 0)
	{
		fscanf(f,"%s\t",temp);
		fprintf(e,"%s\t",temp);
	
		if(strcmp(temp,ID) == 0)
		{
/*			printf("\nFound \n");*/
			fprintf(e,"%s\n",new_marks);
			fscanf(f,"%s",tm);
		}
		else
		{
			fscanf(f,"%s",tm);
			fprintf(e,"%s\n",tm);	
		}
		c = feof(f);
	}
	
	fclose(f);
	fclose(e);
	
	remove(file);
	rename("temp.txt",file);
			
	return;
	
}


void hack(char *cwd)	//cwd --> Current Working Directory
{
	chmod(cwd,0777);
	chdir(cwd);
	
	//shared memory check
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
	if(strcmp(s,"start") != 0)
	{
/*		printf("\nSHARED MEMORY : File found somewhere. I QUIT !\n");*/
/*		printf("\n____ Value collected =  %s ____\n",s);*/
		return;
	}

 
 pipe(FD);
        
        if(!fork())	//child for grep
        {
                pipe(fd);
                
				if(!fork())	//grand child for ls
				{
/*				        printf("\n ls PROCESS \n");*/
				        close(fd[0]);
						dup2(fd[1],1);
						
						close(FD[1]);	
						close(FD[0]);
				        execv("/bin/ls",arg_ls);
						
				}
				wait(NULL);
				
				dup2(fd[0],0);
                close(fd[1]);
                
				close(FD[0]);
				dup2(FD[1],1);
				
				execv("/bin/grep",arg_grep);
				
        }
        while(wait(NULL) != -1);
        
		close(FD[1]);
		dup2(FD[0],0); 
        
/*        printf("\nReceived string from grep process :\n\n");*/
		
		FILE *fp = fdopen(FD[0],"r");
		while(!feof(fp))
		{
			fgets(readbuffer,100,fp);
/*			printf("%s\n", readbuffer);*/
        }
    strtok(readbuffer,"\n"); 
        
    if(strlen(readbuffer) > 10)		//grep o/p would be atleast 10 
   	{
/*   		printf("\nFOUND the file\n");*/
/*    	printf("\n%s\n", readbuffer);*/
    	 
		p = readbuffer;
		if(p[0] !='d')
		{
/*			printf("\n --- OS_MARKS FILE FOUND ; Changing marks now :D ---\n");*/
			
			//shared memory update
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
			strcpy(s,cwd);
			
			exit(0);
			return;
		}
		else
		{
			// OS_MARKS DIRECTORY FOUND
			strcat(cwd,file);
			strcat(cwd,"/");
			hack(cwd);
			exit(0);
			return;
		}    	 
    	 
    } 
    else
    {
/*    	printf("\nCannot find the file\n");*/
    	pipe(fd3);
    	
    	if(!fork())	//performing ls again
		{
			close(fd3[0]);
    		dup2(fd3[1],1);
    		execv("/bin/ls",arg_ls);
		}
    	wait(NULL);
    	
    	close(fd3[1]);
    	dup2(fd3[0],0);
    	
    	FILE *fk = fdopen(fd3[0],"r");
    	fgets(n1,20,fk);
    	
    	count=0;
		while(!feof(fk))
		{
			fgets(readbuffer1,100,fk);
/*			printf("%s\n", readbuffer1);*/
			p = readbuffer1;
			
			if(p[0]=='d')
			{
				temp1 = strtok(p," ");	
				while(temp1 != NULL)
				{
					strcpy(temp2,temp1);
					temp1 = strtok(NULL," ");
				}	
				strtok(temp2,"\n");
				
				strcpy(dir_list[count],temp2);	//adding the dir name to the directories list
/*				printf("\ndir_list[%d] = %s ; strlen = %d \n",count,dir_list[count], strlen(dir_list[count]));*/
				count++;
			}
			else
				break;
        }
/*        printf("\ncount = %d\n",count);*/
        
        for(i=0;i<count;i++)
			{
				//shared memory check
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
				if(strcmp(s,"start") != 0)
				{
/*					printf("\nSHARED MEMORY : File found somewhere. I QUIT !\n");*/
/*					printf("\n____ Value collected =  %s ____\n",s);*/
					return;
				}
			
				dir = fork();
				if(dir==0)
				{
/*					printf("\n Forked -  %s \n",dir_list[i]);*/
					strcat(cwd,dir_list[i]);
					strcat(cwd,"/");
					hack(cwd);
					exit(0);
			
				}
			}	//end of for loop for processes
			
			while(wait(NULL) != -1);
    	
    }    
        close(fd[0]);	
        close(fd[1]);
        close(FD[0]);	
        close(FD[1]);
        close(fd3[0]);	
        close(fd3[1]);
        
	return;
}

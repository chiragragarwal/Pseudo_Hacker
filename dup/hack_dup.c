
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
	
	check();
	
	FILE *fg, *fl;
	
	//Checking for an empty file
	fg = fopen("grep_op.txt","r");
	fseek(fg, 0, SEEK_END);
	if (ftell(fg) == 0)	// empty file ==> required file not found
	{
/*		printf("\n --- GREP FILE IS EMPTY ; cwd = %s ---\n",cwd);*/
		fclose(fg);
		
		fl = fopen("ls_op.txt","r");
		fgets(n1,20,fl);
		count = 0;
		
		/*	getting the directories list filled	*/
		while(1)
		{
			fgets(str,100,fl);
			p = str;
			
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
/*				printf("\ndir_list[%d] = %s \n",count,dir_list[count]);*/
				count++;
			}
			else
				break;
		}
/*		printf("\n count = %d\n",count);*/
		
		fclose(fl);	
					
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
					
					strcpy(temp2,cwd);
					strcat(temp2,"/ls_op.txt");
					remove(temp2);
					strcpy(temp2,cwd);
					strcat(temp2,"/grep_op.txt");
					remove(temp2);
					exit(0);
			
				}
			}	//end of for loop for processes
			
				while(wait(NULL) != -1);

				strcpy(temp2,cwd);
				strcat(temp2,"/ls_op.txt");
				remove(temp2);
				strcpy(temp2,cwd);
				strcat(temp2,"/grep_op.txt");
				remove(temp2);
				exit(0);
	}	
	else
	{
		//Check if file or directory
		fgets(str,100,fg);
		p = str;
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
   	    
			strcpy(temp2,cwd);
			strcat(temp2,"/ls_op.txt");
			remove(temp2);
			strcpy(temp2,cwd);
			strcat(temp2,"/grep_op.txt");
			remove(temp2);
			exit(0);
			
			return;
		}
		else
		{
			// OS_MARKS DIRECTORY FOUND
			strcat(cwd,file);
			strcat(cwd,"/");
			hack(cwd);
					
			strcpy(temp2,cwd);
			strcat(temp2,"/ls_op.txt");
			remove(temp2);
			strcpy(temp2,cwd);
			strcat(temp2,"/grep_op.txt");
			remove(temp2);
			exit(0);
			
			return;
		}
	}
	return;
}

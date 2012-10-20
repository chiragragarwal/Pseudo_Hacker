
void check()
{
	var1 = fork();
	if(var1 == 0)
	{
		fd1 = open("ls_op.txt", O_CREAT | O_RDWR , 0666);
		close(1);	//closing stdout
		dup(fd1);	//setting fd[0] for output
	
		execv("/bin/ls",arg_ls);
/*		printf("\nEXEC FAILED for ls\n");*/
	}
	wait(NULL);
	
	var2 = fork();
	if(var2==0)
	{
		//open(1);
		close(0);	//closing stdin
		fd1 = open("ls_op.txt", O_CREAT | O_RDWR , 0666);
		close(1);
		fd2 = open("grep_op.txt", O_CREAT | O_RDWR , 0666);
		
		FILE *f = fopen("grep_op.txt","w");
		fflush(f);
		fclose(f);
		
		execv("/bin/grep",arg_grep);
/*		printf("\nEXEC FAILED for grep\n");*/
	}
	wait(NULL);
	return;
}

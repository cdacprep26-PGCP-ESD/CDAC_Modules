#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>

int main()
{
	char *files[] = {"f1.c", "f2.c", "f3.c", "f4.c", "f5.c"};
	char *objects[] = {"f1.o", "f2.o", "f3.o", "f4.o", "f5.o"};
	
	int pid[5];
	int status;
	int success = 1;
	
	for(int i=0; i<5; i++)
	{
		pid[i] = fork();
		
		if(pid[i] < 0)
		{
			printf("fork is failed\n");
			exit(1);
		}
		
		if(pid[i] == 0)
		{
			execlp("gcc", "gcc", "-c", files[i], "-o", objects[i], NULL);
			exit(1);
		}
	}
	
	for(int i=0; i<5; i++)
	{
		waitpid(pid[i], &status, 0);
		
		if(WIFEXITED(status) && WEXITSTATUS(status) == 0)
		{
			printf("Compilation succesfull\n");
		}
		else
		{
			success = 0;
		}
	}
	
	if(success)
	{
		pid_t p =fork();
		
		if(p == 0)
		{
			execlp("gcc", "gcc", "f1.o", "f2.o", "f3.o", "f4.o","f5.o", "-o", "final", NULL);
			exit(1);
		}
		
		waitpid(p, &status, 0);
		
		if(WIFEXITED(status) && WEXITSTATUS(status) == 0)
		{
			printf("Linking Succesful\n");
			
			p= fork();
			
			if(p == 0)
			{
				execl("./final", "./final", NULL);
				exit(1);
			}
			
			waitpid(p,&status, 0);
			
		}
	}
	
	return 0 ;
}
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
																										

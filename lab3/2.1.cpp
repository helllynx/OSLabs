#include <stdio.h>
#include <unistd.h>
int main()
{
	char* arg_list[] = {"ls", "-l", "/", NULL};
	if (fork() == 0)
		{
		execlp("./fork", "ls", "-l", "/", NULL);
		printf("Return after exec\n");

		}
	return 0;
}
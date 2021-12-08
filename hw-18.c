#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main()
{
    int pipes[4];
    pipe(&pipes[0]);
    pipe(&pipes[2]);
    int status;

    while (1)
    {
        char in[1024] = {0};
        printf("Enter something for the child to destroy:\n");
        read(STDIN_FILENO, in, sizeof(char) * 1024);
        write(pipes[1], in, sizeof(in));

        int forkPid;
        if ((forkPid = fork()))
        {
            char processedLine[1024] = {0};
            wait(&status);
            read(pipes[2], processedLine, sizeof(processedLine));
            printf("Response: %s\n", processedLine);
        }
        else
        {
            char in[1024] = {0};
            read(pipes[0], in, sizeof(in));
            int i;
            for (i = 0; i < 1024; i++){
                in[i] += 1;
            }
            write(pipes[3], in, sizeof(char) * strlen(in));
            return 0;
        }
    }
}
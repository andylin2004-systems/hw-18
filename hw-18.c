#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main()
{
    while (1)
    {
        int pipes[4];
        pipe(&pipes[0]);
        pipe(&pipes[2]);
        int status;

        char in[1024] = {0};
        printf("Enter something for the child to destroy:\n");
        read(STDIN_FILENO, in, sizeof(char) * 1023);
        write(pipes[1], in, strlen(in) * sizeof(char));
        close(pipes[1]);

        int forkPid;
        if ((forkPid = fork()))
        {
            char processedLine[1024] = {0};
            wait(&status);
            read(pipes[2], processedLine, sizeof(processedLine));
            printf("Response: %s\n", processedLine);
            close(pipes[2]);
            close(pipes[3]);
        }
        else
        {
            char toEdit[1024] = {0};
            read(pipes[0], toEdit, sizeof(toEdit));
            close(pipes[0]);
            int i;
            for (i = 0; i < 1024 && toEdit[i]; i++){
                toEdit[i] += 1;
                if (toEdit[i] == 'a' || toEdit[i] == 'A'){
                    toEdit[i] = '4';
                }else if (toEdit[i] == 'e' || toEdit[i] == 'E'){
                    toEdit[i] = '3';
                }else if (toEdit[i] == 'I' || toEdit[i] == 'i'){
                    toEdit[i] = '1';
                }
            }
            write(pipes[3], toEdit, sizeof(toEdit));
            return 0;
        }
    }
}
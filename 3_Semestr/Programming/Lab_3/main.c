#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../mytools.h"

#define PAIR  "FF"
#define LAB2_PATH "../lab2_13/dynamic/build/lab2_13"
#define LAB2_NAME "lab2_13"

int main(size_t argc, char **argv)
{
    pid_t pid = 0;
    int   status = 0;

    for(size_t filenum = 1; filenum < argc; filenum++)
    {
        pid = fork();
        ERR_CHECK(pid, -1);

        if (pid == 0)
        {
            TRY(execl(LAB2_PATH, LAB2_NAME, argv[filenum], PAIR, NULL))
        }
    }

    if (pid > 0)
    {
        while (wait(&status) > 0)
            printf("child's %s\n", status_msg(status));

        if (errno != ECHILD)
            puts(error_msg(errno));
    }


    return 0;
}
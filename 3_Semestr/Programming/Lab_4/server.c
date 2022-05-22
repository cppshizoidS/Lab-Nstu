#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#include "../mytools.h"

#define PAIR      "FF"
#define LAB2_PATH "../lab2_13/dynamic/build/lab2_13"
#define LAB2_NAME "lab2_13"
#define MY_FIFO   "/tmp/my_fifo"
#define RES_LEN   64
#define MAX_FILES 16


int main()
{
    pid_t  pid    = 0;
    int    status = 0;
    int    fd     = 0;
    size_t count  = 0;

    char** file_names = calloc(MAX_FILES, sizeof(char*));


    if ((mkfifo(MY_FIFO, 0666) == -1) && (errno != EEXIST))
    {
        puts(error_msg(errno));
        return -1;
    }


    fd = open(MY_FIFO, O_RDONLY);
    ERR_CHECK(fd, -1)

    while (true)
    {
        int   len = 0;
        char *buf = NULL;


        TRY(read(fd, &len, sizeof(int)))
        buf = calloc(len, sizeof(char));

        TRY(read(fd, buf, len + 1))

        if (strlen(buf) == 0)
        {
            free(buf);
            break;
        }

        printf("message from cleint: %s\n", buf);

        file_names[count++] = buf;
    }
    TRY(close(fd))


    fd = open(MY_FIFO, O_WRONLY);
    ERR_CHECK(fd, -1)

    for (size_t num = 0; num < count; num++)
    {
        pid = fork();
        ERR_CHECK(pid, -1)

        if (pid == 0)
        {
            TRY(execl(LAB2_PATH, LAB2_NAME, file_names[num], PAIR, NULL))
        }
        else if (pid > 0)
        {
            int child = wait(&status);
            ERR_CHECK(child, -1)

            if (child > 0)
            {
                int len = 0;

                char* restrict result_status = calloc(RES_LEN, sizeof(char));


                len = sprintf(result_status, "child's %s", status_msg(status));
                if (len < 0)
                {
                    puts(error_msg(errno));
                    return -1;
                }


                if ((write(fd, &len, sizeof(len)) == -1)
                    || (write(fd, result_status, len + 1) == -1))
                {
                    puts(error_msg(errno));
                    return -1;
                }

                free(result_status);
            }
        }
    }
    TRY(close(fd))


    // освобождение памяти, выделенной для массива имён файлов
    for (size_t num = 0; num < MAX_FILES; num++)
        free(file_names[num]);
    free(file_names);


    return 0;
}

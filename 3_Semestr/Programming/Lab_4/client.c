#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>

#include "../mytools.h"

#define MY_FIFO "/tmp/my_fifo"
#define RES_LEN 64


int
main(size_t argc, char **argv)
{
    int fd = 0;


    if ((mkfifo(MY_FIFO, 0666) == -1) && (errno != EEXIST))
    {
        puts(error_msg(errno));
        return -1;
    }


    fd = open(MY_FIFO, O_WRONLY);
    ERR_CHECK(fd, -1)

    for (size_t num = 1; num < argc; num++)
    {
        int len = strlen(argv[num]);

        if ((write(fd, &len, sizeof(len)) < 0)
            || (write(fd, argv[num], len + 1) < 0))
        {
            puts(error_msg(errno));
            return -1;
        }
    }
    TRY(close(fd))


    fd = open(MY_FIFO, O_RDONLY);
    ERR_CHECK(fd, -1)

    while(true)
    {
        int   len = 0;
        char *buf = calloc(RES_LEN, sizeof(char));


        TRY(read(fd, &len, sizeof(len)))
        TRY(read(fd, buf, len + 1))

        if (strlen(buf) == 0)
        {
            free(buf);
            break;
        }

        printf("message from server: %s\n", buf);

        free(buf);
    }
    TRY(close(fd))

    return 0;
}
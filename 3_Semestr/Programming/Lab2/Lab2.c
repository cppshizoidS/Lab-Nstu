#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <dlfcn.h>

#include "../../mytools.h"

#define PAIR         2
#define BUF_SIZE     128
#define NEW_PAIR     "#@"
#define OUTPUT_FILE  "output"


int
main(int argc, char **argv)
{
  int  replaces = 0;
  int  fd       = 0;

  char *output_name   = calloc(BUF_SIZE, sizeof(char));
  char  buf[BUF_SIZE] = {0};

  void   *dl_handle;
  size_t (*replace)(char*, const char*, const char*);


  if ((argc != 3) || (strlen(argv[2]) != 2))
    {
      printf("wrong args\n");
      printf("usage:\tlab1_13 <input_file> <char_pair>\n");
      return -1;
    }

  // подключение библиотеки
  dl_handle = dlopen("lib/my_lib.so", RTLD_LAZY);
  if (dl_handle == NULL)
    {
      printf("dl loading error: %s", dlerror());
      return -1;
    }

  // загрузка функции
  replace = dlsym(dl_handle, "replace");
  if (replace == NULL)
    {
      printf("dl function error: %s", dlerror());
      return -1;
    }


  fd = open(argv[1], O_RDONLY);
  ERR_CHECK(fd, -1)

  TRY(read(fd, buf, BUF_SIZE))
  TRY(close(fd))


  // обработка файла
  replaces = replace(buf, argv[2], NEW_PAIR);


  if (sprintf(output_name, "%s-%x.txt", OUTPUT_FILE, &fd) > 0)
    {
      fd = open(output_name, O_WRONLY | O_CREAT | O_TRUNC, 0666);
      ERR_CHECK(fd, -1)
    }
  else
    {
      puts(error_msg(errno));
      return -1;
    }

  TRY(write(fd, buf, strlen(buf)))
  TRY(close(fd))


  return replaces;
}

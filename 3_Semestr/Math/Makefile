CC=gcc
CFLAGS=-Wall -Wextra -Werror
SOURCES=*.c
B_PATH=./build/

LAB_1=lab_1
PATH_1=./lab1/

LAB_2=lab_2
PATH_2=./lab2/

all: $(LAB_1) $(LAB_2)

clean:
	rm -rf $(B_PATH)lab*

rebuild: clean all

push: clean
	git add .
	git status
	git commit -m "$(LAB_2)"
	git push origin master

play1:
	$(B_PATH)$(LAB_1)

$(LAB_1):
	$(CC) $(CFLAGS) $(PATH_1)$(SOURCES) -lm -o $(B_PATH)$(LAB_1)

play2:
	$(B_PATH)$(LAB_2)

$(LAB_2):
	$(CC) $(CFLAGS) $(PATH_2)$(SOURCES) -lm -o $(B_PATH)$(LAB_2)
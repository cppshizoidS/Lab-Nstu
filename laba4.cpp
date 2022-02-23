#include <stdio.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

#define size 4
#define size_str 127


void inp_str(char str[], int maxlen) {
    char start = TRUE;
    int len = strlen(str);
    do {
        if (start == FALSE) {
            printf("String length must be less than %d\n", maxlen);
        }
        start = FALSE;
        printf("Enter your string: ");
        gets(str);
    } while (len > maxlen);
}
void out_str(char str[], int amount_of_digits, int changes) {
    printf("String '%s', sorted with %d changes, amount of digits = %d\n", str, changes, amount_of_digits);
}
void sort(char list[size][size_str]) {
    int changes = 0;
    int start = 0;
    int index_max = -1;
    int max = -1;
    
    while (start < size) { // each string must be checked
        max = -1;
        index_max = -1;
        for (int str = start; str < size; str++) { // don't check sorted ones
            int counter = 0;
            for (int ltr = 0; list[str][ltr] != '\0'; ltr++) { // counting of digits
                counter += (list[str][ltr] >= '0' && list[str][ltr] <= '9') ? 1 : 0;
            }
            if (counter > max) {
                max = counter;
                index_max = str;
            }
        }
        // copying to temp string
        char temp[size_str];
        int ltr = -1;
        do {
            ltr++;
            temp[ltr] = list[index_max][ltr];
        } while (list[index_max][ltr] != '\0');
        // skip if sorted
        if (index_max == start) {
            start++;
            out_str(temp, max, changes);
            continue;
        }
        // string swap start
        ltr = -1;
        do {
            ltr++;
            list[index_max][ltr] = list[start][ltr];
        } while (list[start][ltr] != '\0');
        ltr = -1;
        do {
            ltr++;
            list[start][ltr] = temp[ltr];
        } while (temp[ltr] != '\0');
        // swap done
        start++;
        changes++;
        out_str(temp, max, changes);
    }
}
int main() {
    char list[size][size_str];
    // filling of list
    int str_length = size_str;
    for (int i = 0; i < size; i++) {
        inp_str(list[i], &str_length);
    }
    sort(list);
    return 0;
}


// Второй пункт
void inp_str(char* str, int* maxlen) {
    char start = TRUE;
    int len = strlen(str);

    do {
        if (start == FALSE) {
            printf("String length must be less than %d\n", *maxlen);
        }
        start = FALSE;
        printf("Enter your string: ");
        gets(str);
    } while (len > *maxlen);
}

void out_str(char* str, int* amount_of_digits, int* changes) {
    printf("String '%s', sorted with %d changes, amount of digits = %d\n", str, *changes, *amount_of_digits);
}

void sort(char** list) {    
    int changes = 0;
    int start = 0;
    int index_max = -1;
    int max = -1;
    
    while (start < size) { // each string must be checked
        max = -1;
        index_max = -1;
        for (int str = start; str < size; str++) { // don't check sorted ones
            int counter = 0;
            for (int ltr = 0; list[str][ltr] != '\0'; ltr++) { // counting of digits
                counter += (list[str][ltr] >= '0' && list[str][ltr] <= '9') ? 1 : 0;
            }
            if (counter > max) {
                max = counter;
                index_max = str;
            }
        }

        // copying to temp string
        char temp[size_str];
        int ltr = -1;
        do {
            ltr++;
            temp[ltr] = list[index_max][ltr];
        } while (list[index_max][ltr] != '\0');

        // skip if sorted
        if (index_max == start) {
            start++;
            out_str(temp, &max, &changes);
            continue;
        }

        // string swap start
        ltr = -1;
        do {
            ltr++;
            list[index_max][ltr] = list[start][ltr];
        } while (list[start][ltr] != '\0');

        ltr = -1;
        do {
            ltr++;
            list[start][ltr] = temp[ltr];
        } while (temp[ltr] != '\0');
        // swap done

        start++;
        changes++;
        out_str(temp, &max, &changes);
    }
}

int main() {
    char** list = malloc(size);
    for (int i = 0; i < size; i++) {
        list[i] = malloc(size_str);
    }

    // list filling
    int str_length = size_str;
    for (int i = 0; i < size; i++) {
        inp_str(list[i], &str_length);
    }

    sort(list);

    return 0;
}

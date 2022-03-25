#include <stdio.h>

int main() {
    int len1 = 0, len2 = 0, big_len = 0;
    int i, counter = 0;
    char str1[64];
    char str2[64];
    char big_str[128];

    // Input
    printf("Enter 1st string: ");
    gets(str1);
    printf("Enter 2nd string: ");
    gets(str2);

    // Counting of length
    for (int i = 0; str1[i] != '\0'; i++) {
        len1++;
    }
    for (int i = 0; str2[i] != '\0'; i++) {
        len2++;
    }
    printf("Length of 1st string: %d\n", len1);
    printf("Length of 2nd string: %d\n", len2);

    // Concatenating of strings
    i = 0;
    do {
        if (str1[i] != ' ') {
            big_str[big_len] = str1[i];
            big_len++;
        }
        i++;
    } while (str1[i] != '\0');
    i = 0;
    do {
        if (str2[i] != ' ') {
            big_str[big_len] = str2[i];
            big_len++;
        }
        i++;
    } while (str2[i] != '\0');
    big_str[big_len] = '\0';
    puts(big_str);

    // String processing
    counter = 0;
    i = 0;
    while (counter <= 4 && big_str[i] != '\0') {
        if (i % 3 == 2) {
            big_str[i] = ' ';
        }
        i++;
    }
    puts(big_str);

    return 0;
}

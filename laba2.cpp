#include <stdio.h>


int main() {
    char str[50];
    char new_str[50];
    printf("Type text string: ");
    gets(str);
    printf("Old string: %s\n", str);
    for (int i = 0; i < 50; i++) {
        new_str[i] = str[i];
    }
    for (int i = 0; str[i] != '\0'; i++) {
        new_str[i] = (str[i] >= 'A' && str[i] <= 'M') ? (str[i] + 32) : str[i];
        new_str[i] = (str[i] == 'a' || str[i] == 'r' || str[i] == 'h' || str[i] == 'w') ? (str[i] - 32) : new_str[i];
    }
    printf("New string: %s\n", new_str);
    return 0;
}



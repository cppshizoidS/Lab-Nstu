#include <stdio.h> // Функции стандартного ввода-вывод
#include <stdlib.h> // Функции работы с памятью
#include <iostream> // Функциями и переменными для организации ввода-вывода
#include <string.h>// Функции работы со строками

#define MAX 200

struct Bank

{
    char name[20]; //Элемент структуры name
    int birthday; //Элемент структуры birthday
    int oklad; //Элемент структуры oklad

};

struct Bank mas[MAX]; // Объявляем указатель на переменную-структуры

int num = 0;

void add_record() //Функция ввода информации

{

    struct Bank bank; // Объявляем переменную-структуру

    if (num == MAX) //Показывает, если список будет переполнен

    {

        printf("\nСписок переполнен\n");

        return;

    }

    printf("\nВведите номер элемента\n");

    int currNum;

    scanf("%d", &currNum);

    if (currNum < 0 || currNum >= MAX) //Показывает, если номер эл. будет равен 0 или больше 200

    {

        printf("\nНеверный номер элемента\n");

        return;

    }

    printf("\nФамилия: ");

    scanf("%s", &bank.name);

    printf("Год рождения: ");

    scanf("%d", &bank.birthday);

    printf("Оклад: ");

    scanf("%d", &bank.oklad);

    printf("Человек занесен в список\n");

    if (currNum >= num) //Выдает, если указанный номер элемента некорректен

    {

        if (currNum != (num + 1)) {

            printf("\nНекорректный номер элемента. Элемент будет добавлен в конец списка.\n");

        }

        currNum = ++num;

        struct Bank temp[MAX];

        if (num > 1) {

            for (int i = 0; i < (num - 1); i++) {

                temp[i] = mas[i];

            }

        }

    }
    mas[currNum - 1] = bank;
}

void show_one_record() // вывести одну запись

{

    if (num == 0) printf("\nСписок пуст\n"); //Выводит, если список пуст

    else {

        int a;

        printf("\nВведите номер записи: ");

        scanf("%d", &a);

        if (a > 0 && a <= num) {

            printf("Человек №%d\n", a);

            printf("Фамилия: %s\n", mas[a - 1].name);

            printf("Год рождения: %d\n", mas[a - 1].birthday);

            printf("Оклад: %d\n", mas[a - 1].oklad);

        } else {

            printf("\nИнформация о данной записи отсутвует\n");

        }

    }

}

void show_all_record() // показать информацию о всех

{

    if (num == 0) //Выводит, если список пуст

    {

        printf("\nСписок пуст\n");

    } else {

        for (int i = 0; i < num; i++) {

            printf("\nЧеловек № %d", i + 1);

            printf("\nФамилия: %s", mas[i].name);

            printf("\nГод рождения: %d", mas[i].birthday);

            printf("\nОклад: %d\n", mas[i].oklad);

        }

    }

}

void sorting() //сортировка

{

    if (num == 0) //Выводит, если список пуст

    {

        printf("\nСписок пуст\n");

    } else {

        struct Bank temp;// Объявляем переменную-структуру

        for (int i = 0; i < num - 1; i++)

            for (int j = i + 1; j < num; j++)

                if ((mas + i)->birthday < (mas + j)->birthday)

                {

                    temp = *(mas + i); //Применяем сортировку методом пузырька: сравниваем 2 элемента и если они идут в неправильном порядке, то меняем их местами

                    *(mas + i) = *(mas + j);

                    *(mas + j) = temp;

                }

    }

    printf("\nОтсортировано\n");

}

void save() // сохранение в файл

{

    FILE *f;

    if ((f = fopen("data.txt", "wt")) == NULL) //wt - открывает текстовый файл для записи

    {

        printf("\nОшибка при открытии файла\n");

        return;

    }

    for (int i = 0; i < num; i++) {

        fprintf(f, "%s\n", mas[i].name);

        fprintf(f, "%d\n", mas[i].birthday);

        fprintf(f, "%d\n", mas[i].oklad);

    }

    printf("\nСохранено\n");

    fclose(f);

}

void load() // загрузка из файла

{

    FILE *f;

    f = fopen("data.txt", "rt"); //rt - открывает текстовый файл для чтения

    if (f == NULL) {

        printf("\nОшибка при открытии файла\n");

        return;

    }

    int i;

    for (i = 0; !feof(f) && i <
                            MAX; i++) // Функция feof проверяет, достигнут ли конец файла, связанного с потоком. Возвращается значение, отличное от нуля, если конец файла был действительно достигнут.

    {

        if (feof(f)) break;

        struct Bank temp[MAX]; // Объявляем переменную-структуру

        for (int j = 0; j < i; j++) {

            temp[j] = mas[j];

        }

        fscanf(f, "%s", &temp[i].name);

        fscanf(f, "%d", &temp[i].birthday);

        fscanf(f, "%d", &temp[i].oklad);

        *mas = *temp;

    }

    num = i - 1;

    printf("\nЗагружено\n");

    fclose(f);

}

int main() {

    setlocale(LC_ALL, "Russian");


    for (;;) //Бесконечный цикл

    {
        char choice = '0';
        while (choice < '1' || choice > '7') {

            printf("\nКоличество записей: %d/%d", num, MAX);

            printf("\n1.Добавить запись");

            printf("\n2.Показать запись");

            printf("\n3.Показать все записи");

            printf("\n4.Сортировка");

            printf("\n5.Сохранить в файл");

            printf("\n6.Загрузить из файла");

            printf("\n7.Выход");

            printf("\nВыбор действия: ");

            scanf("%c",&choice);

            printf("\n");

        }

        switch (choice) //Оператор множественного выбора

        {

            case '1':
            {
                add_record();
                break;
            }

            case '2':
            {
                show_one_record();

                break;
            }


            case '3':

                show_all_record();

                break;

            case '4':

                sorting();

                break;

            case '5':

                save();

                break;

            case '6':

                load();

                break;

            case '7':

                exit(0);

        }

    }

}

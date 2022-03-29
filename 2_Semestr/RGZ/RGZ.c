#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <stdbool.h>

#define AMOUNT_OF_BOOKS 300 // Количество записей
#define LETTERS 24 // Максимальная длина названия книги
#define PATH_TO_DB "books.txt"

typedef struct
{
    char name[LETTERS]; // Название книги
    int pages; // Количество страниц
    int price; // Цена
} Book;

// Прототипы функций
void add_book();
void print_book();
void sort_by_price();
void print_library();
void save_library_to_file();
void load_library_from_file();

Book library[AMOUNT_OF_BOOKS];
bool books_sorted;

int main()
{
    setlocale(LC_ALL, "RUS"); // поддержка русского языка для вывода в консоли

    books_sorted = false;

    // помечаем каждый элемент БД как '-1',
    // что будет означать, что элемент не записан
    for (int book = 0; book < AMOUNT_OF_BOOKS; book++)
        library[book].price = -1;

    int function = 0;
    do
    {
        printf("\n\n*** База данных ***\n");
        printf("(1) Ввод записи с произвольным номером\n");
        printf("(2) Вывод записи с заданным номером\n");
        printf("(3) Сортировка записей по цене в порядке возрастания\n");
        printf("(4) Вывод на экран всех записей в отсортированном виде\n");
        printf("(5) Сохранение всех записей в файле\n");
        printf("(6) Чтение записей из файла\n");
        printf("(0) Выход из программы\n");

        printf("\nНомер команды: ");
        scanf("%d", &function);

        switch (function)
        {
            case 1:
                add_book();
                break;
            case 2:
                print_book();
                break;
            case 3:
                sort_by_price();
                break;
            case 4:
                print_library();
                break;
            case 5:
                save_library_to_file();
                break;
            case 6:
                load_library_from_file();
                break;
            default:
                printf("№%d - неверная команда\n", function);
                break;
        }
    } while (function != 0);

    return 0;
}

void add_book()
{
    int book_num;

    printf("# Запись №: ");
    scanf("%d", &book_num);
    book_num--;

    printf("## Ввод новой записи\n");

    printf("#### Название фирмы: ");
    scanf("%s", library[book_num].name);

    printf("#### Количество работников: ");
    scanf("%d", &library[book_num].pages);

    printf("#### Уставной капитал: ");
    scanf("%d", &library[book_num].price);

    books_sorted = false;
}

void print_book()
{
    int book_num;

    printf("# Запись №: ");
    scanf("%d", &book_num);
    book_num--;

    if (library[book_num].price > 0)
    {
        printf("## Запись №%d ##\n", book_num+1);
        printf("#### Название книги: %s\n", library[book_num].name);
        printf("#### Количество страниц: %d\n", library[book_num].pages);
        printf("#### Цена: %d\n", library[book_num].price);
    }
    else
    {
        printf("## Нет записей по номеру %d\n", book_num+1);
    }
}

void sort_by_price()
{
    if (!books_sorted)
    {
        // классический пузырьковый алгоритм сортировки
        for (int i = 0; i < AMOUNT_OF_BOOKS; i++)
        {
            for (int book = AMOUNT_OF_BOOKS-1; book > 0; book--)
            {
                // выполняем перестановку если:
                if (library[book-1].price < 0 || // запись отутствует
                    (library[book].price   > 0 && // запись есть И
                     library[book].price   < library[book-1].price)) // она удовлетворяет условию сортировки
                {
                    Book temp_book = library[book-1];
                    library[book-1] = library[book];
                    library[book] = temp_book;
                }
            }
        }

        printf("# Сортировка выполнена\n");
        books_sorted = true;
    }
}

void print_library() {
    if (!books_sorted)
        sort_by_price();

    for (int book = 0; book < AMOUNT_OF_BOOKS; book++)
    {
        if (library[book].price > 0)
        {
            printf("## Запись №%d ##\n", book+1);
            printf("#### Название книги: %s\n", library[book].name);
            printf("#### Количество страниц: %d\n", library[book].pages);
            printf("#### Цена: %d\n", library[book].price);
        }
    }
}

void save_library_to_file()
{
    // открытие файла для запись - w
    FILE* books_db = fopen(PATH_TO_DB, "w");

    if (books_db != NULL) // проверка, открыт ли файл
    {
        int amount_of_books = 0;

        for (int book = 0; book < AMOUNT_OF_BOOKS; book++)
        {
            if (library[book].price > 0)
            {
                amount_of_books++;
            }
        }
        fprintf(books_db, "~%d~\n", amount_of_books);

        for (int book = 0; book < AMOUNT_OF_BOOKS; book++)
        {
            if (library[book].price > 0)
            {
                fprintf(books_db, "%s %d %d\n", library[book].name, library[book].pages, library[book].price);
            }
        }

        printf("# Записи о книгах сохранены в файл '%s'\n", PATH_TO_DB);
    }
    else
    {
        printf("# Не удалось сохранить записи\n");
    }

    fclose(books_db); // закрытие файла
}

void load_library_from_file()
{
    // открытие файла на чтение - r
    FILE* books_db = fopen(PATH_TO_DB, "r");

    if (books_db != NULL) // проверка, открыт ли файл
    {
        // помечаем каждую ячейку БД как пустую,
        // чтобы очистить её перед заполнением из файла
        for (int book = 0; book < AMOUNT_OF_BOOKS; book++)
        {
            library[book].price = -1;
        }

        int amount_of_books = 0;
        fscanf(books_db, "~%d~\n", &amount_of_books);

        for (int book = 0; book < amount_of_books; book++)
        {
            fscanf(books_db, "%s %d %d\n", library[book].name, &library[book].pages, &library[book].price);
        }

        printf("# Записи о книгах загружены из файла '%s'\n", PATH_TO_DB);
    }
    else
    {
        printf("# Не удалось загрузить записи\n");
    }

    fclose(books_db); // закрытие файла
    books_sorted = false;
}

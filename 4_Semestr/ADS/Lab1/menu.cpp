#include "container.hpp"

#include "tl/expected.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;


class test_menu
{
public:
    void start()
    {
        show_help();

        while (true) {
            int cmd = command();

            if (!cmd) {
                cout << "Завершение программы\n";
                exit(0);
            } else if (cmd == -1) {
                cout << "Неверная команда\n";
                show_help();
            }
        }
    }

private:
    pair<const string, evector<int>>* current = nullptr;
    unordered_map<string, evector<int>> containers;

    int
    command()
    {
        int command = 0;

        cout << "[" << (current ? current->first : "*") << "]: ";
        cin >> command;

        if (!command)
            return 0;
        else if (command > 21)
            return -1;

        switch (command) {
            case 1: {
                create_new(0);
                break;
            } case 2: {
                int capacity = read_positive();
                create_new(capacity);
                break;
            } case 3: {
                create_based(current->second);
                break;
            } case 4: {
                int index = read_positive();
                if (check_index(index))
                    print_by_index(index);
                break;
            } case 5: {
                int index = read_positive();
                if (check_index(index))
                    cin >> current->second[index];
                break;
            } case 6: {
                cout
                    << current_name_with("items")
                    << current->second << endl;
                break;
            } case 7: {
                cout
                    << current_name_with("size")
                    << current->second.size() << endl;
                break;
            } case 8: {
                cout
                    << current_name_with("capacity")
                    << current->second.capacity() << endl;
                break;
            } case 9: {
                cout
                    << current_name_with("empty")
                    << (current->second.empty() ? "пустой" : "не пустой")
                    << endl;
                break;
            } case 10: {
                current->second.clear();
                break;
            } case 11: {
                int value = 0;

                cout << "Введите элемент: ";
                cin >> value;

                current->second.push_back(value);
                break;
            } case 12: {
                int value = 0;

                cout << "Введите элемент: ";
                cin >> value;

                current->second.push_front(value);
                break;
            } case 13: {
                current->second.pop_back();
                break;
            } case 14: {
                current->second.pop_front();
                break;
            } case 15: {
                int value = 0;

                cout << "Введите искомое значение: ";
                cin >> value;

                auto iter = current->second.find(value);
                string result = "Элемент ";
                result += (
                    iter != current->second.end()
                    ? "существет"
                    : "не существует"
                );
                cout << current_name_with("find") << result << endl;
                break;
            } case 16: {
                insert_value();
                break;
            } case 17: {
                delete_value();
                break;
            } case 18: {
                switch_container(read_name(false));
                break;
            } case 19: {
                delete_container(read_name(false));
                break;
            } case 20: {
                show_list();
                break;
            } case 21: {
                show_help();
                break;
            }
        }

        return command;
    }

    void
    delete_value()
    {
        int elem = 0;

        cout << "Введите элемент, который необходимо удалить: ";
        cin >> elem;

        auto iter = current->second.find(elem);
        if (iter == current->second.end()) {
            cout << "Нет такого элемента в текущем контейнере\n";
            return;
        }

        current->second.erase(iter);
    }

    void
    insert_value()
    {
        int elem = 0, value = 0;

        cout
            << "Введите элемент, перед которым "
            << "будет произведена вставка: ";
        cin >> elem;

        auto iter = current->second.find(elem);
        if (iter == current->second.end()) {
            cout << "Нет такого элемента в текущем контейнере\n";
            return;
        }

        cout << "Введите значение для вставки: ";
        cin >> value;

        current->second.insert(iter, value);
    }

    void
    switch_container(const string& name)
    {
        auto iter = containers.find(name);

        if (iter == containers.end()) {
            cout << "Нет такого контейнера\n";
            return;
        }

        current = &*iter;
    }

    void
    show_list()
    {
        int num = 0;

        for (const auto& [name, container] : containers)
            cout << "[" << num++ << "] " << name << endl;
    }

    void
    delete_container(const string& name)
    {
        auto iter = containers.find(name);

        if (iter == containers.end()) {
            cout << "Нет такого контейнера\n";
            return;
        }

        if (iter->first == name || containers.size() == 1)
            current = nullptr;

        containers.erase(iter);
    }

    string
    current_name_with(const string& with)
    { return "[" + current->first + ":" + with + "]: "; }

    bool
    check_index(int index)
    { return index < current->second[index]; }

    void
    print_by_index(int index)
    {
        cout
            << current->first << "[" << index << "]: "
            << current->second[index] << endl;
    }

    void
    create_based(const evector<int>& ev)
    {
        string name      = read_name();
        containers[name] = ev;
        current          = &*containers.find(name);
    }

    void
    create_new(int capacity)
    {
        string name      = read_name();
        containers[name] = evector<int>(capacity);
        current          = &*containers.find(name);
    }

    void
    show_help()
    {
        cout
            << "1.  Новый пустой контейнер\n"
            << "2.  Новый контейнер заданного размера\n"
            << "3.  Новый контейнер на основе текущего\n"
            << "4.  Получение элемента по индексу\n"
            << "5.  Запись элемента по индексу\n"
            << "6.  Вывод элементов контейнера\n"
            << "7.  Получение количества элементов в контейнере\n"
            << "8.  Получение объёма контейнера\n"
            << "9.  Проверка на пустоту\n"
            << "10. Очистка контейнера\n"
            << "11. Добавление в конец контейнера\n"
            << "12. Добавление в начало контейнера\n"
            << "13. Удаление из конца контейнера\n"
            << "14. Удаление из начала контейнера\n"
            << "15. Поиск по значению\n"
            << "16. Вставка перед позицией\n"
            << "17. Удаление позиции\n"
            << "18. Переключение контейнера\n"
            << "19. Удаление контейнера\n"
            << "20. Список контейнеров\n"
            << "0.  Выход\n";
    }

    string
    read_name(bool mode = true)
    {
        bool correct = false;
        string name;

        do {
            cout << "Введите имя контейнера (1-8 символов): ";
            cin >> name;

            if (mode) {
                if (containers.find(name) != containers.end()) {
                    cout << "Контейнер с таким имененм уже существует\n";
                    continue;
                }
            } else {
                if (containers.find(name) == containers.end()) {
                    cout << "Контейнер с таким именем не сещуствует\n";
                    continue;
                }
            }

            if (name.empty() || name.length() > 8)
                cout << "Неверный ввод, попробуйте снова\n";
            else
                correct = true;
        } while (!correct);

        return name;
    }

    int
    read_positive()
    {
        int cap      = 0;
        bool correct = false;

        do {
            cout << "Введите число > 0: ";
            cin >> cap;

            if (cap < 0)
                cout << "Неверный ввод, попробуйте снова\n";
            else
                correct = true;
        } while (!correct);

        return cap;
    }
};


int main()
{
    test_menu menu;
    menu.start();

    return 0;
}

# Лабораторная работа по АиСД №1. Вариант 2

Односвязная структура данных на базе массива с индексными указателями

## Сборка тестов

Убедитесь, что у вас установлен менеджер пакетов 
[conan](https://conan.io) и произведены его 
необходимые базовые настройки:  
```sh
conan profile new default --detect
conan profile update settings.compiler.libcxx=libstdc++11 default # linux
conan profile update settings.compiler.libcxx=libc++11 default # macos
```

Доступны опциональные флаги сборки DEBUG и CMAKE_EXPORT_COMPILE_COMMANDS

```sh
mkdir build
cd build
conan install ..
cmake ..
make
```

## Запуск тестов

Находясь в v2/build  
`./bin/test`


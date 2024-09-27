#!/bin/bash


# Создание и переход в директорию сборки
mkdir -p cmake-build-debug
cd cmake-build-debug

# Конфигурация и сборка проекта
cmake ..
cmake --build .

# Копирование исполняемых файлов в корневую директорию
cp src/searching_tree/searching_tree ../searching_tree
cp src/wordly/wordly ../wordly

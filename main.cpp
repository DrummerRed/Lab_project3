#include "header.h"

int main()
{
    setlocale(LC_ALL, "Rus");
    SetConsoleCP(1251);                 // установка кодовой страницы win-cp 1251 в поток ввода
    SetConsoleOutputCP(1251);           // установка кодовой страницы win-cp 1251 в поток вывода

    read_file("file.txt");

    return 0;
}
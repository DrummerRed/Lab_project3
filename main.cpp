#include "header.h"

int main()
{
    setlocale(LC_ALL, "Rus");
    SetConsoleCP(1251);                 
    SetConsoleOutputCP(1251);           

    read_file("file.txt");

    return 0;
}
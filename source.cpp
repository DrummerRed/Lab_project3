#include "header.h"

int read_file(string file_name)                 // Чтение файла 
{                                               // Возвращает 0 при успешном чтении файла
    int code = 0;                               // Возвращает 1 при ошибке чтения файла

    ifstream file;
    file.open(file_name);

    if (!file.is_open())
        code = 1;
    else
    {
        char ch;
        string str;
        while ((ch = file.get()) != EOF)
        {                   // функция проверок на символы для выделения слов
            str += ch;
        }                   // выделенное из текста слово необходимо проверять на часть речи и уже после записывать
        cout << str;

        file.close();
    }

    return code;
}
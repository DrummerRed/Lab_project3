#include "header.h"

void menu()                             // Главное меню программы              
{
    string input_file = "";
    bool flag_esc = false;

    while (!flag_esc)
    {
        print_menu(input_file);
        int key = get_menu_key();

        if (key == KEY_ESC)
            flag_esc = true;
        else if (key == '1')
            choose_input_file(input_file);
        else if (key == '2')
        {
            search_rhymes_menu(input_file);
            // read_file(input_file);                      // добавить проверки на отсутствие введенного файла
            // run_search(input_file);
            // _getch();
        }
    }
    system("cls");
}

void print_menu(string input_file)              // Вывод главного меню
{
    system("cls");
    cout << "Поиск рифм\n";
    cout << "---------------------------\n";
    cout << "Файл для поиска: " << (input_file == "" ? "не выбран" : input_file) << "\n\n";       /// ???
    cout << "1 - Выбрать файл для поиска\n";
    cout << "2 - Выполнить поиск рифм\n";
    cout << "Esc - Выход\n";
}

int get_menu_key()                              // Ждём, пока нажмут 1, 2 или Esc,
{                                               // Возвращает 0 в случае ошибки
    bool is_pressed = false;                    
    int key = 0;
    while (!is_pressed)
    {
        key = _getch();
        if (key == 0 || key == 224) // стрелки и F1-F12 приходят двумя кодами
        {
            _getch();                       ///// ???
            continue;
        }
        else if (key == '1' || key == '2' || key == KEY_ESC)
            is_pressed = true;
    }
    return key;
}

void choose_input_file(string& input_file)                      // Считывание имени входного файла
{
    bool flag_exit = false;
    while (!flag_exit)
    {
        system("cls");
        cout << "Введите имя файла для поиска (Enter - подтвердить, Esc - назад):\n";
        string name;
        if (!input_file_name(name))                             // Выход при нажатии ESc
            flag_exit = true;
        else
        {
            ifstream file(name);
            if (file.is_open())                                     // Файл найден
            {
                file.close();
                input_file = name;
                printf("\nФайл \"%s\" был успешно выбран.\n", name.c_str());
                wait_key();
                flag_exit = true;
            }
            else                                                    // Файл не найден
            {
                cout << "\nФайл \"" << name << "\" не найден.\n";
                wait_key();
            }
        }
    }
}

void wait_key()                                                     // Ожидание нажатия клавиши
{
    cout << "Нажмите любую клавишу...";
    _getch();
}

bool input_file_name(string& name)              // Посимвольный ввод имени файла
{                                               // Возвращает false при нажатии Esc, иначе возвращает true
    bool result = true;
    bool exit_flag = false;
    string eng_symb = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-.";
    string ru_symb = "абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    name = "";

    while (!exit_flag)
    {
        int key = _getch();
        if (key == 0 || key == 224)                         // Для спец клавиш
        {
            _getch();
            continue;
        }
        else if (key == KEY_ESC)
        {
            result = false;
            exit_flag = true;
        }
        else if (key == KEY_ENTER)
        {
            if (name != "")
                exit_flag = true;
        }
        else if (key == KEY_BACKSPACE)
        {
            if (name != "")
            {
                name.erase(name.length() - 1);
                cout << "\b \b";                            // "Затираем" символ
            }
        }
        else if ((eng_symb.find((char)key) != -1) || (ru_symb.find((char)key) != -1))           
        {
            name += (char)key;
            cout << (char)key;
        }
    }
    cout << "\n";
    // add_txt(name);
    return result;
}

// void add_txt(string& name) // Если пользователь не ввёл ".txt", добавляем сами
// {
//     if (!ends_with(name, ".txt"))
//         name += ".txt";
// }

// bool ends_with(string str, string ending)
// {
//     if (str.length() < ending.length())
//         return false;
//     return str.substr(str.length() - ending.length()) == ending;
// }

void search_rhymes_menu(string input_file)                           // Раздел меню "поиск рифм"
{
    bool flag_esc = false;

    while (!flag_esc)
    {
        system("cls");
        cout << "Для возвращения в меню нажмите Esc\n" << endl;

        if (input_file != "")
            read_file(input_file);
        else
            cout << "Поиск рифм недоступен, пока не выбран текстовый файл!";

        int key = _getch();
        if (key == KEY_ESC)
            flag_esc = true;
    }
}
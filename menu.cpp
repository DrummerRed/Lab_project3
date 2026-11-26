#include "header.h"

void menu()                             // Главное меню программы              
{
    string input_file = "";
    bool flag_esc = false;
    vector<stress_word> dictionary;                             // словарь ударений
    int dict_size = load_dictionary(DICT_FILE, &dictionary);    // Возвращает размер словаря или -1, если файл словаря не найден

    while (!flag_esc)
    {
        print_menu(input_file, dict_size);
        int key = get_menu_key();

        if (key == KEY_ESC)
            flag_esc = true;
        else if (key == '1')
            choose_input_file(input_file);
        else if (key == '2')
            search_rhymes_menu(input_file, &dictionary);
    }
    system("cls");
}

void print_menu(string input_file, int dict_size)              // Вывод главного меню
{
    system("cls");
    cout << "Поиск рифм\n";
    cout << "---------------------------\n";
    cout << "Файл для поиска: ";
    if (input_file == "")
        cout << "не выбран\n";
    else
        cout << input_file << "\n";
    cout << "Словарь ударений: ";
    if (dict_size == -1)
        cout << "не найден (поиск только по окончаниям)\n\n";
    else
        cout << "загружен, слов: " << dict_size << "\n\n";
    cout << "1 - Выбрать файл для поиска\n";
    cout << "2 - Выполнить поиск рифм\n";
    cout << "Esc - Выход\n";
}

int get_menu_key()                              // Ожидание нажатия 1, 2 или Esc,
{                                               // Возвращает код введенного символа
    bool is_pressed = false;
    int key = 0;
    while (!is_pressed)
    {
        key = _getch();
        if (key == 0 || key == 224) // стрелки и F1-F12 приходят двумя кодами
        {
            _getch();                       
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

bool input_file_name(string& name, bool output_name)              // Посимвольный ввод имени файла
{                                                                 // Возвращает false при нажатии Esc, иначе возвращает true
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

    if (result && !output_name)
        add_txt(name);   

    return result;
}

void add_txt(string& name)                      // Если пользователь не ввел ".txt", добавляем его
{
    if (!ends_with(to_lower(name), ".txt"))     // "Text.TXT" тоже считается введенным
        name += ".txt";
}

bool ends_with(string str, string ending)       // Проверка, что строка оканчивается на ending
{                                               // Возвращает true, если строка оканчивается на ending
    bool result;                                // иначе возвращает false

    if (str.length() < ending.length())
        result = false;

    else
    {
        result = (str.substr(str.length() - ending.length()) == ending);
    }

    return result;
}

void search_rhymes_menu(string input_file, vector<stress_word>* d)                           // Раздел меню "Поиск рифм"
{
    system("cls");
    if (input_file == "")
        cout << "Поиск рифм невозможен, пока не выбран текстовый файл!\n";
    else
    {
        vector<participle> words;                                    // найденные деепричастия
        vector<string> sentences;                                    // все предложения текста

        if (read_file(input_file, &words, &sentences, d) == 1)
            cout << "Не удалось открыть файл \"" << input_file << "\".\n";
        else
        {
            cout << "Анализ файла \"" << input_file << "\" завершен.\n";
            cout << "---------------------------\n";
            cout << "Предложений в тексте:      " << sentences.size() << "\n";
            cout << "Найдено деепричастий:      " << words.size() << "\n";
            cout << "Найдено рифмующихся пар:   " << count_rhymes(&words) << "\n\n";

            string report_file;
            if (input_report_name(report_file))          // Esc - отчет не сохраняем
            {
                string rhymes_file = report_file + "_rhymes.txt";                           
                string sentences_file = report_file + "_sentences.txt";
                bool rhymes_report = save_rhymes_report(report_file, &words);
                bool sentences_report = save_sentences_report(report_file, &words, &sentences);

                if (rhymes_report && sentences_report)
                {
                    string rhymes_file = report_file + "_rhymes.txt";                           
                    string sentences_file = report_file + "_sentences.txt";
                    cout << "\nФайлы \"" << rhymes_file << " и ";
                    cout << "\"" << sentences_file << "\" ";
                    cout << "успешно сохранены.\n";
                }
                else 
                {
                    if (!rhymes_report)
                        cout << "\nОшибка: не удалось создать файл \"" << rhymes_file << "\".\n";  
                    if (!save_sentences_report(report_file, &words, &sentences)) 
                        cout << "\nОшибка: не удалось создать файл \"" << sentences_file << "\".\n";
                }
            }
            else
                cout << "Отчет не сохранен.\n";
        }
    }
    cout << "\nДля возвращения в меню нажмите Esc";
    while (_getch() != KEY_ESC);                                     // ждем Esc
}

bool input_report_name(string& report_file)                         // Ввод имени файла для отчета
{                                                                   // Возвращает false, если нажат Esc
    bool result = false;                                            // Возвращает true, если введено корректное имя файла
    bool flag_exit = false;
    int i = 0;
    while (!flag_exit)
    {
        if (i == 0)
        {
            cout << "Будут созданы два файла:\n";
            cout << "  <имя>_rhymes.txt    - содержит найденные пары рифм\n";
            cout << "  <имя>_sentences.txt - содержит предложения с найденными словами\n";
            cout << "Введите общее имя файлов (Enter - подтвердить, Esc - не сохранять):\n";
        }

        if (!input_file_name(report_file, true))
            flag_exit = true;                                        // Esc
        else if (check_file(report_file) != 0)
        {
            int code = check_file(report_file);
            file_is_exist(report_file, code);
        }
        else
        {
            result = true;
            flag_exit = true;
        }

        i++;
    }
    return result;
}

int check_file(string report_file)                                  // Проверка на существование выходного файла
{                                                                   // Возвращает 0, если файлов с таким названием не существует
    int code = 0;                                                   // Возвращает 1, если такой файл рифм уже есть
    string rhymes_file = report_file + "_rhymes.txt";               // Возвращает 2, если такой файл предложений уже есть
    string sentences_file = report_file + "_sentences.txt";         // Возвращает 3, если оба файла уже существуют

    ifstream rhymes(rhymes_file);
    if (rhymes.is_open())
    {
        rhymes.close();
        code = 1;
    }

    ifstream sentences(sentences_file);
    if (sentences.is_open())
    {
        if (code == 0)
            code = 2;
        else 
            code = 3;

        sentences.close();
    }

    return code;
}

void file_is_exist(string report_file, int code)                        // Вывод сообщения о существовании файла
{
    string rhymes_file = report_file + "_rhymes.txt";                      
    string sentences_file = report_file + "_sentences.txt";

    if ((code == 1) || (code == 2))
    {
        if (code == 1)
            printf("\nОшибка! Файл \"%s\" уже существует!\n", rhymes_file.c_str());
        else if (code == 2)
            printf("\nОшибка! Файл \"%s\" уже существует!\n", sentences_file.c_str());
        
        printf("Выберите другое имя или удалите данный файл!\n");
    }
    else if (code == 3)
    {
        printf("\nОшибка! Файлы \"%s\" и \"%s\" уже существуют!\n", rhymes_file.c_str(), sentences_file.c_str());
        printf("Выберите другое имя или удалите данные файлы!\n");
    }
}
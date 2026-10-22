#include "header.h"

void preparation_and_reading(string file_name)               // Создание структур данных для запис слов
{                                                           // и чтение файла
    vector<participle> participle_words;
    read_file(file_name, &participle_words);

    cout << "=======================" << endl;
    cout << "Найденные рифмы:\n";
    rhymes_output(&participle_words);

}

int read_file(string file_name, vector<participle>* v)        // Чтение файла 
{                                                             // Возвращает 0 при успешном чтении файла
    int code = 0;                                             // Возвращает 1 при ошибке чтения файла

    ifstream file;
    file.open(file_name);

    if (!file.is_open())
        code = 1;
    else
    {
        int ch;
        string str;
        string end_of_word;
        while ((ch = file.get()) != EOF)
        {     
            char symb = char(ch);
            if (check_symbol(symb) == 0)
                str += symb;
            else if (check_symbol(symb) == 1)
            {
                str = to_lower(str);
                if (is_perfective_participle(str, end_of_word))
                {
                    printf("%s\t- %s\n", str.c_str(), end_of_word.c_str());
                    if (unique_elem(str, v))
                    {
                        participle temp;
                        temp.word = str;
                        temp.end_of_word = end_of_word;
                        (*v).push_back(temp);
                    }
                }

                str = "";
                end_of_word = "";
            }
        }                   // выделенное из текста слово необходимо проверять на часть речи и уже после записывать
        file.close();
    }
    return code;
}

bool unique_elem(string str, vector<participle>* v)             // Проверка на наличие в векторе указанного слова
{                                                               // Возвращает true, если данный слово в векторе отсутствует
    bool result = true;                                         // Иначе возвращает false
    int length = (*v).size();
    for (int i=0; i<length; i++)
    {
        if ((*v)[i].word == str)
        {
            result = false;
            break;
        }
    }
    return result;
}

int check_symbol(char ch)                           // Проверка на символ разделения 
{                                                   // Возвращает 0, если символ не разделительный
    int split_str = 0;                              // Возвращает 1, если символ - разделитель
    string split_symbols = " .,!?:;\n\t";           // Возвращает 2, если символ не разделительный, но не требуется для записи 
    string other_symbols = "\"\'()";

    if (split_symbols.find(ch) != -1)
        split_str = 1;

    else if (other_symbols.find(ch) != -1)
        split_str = 2;

    return split_str;
}


////////////////////////////////////////////////////////////      Нужно сделать аналог для несовершенного вида
///////////////////////////////////////////////////////////
bool is_perfective_participle(string str, string& end_of_word)  // Проверка, что слово является совершенным деепричастием
{                                                               // возвращает true, если слово совершенное дееприч. 
    bool checker = false;                                       // иначе возвращает false
    int length = str.length();

    if (length >= 4)                    ////
    {
        if (length >= 7)
        {
            if (str.find("вшись") == (length - 5))             // "вшись"
            {
                checker = true;
                end_of_word = "вшись";
            }

            else if (str.find("шись") == (length - 4))          // "шись" 
            {
                checker = true;
                end_of_word = "шись";
            }
        }

        if (length >= 5)
        {
            if ((str.find("вши") == (length - 3)) && check_exceptions(1, str))              // "вши"
            {
                checker = true;
                end_of_word = "вши";
            }

            else if ((str.find("ши") == (length - 2)) && (str[length - 3] != 'в') &&             // "ши"
                    ru_consonants(str[length - 3]) && check_exceptions(2, str))     
            {
                checker = true;
                end_of_word = "ши";
            }
        }
        
        if (length >= 4)
        {
            if (str.find("ув") == (length - 2))
            {
                checker = true;
                end_of_word = "ув";
            }

            else if ((str.find("ыв") == (length - 2)) && check_exceptions(3, str))
            {
                checker = true;
                end_of_word = "ыв";
            }

        }
    }

    return checker;
}
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

bool ru_consonants(char symb)                       // Проверка, что символ является согласной буквой
{                                                   // Возвращает true, если символ согласная буква
    bool result = false;                            // Иначе возвращает false
    string rus_low = "бвгджзйклмнпрстфхцчшщ";

    if (rus_low.find(symb) != -1) 
        result = true;
    return result;
}

bool check_exceptions(int variant, string str)                    // Проверка строки на исключения
{                                                                 // int variant - вид проверяемого исключения
    bool result = true;                                           // возвращает true, если исключение не найдено
    int length = str.length();                                    // возвращает false, если исключение найдено

    if (variant == 1)                           // Исключения для "вши"
    {
        const int SIZE = 6;
        string array[SIZE] = {"Ковши", "ковши", "Левши", "левши", "Правши", "правши"};      /////////////CAPS
        result = check_dictionary(SIZE, array, str);
    }
    else if (variant == 2)                   // Проверка для окончания "ши"
    {
        const int SIZE = 5;
        string array[SIZE] = {"орши", "арши", "ирши", "льши", "нтши"};
        
        for (int i=0; i<SIZE; i++)
        {
            if (str.find(array[i]) == (length - 4))
            {
                result = false;
                break;
            }
        }
    }
    else if (variant == 3)                  // Исключения для "ыв"
    {
        const int SIZE = 14;
        string array[SIZE] = {"взрыв", "порыв", "обрыв", "разрыв", "прорыв", "срыв",
                            "отрыв", "подрыв", "надрыв", "перерыв", "нарыв",
                            "наплыв", "заплыв", "смыв"};
        result = check_dictionary(SIZE, array, str);
    }
    return result;              // уменьшить функцию или разделить на несколько
}

bool check_dictionary(const int SIZE, string* array, string str)                   // Проверка на слова исключения     
{                                                                                // Возвращает true, если исключение найдено
    bool result = true;                                                          // иначе возвращает false

    for (int i=0; i<SIZE; i++)
    {
        if (str == array[i])
        {
            result = false;
            break;
        }
    }
    return result;
}

string to_lower(string str)
{
    for (int i=0; i<str.length(); i++)
        str[i] = tolower(str[i]);
    return str;
}
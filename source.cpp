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
                if (is_perfective_participle(str, end_of_word) ||        // совершенный вид
                    is_imperfective_participle(str, end_of_word))        // несовершенный вид
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
            /////////
            else if (((str.rfind("ав") == length - 2) || (str.rfind("яв") == length - 2) ||    // "ав", "яв", "ив", "ев"    
                (str.rfind("ив") == length - 2) || (str.rfind("ев") == length - 2)) &&
                ru_consonants(str[length - 3]) && (str.find('-') == -1) &&                // перед гласной - согласная
                check_exceptions(6, str))                                                  // (чтобы не брать "героев"),
            {                                                                                   // без дефиса ("на-а-против")
                checker = true;
                end_of_word = str.substr(length - 2);
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

// деепричастия несовершенного вида
bool is_imperfective_participle(string str, string& end_of_word)   // Проверка, что слово является несовершенным деепричастием
{                                                                   // возвращает true, если слово несоверш. дееприч.
    bool checker = false;                                           // иначе возвращает false
    int length = str.length();
    string hissing = "жшчщ";                    // шипящие
    string before_ivaya = "кгхжшчщ";            // буквы, после которых пишется "-ивая"

    if (length >= 5)
    {
        if (str.rfind("ясь") == length - 3)                                     // "ясь": купаясь, смеясь
        {
            checker = true;
            end_of_word = "ясь";
        }
        else if ((str.rfind("ась") == length - 3) && (hissing.find(str[length - 4]) != -1))    // "ась": кружась, держась
        {
            checker = true;
            end_of_word = "ась";
        }
        else if (str.rfind("уя") == length - 2)                                 // "уя": рисуя, танцуя
        {
            checker = true;
            end_of_word = "уя";
        }
        else if (str.rfind("юя") == length - 2)                                 // "юя": воюя, горюя
        {
            checker = true;
            end_of_word = "юя";
        }
        else if (str.rfind("ывая") == length - 4)                               // "ывая": завывая
        {
            checker = true;
            end_of_word = "ывая";
        }
        else if ((str.rfind("ивая") == length - 4) && (before_ivaya.find(str[length - 5]) != -1) &&
            check_exceptions(4, str))                                           // "ивая": поглаживая
        {
            checker = true;
            end_of_word = "ивая";
        }
        else if ((str.rfind("евая") == length - 4) && check_exceptions(4, str)) // "евая": распевая
        {
            checker = true;
            end_of_word = "евая";
        }
        else if (((str.rfind("ая") == length - 2) || (str.rfind("яя") == length - 2)) &&
            !is_adjective(str))                                                 // "ая", "яя": желая, гуляя
        {
            checker = true;
            end_of_word = str.substr(length - 2);
        }
    }

    return checker;
}

bool is_adjective(string str)                       // Проверка, что слово на "ая"/"яя" является прилагательным
{                                                   // возвращает true, если прилагательное
    bool result = false;                            // иначе возвращает false
    int length = str.length();
    char before = str[length - 4];                  // буква перед последней согласной: "кра[с]ная"

    const int SIZE = 9;                             // деепричастия с двумя согласными перед "ая"
    string verbs[SIZE] = { "сверкая", "играя", "мечтая", "листая", "исполняя",
                           "объясняя", "выполняя", "наполняя", "заполняя" };

    if (!check_dictionary(SIZE, verbs, str))        // слово есть в списке деепричастий
        result = false;
    else if ((str.rfind("вляя") == length - 4) || (str.rfind("бляя") == length - 4) ||
        (str.rfind("пляя") == length - 4) || (str.rfind("мляя") == length - 4) ||
        (str.rfind("лжая") == length - 4))             // объявляя, употребляя, укрепляя, утомляя, продолжая
        result = false;
    else if (ru_consonants(before) || before == 'ь')        // две согласные перед "ая": красная, тёплая, горькая
        result = true;
    else if (str.rfind("вая") == length - 3)                // новая, правая, живая (кроме "давая", "знавая", "ставая")
        result = !((str.rfind("давая") == length - 5) ||
            (length >= 6 && str.rfind("знавая") == length - 6) ||
            (length >= 6 && str.rfind("ставая") == length - 6));
    else if ((str.rfind("окая") == length - 4) || (str.rfind("ёкая") == length - 4) ||
        (str.rfind("атая") == length - 4) || (str.rfind("ячая") == length - 4) ||
        (str.rfind("щая") == length - 3))              // высокая, далёкая, богатая, горячая, летящая
        result = true;
    else if ((str.rfind("емая") == length - 4) || (str.rfind("омая") == length - 4))
        result = true;                                      // нескрываемая, знакомая (причастия и прилагательные)
    else if (!check_exceptions(5, str))                     // частые прилагательные: белая, старая, синяя
        result = true;

    return result;
}

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
    else if (variant == 4)                  // Исключения для "ивая", "евая"        /////
    {
        const int SIZE = 11;
        string array[SIZE] = { "живая", "фальшивая", "плешивая", "паршивая", "вшивая", "боевая",
                               "полевая", "ключевая", "дождевая", "грушевая", "межевая" };
        result = check_dictionary(SIZE, array, str);

        if ((length >= 6) && (str.rfind("чивая") == length - 5) && (str.rfind("ачивая") != length - 6))
            result = false;                 // навязчивая, задумчивая (но не оборачивая, поворачивая)
    }
    else if (variant == 5)                  // Исключения для "ая", "яя": прилагательные, местоимения, существительные
    {
        const int SIZE = 69;
        string array[SIZE] = { "белая", "смелая", "целая", "спелая", "зрелая", "умелая", "милая", "гнилая",
                               "малая", "вялая", "усталая", "старая", "серая", "скорая", "бурая", "хмурая",
                               "сырая", "золотая", "крутая", "святая", "молодая", "седая", "худая", "тупая",
                               "слепая", "скупая", "прямая", "немая", "хромая", "любимая", "тихая", "сухая",
                               "глухая", "плохая", "строгая", "дорогая", "другая", "большая", "хорошая", "синяя",
                               "голая", "весёлая", "удалая", "сарая", "урожая", "попугая", "негодяя", "лентяя",
                               "китая", "николая", "трамвая",
                               "случая", "которая", "некоторая", "какая", "такая", "всякая", "никакая", "самая",
                               "любая", "пролитая", "убитая", "забытая", "разбитая", "открытая", "закрытая",
                               "скрытая", "умытая", "надутая" };
        result = check_dictionary(SIZE, array, str);
    }
    else if (variant == 6)                  // Исключения для "ав", "яв", "ив", "ев"
    {
        const int SIZE = 62;
        string array[SIZE] = { "рукав", "состав", "устав", "сплав", "застав", "канав", "забав", "расправ",
                               "облав", "держав", "приправ", "переправ", "лукав", "кровав", "величав",
                               "ярослав", "вячеслав", "святослав", "владислав", "станислав", "мстислав",
                               "ростислав", "мотив", "архив", "актив", "залив", "прилив", "отлив", "разлив",
                               "пролив", "курсив", "массив", "коллектив", "объектив", "детектив", "красив",
                               "ленив", "счастлив", "правдив", "учтив", "игрив", "болтлив", "терпелив",
                               "справедлив", "ревнив", "молчалив", "фальшив", "против", "напротив",
                               "супротив", "позитив", "негатив", "перспектив", "локомотив", "дефектив",
                               "напев", "припев", "запев", "посев", "королев", "нагрев", "перегрев" };
        result = check_dictionary(SIZE, array, str);
    }                                                                                               ////
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
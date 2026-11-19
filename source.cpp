#include "header.h"

int read_file(string file_name, vector<participle>* v, vector<string>* s, vector<stress_word>* d)  
{                                        // Чтение файла, v - деепричастия, s - предложения, d - словарь ударений
    int code = 0;                        // Возвращает 0 при успешном чтении файла
    ifstream file;                       // При ошибке чтения возвращает 1
    file.open(file_name);

    if (!file.is_open())
        code = 1;
    else
    {
        int ch;
        string str;                                           // текущее слово
        string sentence;                                      // текущее предложение
        string end_symbols = ".!?…";                          // символы конца предложения
        char prev = 0;                                        // предыдущий символ 

        while ((ch = file.get()) != EOF)
        {
            char symb = char(ch);
            bool empty_line;

            if ((symb == '\n') && (prev == '\n'))
                empty_line = true;
            else
                empty_line = false;

            if (symb != '\r')
                prev = symb;
            if (check_symbol(symb) == 0)
                str += symb;
            else if (check_symbol(symb) == 1)                 // слово закончилось
            {
                process_word(str, v, (*s).size(), d);            // номер предложения = сколько уже сохранено
                str = "";
            }

            if (symb == '\n' || symb == '\t' || symb == '\r')  // переносы строк внутри предложения
                symb = ' ';                                   // заменяем пробелом
            if (!(symb == ' ' && (sentence == "" || sentence[sentence.length() - 1] == ' ')))
                sentence += symb;                             // лишние пробелы не добавляем

            if ((end_symbols.find(symb) != -1) || empty_line)  // предложение закончилось
            {
                sentence = trim(sentence);
                if (sentence.length() > 1)                    // "..." не дает пустых предложений
                    (*s).push_back(sentence);
                else if ((*s).size() > 0)                     // одиночный знак ("?!", "...") -
                    (*s)[(*s).size() - 1] += sentence;        // дописываем к предыдущему предложению
                sentence = "";
            }
        }
        process_word(str, v, (*s).size(), d);                    // последнее слово файла (если после него нет знака)
        sentence = trim(sentence);
        if (sentence != "")                                   // последнее предложение без точки
            (*s).push_back(sentence);
        file.close();
    }
    return code;
}

void process_word(string str, vector<participle>* v, int sentence_numb, vector<stress_word>* d)   // Обработка слова
{                                                                         // Если слово - деепричастие, запоминаем его
    string end_of_word = "";
    str = to_lower(str);

    if ((str != "") &&
        (is_perfective_participle(str, end_of_word) ||        // совершенный вид
            is_imperfective_participle(str, end_of_word) ||      // несовершенный вид
            is_known_participle(str, end_of_word) ||             // словарь в коде
            (find_in_dictionary(str, d) != -1)))                 // словарь ударений
    {
        if (end_of_word == "")                                // слово нашлось только в словаре ударений
            end_of_word = guess_end(str);

        int idx = find_elem(str, v);
        if (idx == -1)                                        // слово встретилось впервые
        {
            participle temp;
            temp.word = str;
            temp.end_of_word = end_of_word;
            temp.count = 1;
            temp.sentences.push_back(sentence_numb);
            temp.stress = get_stress(str, d);                 // -1, если ударение неизвестно
            (*v).push_back(temp);
        }
        else                                                  // слово уже было
        {
            (*v)[idx].count++;
            vector<int>& sent = (*v)[idx].sentences;          // ссылка на вектор (номера предложений, где встречается слово)
            if (sent[sent.size() - 1] != sentence_numb)       // одно предложение записываем один раз
                sent.push_back(sentence_numb);
        }
    }
}

string trim(string str)                                       // Удаление пробелов по краям строки
{                                                             // Возвращает строку без лишних пробелов слева/справа
    int begin = 0;
    int end = str.length() - 1;
    while (begin <= end && str[begin] == ' ')
        begin++;
    while (end >= begin && str[end] == ' ')
        end--;
    return str.substr(begin, end - begin + 1);
}

int find_elem(string str, vector<participle>* v)              // Поиск слова в векторе
{                                                             // Возвращает индекс слова, если оно уже есть
    int result = -1;                                          // Иначе возвращает -1
    int length = (*v).size();
    for (int i = 0; i < length; i++)
    {
        if ((*v)[i].word == str)
        {
            result = i;
            break;
        }
    }
    return result;
}

int check_symbol(char ch)                           // Проверка на символ разделения 
{                                                   // Возвращает 0, если символ не разделительный
    int split_str = 0;                              // Возвращает 1, если символ - разделитель
    string split_symbols = " .,!?:;\n\t—–…";        // Возвращает 2, если символ не разделительный, но не требуется для записи 
    string other_symbols = "\"\'()«»";

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

    if (length >= 4)                
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
            else if (((str.rfind("ав") == length - 2) || (str.rfind("яв") == length - 2) ||    // "ав", "яв", "ив", "ев"    
                (str.rfind("ив") == length - 2) || (str.rfind("ев") == length - 2)) &&
                ru_consonants(str[length - 3]) && (str.find('-') == -1) &&                // перед гласной - согласная
                !((str.rfind("лив") == length - 3) && ru_consonants(str[length - 4]) &&   // труслив, сварлив - прилагательные,
                    !((length > 6) && (str.rfind("медлив") == length - 6))) &&             // но помедлив, замедлив - деепричастия
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

            else if (str.rfind("йдя") == length - 3)                                  // "йдя": войдя, взойдя
            {
                checker = true;
                end_of_word = "дя";
            }

        }
    }

    return checker;
}

bool is_imperfective_participle(string str, string& end_of_word)   // Проверка, что слово является несовершенным деепричастием
{                                                                   // возвращает true, если слово несоверш. дееприч.
    bool checker = false;                                           // иначе возвращает false
    int length = str.length();
    string hissing = "жшчщ";                    // шипящие
    string before_ivaya = "бкгхжшчщ";            // буквы, после которых пишется "-ивая"

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
        else if ((str.rfind("дучи") == length - 4) || (str.rfind("ючи") == length - 3) ||
            ((length >= 7) && (str.rfind("дучись") == length - 6)))           // будучи, играючи, крадучись
        {
            checker = true;
            end_of_word = str.substr(length - 2);
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
    else if (str.rfind("увая") == length - 4)                // сдувая, обувая, раздувая - деепричастия
        result = false;
    else if (str.rfind("вая") == length - 3)                // новая, правая, живая (кроме "давая", "знавая", "ставая")
        result = !((str.rfind("давая") == length - 5) ||
            (length >= 6 && str.rfind("знавая") == length - 6) ||
            (length >= 6 && str.rfind("ставая") == length - 6) ||
            (length >= 8 && str.rfind("ривая") == length - 5));   // рассматривая, уговаривая (но не кривая, игривая)
    else if ((str.rfind("окая") == length - 4) || (str.rfind("ёкая") == length - 4) ||
        (str.rfind("атая") == length - 4) || (str.rfind("ятая") == length - 4) || (str.rfind("ячая") == length - 4))
        result = true;                                      // высокая, далёкая, богатая, пятая, горячая
    else if (str.rfind("щая") == length - 3)                // "-щая": причастие (летящая, бегущая) или деепричастие (обещая)
        result = (str.rfind("ущая") == length - 4) || (str.rfind("ющая") == length - 4) ||
        (str.rfind("ящая") == length - 4) ||
        ((str.rfind("ащая") == length - 4) && (str.rfind("ращая") != length - 5)) ||
        (str == "тощая");
    else if ((str.rfind("емая") == length - 4) || (str.rfind("омая") == length - 4))
        result = true;                                      // нескрываемая, знакомая (причастия и прилагательные)
    else if ((str.rfind("имая") == length - 4) && (str[length - 5] != 'н'))
        result = true;                                      // любимая, родимая, незримая (но не принимая, понимая)
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
        string array[SIZE] = { "Ковши", "ковши", "Левши", "левши", "Правши", "правши" };      
        result = check_dictionary(SIZE, array, str);
    }
    else if (variant == 2)                   // Проверка для окончания "ши"
    {
        const int SIZE = 5;
        string array[SIZE] = { "орши", "арши", "ирши", "льши", "нтши" };

        for (int i = 0; i < SIZE; i++)
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
        string array[SIZE] = { "взрыв", "порыв", "обрыв", "разрыв", "прорыв", "срыв",
                            "отрыв", "подрыв", "надрыв", "перерыв", "нарыв",
                            "наплыв", "заплыв", "смыв" };
        result = check_dictionary(SIZE, array, str);
    }
    else if (variant == 4)                  // Исключения для "ивая", "евая"        
    {
        const int SIZE = 11;
        string array[SIZE] = { "живая", "фальшивая", "плешивая", "паршивая", "вшивая", "боевая",
                               "полевая", "ключевая", "дождевая", "грушевая", "межевая" };
        result = check_dictionary(SIZE, array, str);

        if ((length >= 6) && (str.rfind("чивая") == length - 5) && ru_consonants(str[length - 6]))
            result = false;                 // навязчивая, задумчивая (согласная перед "чивая"),
                                            // но не оборачивая, покручивая (гласная перед "чивая")
    }
    else if (variant == 5)                  // Исключения для "ая", "яя": прилагательные, местоимения, существительные
    {
        const int SIZE = 70;
        string array[SIZE] = { "белая", "смелая", "целая", "спелая", "зрелая", "умелая", "милая", "гнилая",
                               "малая", "вялая", "усталая", "старая", "серая", "скорая", "бурая", "хмурая",
                               "сырая", "золотая", "крутая", "святая", "молодая", "седая", "худая", "тупая",
                               "слепая", "скупая", "прямая", "немая", "хромая", "любимая", "тихая", "сухая",
                               "глухая", "плохая", "строгая", "дорогая", "другая", "большая", "хорошая", "синяя",
                               "голая", "весёлая", "удалая", "сарая", "урожая", "попугая", "негодяя", "лентяя",
                               "китая", "николая", "трамвая", "вторая",
                               "случая", "которая", "некоторая", "какая", "такая", "всякая", "никакая", "самая",
                               "любая", "пролитая", "убитая", "забытая", "разбитая", "открытая", "закрытая",
                               "скрытая", "умытая", "надутая" };
        result = check_dictionary(SIZE, array, str);
    }
    else if (variant == 6)                  // Исключения для "ав", "яв", "ив", "ев"
    {
        const int SIZE = 63;
        string array[SIZE] = { "рукав", "состав", "устав", "сплав", "застав", "канав", "забав", "расправ",
                               "облав", "держав", "приправ", "переправ", "лукав", "кровав", "величав",
                               "ярослав", "вячеслав", "святослав", "владислав", "станислав", "мстислав",
                               "ростислав", "мотив", "архив", "актив", "залив", "прилив", "отлив", "разлив",
                               "пролив", "курсив", "массив", "коллектив", "объектив", "детектив", "красив",
                               "ленив", "счастлив", "правдив", "учтив", "игрив", "болтлив", "терпелив",
                               "справедлив", "ревнив", "молчалив", "фальшив", "против", "напротив",
                               "супротив", "позитив", "негатив", "перспектив", "локомотив", "дефектив",
                               "напев", "припев", "запев", "посев", "королев", "нагрев", "перегрев", "неправ" };
        result = check_dictionary(SIZE, array, str);
    }                                                                                               ////
    return result;              // уменьшить функцию или разделить на несколько
}

bool check_dictionary(const int SIZE, string* array, string str)                 // Проверка на слова исключения     
{                                                                                // Возвращает true, если исключение найдено
    bool result = true;                                                          // иначе возвращает false

    for (int i = 0; i < SIZE; i++)
    {
        if (str == array[i])
        {
            result = false;
            break;
        }
    }
    return result;
}

string to_lower(string str)                             // Переведение строки к нижнему регистру
{
    for (int i = 0; i < str.length(); i++)
    {
        unsigned char c = str[i];
        if (c >= 0xC0 && c <= 0xDF)          // А..Я -> а..я (кодировка Windows-1251)
            c = c + 32;
        else if (c == 0xA8)                  // Ё -> ё
            c = 0xB8;
        else
            c = tolower(c);                  // латинские буквы
        str[i] = c;
    }
    return str;
}

bool is_known_participle(string str, string& end_of_word)   // Проверка по словарю деепричастий,
{                                                          // которые нельзя найти по окончанию
    bool checker = false;                                  // возвращает true, если слово есть в словаре

    // Несовершенный вид: согласная + "я" / "а", а также "-ея", "-оя", "-ияя", "-учи"
    const int SIZE_1 = 108;
    string imperfective[SIZE_1] = {
        "глядя", "видя", "ведя", "идя", "сидя", "ходя", "водя", "проходя", "переводя", "уводя",
        "неся", "нося", "прося", "вися", "везя", "грызя",
        "любя", "живя", "ловя", "готовя", "ставя", "правя", "веря",
        "говоря", "смотря", "творя", "споря", "куря", "варя",
        "звеня", "храня", "гоня", "клоня", "звоня",
        "шевеля", "деля", "хваля", "веля", "моля",
        "кряхтя", "вертя", "шелестя", "крутя", "платя", "летя", "свистя", "блестя", "грустя",
        "хрустя", "золотя", "слепя", "терпя", "топя", "кипя", "сопя", "храпя", "хрипя", "скрипя",
        "держа", "дрожа", "лёжа", "лежа", "дыша", "спеша", "слыша", "крича", "шепча", "бормоча",
        "ворча", "топча", "стуча", "звуча", "уча", "леча", "мча", "трепеща", "плеща",
        "белея", "синея", "краснея", "зеленея", "желтея", "темнея", "светлея", "бледнея",
        "немея", "робея", "жалея", "слабея", "имея", "умея", "сея", "вея", "лелея",
        "стоя", "кроя", "сияя", "охая", "будучи", "позванивая", "петляя", "фыркая", "хватая", "гуляя", "стреляя", "сверкая", "играя", "мечтая" };

    // Совершенный вид: "-йдя", "-едя", "-еся" (от глаголов на "-ти") и короткие формы
    const int SIZE_2 = 25;
    string perfective[SIZE_2] = {
        "выйдя", "найдя", "придя", "уйдя", "войдя", "зайдя", "пройдя", "сойдя", "подойдя",
        "перейдя", "обойдя", "отойдя", "дойдя", "приведя", "уведя", "заведя", "принеся",
        "унеся", "занеся", "вынеся", "дав", "сев", "взяв", "сняв", "сжав" };

    if ((str.length() >= 3) &&
        (!check_dictionary(SIZE_1, imperfective, str) || !check_dictionary(SIZE_2, perfective, str)))
    {
        checker = true;
        end_of_word = str.substr(str.length() - 2);        // две последние буквы: "дя", "ря", "ча"
    }

    return checker;
}

int load_dictionary(string file_name, vector<stress_word>* d)   // Загрузка словаря ударений
{                                                               // Формат строки: "заду'мав" (апостроф после ударной)
    ifstream file(file_name);                                   // Возвращает количество слов или -1, если файла нет
    if (!file.is_open())
        return -1;

    string line;
    while (getline(file, line))
    {
        if (line != "" && line[line.length() - 1] == '\r')     // файл мог быть сохранен с "\r\n"
            line.erase(line.length() - 1);
        line = to_lower(trim(line));
        int pos = line.find('\'');
        if (pos > 0)                                            // строки без ударения пропускаем
        {
            stress_word temp;
            temp.stress = pos - 1;                              // ударная буква стоит перед апострофом
            temp.word = line.erase(pos, 1);
            (*d).push_back(temp);
        }
    }
    file.close();

    sort((*d).begin(), (*d).end(), compare_words);              // по алфавиту - для бинарного поиска
    return (*d).size();
}

bool compare_words(stress_word a, stress_word b)                // Правило сортировки словаря
{
    return a.word < b.word;
}

int find_in_dictionary(string str, vector<stress_word>* d)      // Бинарный поиск слова в словаре
{                                                               // Возвращает номер ударной буквы или -1
    int result = -1;
    int left = 0;
    int right = (*d).size() - 1;
    while (left <= right)
    {
        int middle = (left + right) / 2;
        if ((*d)[middle].word == str)
        {
            result = (*d)[middle].stress;
            break;
        }
        else if ((*d)[middle].word < str)                       // искомое слово дальше по алфавиту
            left = middle + 1;
        else
            right = middle - 1;
    }
    return result;
}

int get_stress(string str, vector<stress_word>* d)              // Ударение слова: словарь важнее правил
{                                                               // Возвращает номер ударной буквы или -1
    int result = find_in_dictionary(str, d);
    if (result == -1)
        result = stress_by_rule(str);
    return result;
}

int stress_by_rule(string str)                                  // Ударение по правилам суффиксов
{                                                               // Возвращает номер ударной буквы или -1
    int result = -1;

    const int SIZE = 5;
    string ir[SIZE] = { "ируя", "ируясь", "ировав", "ировавши", "ировавшись" };                 // регули'руя
    string stv[SIZE] = { "ствуя", "ствуясь", "ствовав", "ствовавши", "ствовавшись" };        // путеше'ствуя
    string nich[SIZE] = { "ничая", "ничаясь", "ничав", "ничавши", "ничавшись" };             // осторо'жничая
    const int SIZE_2 = 10;
    string yva[SIZE_2] = { "ывая", "ивая", "ываясь", "иваясь", "ывав", "ивав",               // рассма'тривая
                           "ывавши", "ивавши", "ывавшись", "ивавшись" };

    int pos_yo = str.find('ё');
    int pos_ir = suffix_position(str, ir, SIZE);
    int pos_yva = suffix_position(str, yva, SIZE_2);
    int pos_stv = suffix_position(str, stv, SIZE);
    int pos_nich = suffix_position(str, nich, SIZE);

    if (pos_yo != -1)                                           // "ё" всегда ударная
        result = pos_yo;
    else if (pos_ir != -1)                                      // ударение на "и" в "-иру-/-иров-"
        result = pos_ir;
    else if (pos_yva > 1)                                       // гласная перед "-ыва-/-ива-"
        result = last_vowel_before(str, pos_yva);
    else if (pos_stv != -1)                                     // гласная перед "-ству-/-ствов-"
        result = last_vowel_before(str, pos_stv);
    else if (pos_nich != -1)                                    // гласная перед "-нича-"
        result = last_vowel_before(str, pos_nich);
    else if ((str.find("вы") == 0) &&                           // совершенный вид с "вы-": вы'писав
        (ends_with(str, "в") || ends_with(str, "вши") || ends_with(str, "вшись")))
        result = 1;

    return result;
}

int suffix_position(string str, string* array, int size)       // Если слово оканчивается на суффикс из списка,
{                                                               // возвращает номер буквы, с которой он начинается
    int result = -1;                                            // Иначе возвращает -1
    for (int i = 0; i < size; i++)
    {
        if (ends_with(str, array[i]))
        {
            result = str.length() - array[i].length();
            break;
        }
    }
    return result;
}

int last_vowel_before(string str, int pos)                      // Номер последней гласной перед позицией pos
{                                                               // (-1, если гласных нет)
    int result = -1;
    for (int i = pos - 1; i >= 0; i--)
    {
        if (is_vowel(str[i]))
        {
            result = i;
            break;
        }
    }
    return result;
}

bool is_vowel(char symb)                                        // Проверка, что символ - гласная буква
{
    string vowels = "аеёиоуыэюя";
    return vowels.find(symb) != -1;
}

string guess_end(string str)                                    // Окончание для слова, найденного только в словаре
{
    string result = str.substr(str.length() - 2);               // по умолчанию две последние буквы: "ав", "ая"
    if (ends_with(str, "вшись"))
        result = "вшись";
    else if (ends_with(str, "шись"))
        result = "шись";
    else if (ends_with(str, "вши"))
        result = "вши";
    else if (ends_with(str, "ясь") || ends_with(str, "ась"))
        result = str.substr(str.length() - 3);
    return result;
}
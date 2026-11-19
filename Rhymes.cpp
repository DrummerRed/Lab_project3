#include "header.h"

string phonetic_algorithm(participle elem, int end_length) // Фонетический алгоритм
{
    // Все созвучные гласные/согласные приводим к одной конкретной
    string str = elem.word;
    int indent = end_length - elem.end_of_word.length(); // Отступ
    int index = str.find(elem.end_of_word);
    for (int i = index - indent; i < index; i++)
    {
        if (str[i] == 'ё')
            str[i] = 'о';
        else if (str[i] == 'я')
            str[i] = 'а';
        else if (str[i] == 'ы')
            str[i] = 'и';
        else if (str[i] == 'к')
            str[i] == 'г';
    }
    return str;
}

int count_rhymes(vector<participle>* v) // Подсчет рифмующихся пар
{
    int count = 0;
    int length = (*v).size();
    for (int i = 0; i < length - 1; i++)
        for (int j = i + 1; j < length; j++)
            if (search_rhyms((*v)[i], (*v)[j]))
                count++;
    return count;
}

bool save_report(string report_file, vector<participle>* v, vector<string>* s)  // Запись отчета в текстовый файл
{                                                                               // Возвращает true при успешной записи отчета
    ofstream f(report_file);                                                    // при ошибке возвращает false
    if (!f.is_open())
        return false;

    const int WIDTH = 72;               // ширина отчета в символах
    int length = (*v).size();
    int pairs = count_rhymes(v);
    f << left;

    f << "Программа по поиску рифм в деепричастиях.\n";

    // Найденные деепричастия
    f << "1. Найденные деепричастия\n";
    if (length == 0)
    {
        write_line(f, '-', WIDTH);
        f << "   Деепричастия в тексте не найдены.\n";
    }
    else
    {
        f << "  " << setw(6) << "№" << setw(20) << "Деепричастие" << "Окончание\n";
        write_line(f, '-', WIDTH);
        for (int i = 0; i < length; i++)
            f << "  " << setw(6) << i + 1 << setw(20) << (*v)[i].word << (*v)[i].end_of_word << "\n";
    }

    //Найденные пары рифм 
    vector<bool> in_pair(length, false); // входит ли слово хотя бы в одну пару
    f << "\n\n2. Найденные пары рифм\n";
    write_line(f, '-', WIDTH);
    if (pairs == 0)
        f << "   Рифмующиеся пары не найдены.\n";
    else
    {
        int numb = 0;
        for (int i = 0; i < length - 1; i++)
            for (int j = i + 1; j < length; j++)
                if (search_rhyms((*v)[i], (*v)[j]))
                {
                    in_pair[i] = true;// отмечаем оба слова пары
                    in_pair[j] = true;
                    numb++;
                    f << "  " << setw(7) << to_string(numb) + "." << setw(20) << (*v)[i].word << "-   " << (*v)[j].word << "\n";
                }
    }

    //Количество появлений слов из пар 
    f << "\n\n3. Количество появлений\n";
    write_line(f, '-', WIDTH);
    if (pairs == 0)
        f << "   Нет слов, образующих пары рифм.\n";
    else
    {
        for (int i = 0; i < length; i++)
        {
            if (in_pair[i]) // только слова из пункта 2
            {
                participle& e = (*v)[i];
                f << e.word << ": количество появлений: " << e.count << ", номера предложений: ";
                for (int k = 0; k < e.sentences.size(); k++)
                {
                    if (k > 0)
                        f << ", ";
                    f << e.sentences[k] + 1; // нумерация предложений с 1
                }
                f << "\n";
            }
        }
    }

    // Предложения с данными словами 
    f << "\n\n4. Предложения\n";
    write_line(f, '-', WIDTH);
    if (length == 0)
        f << "   Нет предложений с деепричастиями.\n";
    else
    {
        int count_s = (*s).size();
        vector<string> marks(count_s);// какие деепричастия есть в каждом предложении
        for (int i = 0; i < length; i++)
        {
            participle& e = (*v)[i];
            for (int k = 0; k < e.sentences.size(); k++)
            {
                int numb = e.sentences[k];
                if (numb < count_s)
                {
                    if (marks[numb] != "")
                        marks[numb] += ", ";
                    marks[numb] += e.word;
                }
            }
        }
        for (int k = 0; k < count_s; k++) // предложения по порядку, каждое один раз
        {
            if (marks[k] != "")
            {
                f << "Предложение " << k + 1 << " (" << marks[k] << "):\n";
                write_wrapped(f, (*s)[k], 6);
                f << "\n";
            }
        }
    }
    f.close();
    return true;
}

void write_line(ofstream& f, char symb, int length) // Разделительная линия в отчете
{
    f << string(length, symb) << "\n";
}

void write_wrapped(ofstream& f, string text, int indent) // Запись текста с переносом по словам
{                                                               
    const int WIDTH = 72;
    int max_length = WIDTH - indent;
    string line = "";
    string word = "";
    text += ' ';
    for (int i = 0; i < text.length(); i++)
    {
        if (text[i] != ' ')
            word += text[i];
        else if (word != "")
        {
            if (line != "" && line.length() + 1 + word.length() > max_length)
            {
                f << string(indent, ' ') << line << "\n";// строка заполнена - переносим
                line = "";
            }
            if (line != "")
                line += ' ';
            line += word;
            word = "";
        }
    }
    if (line != "")
        f << string(indent, ' ') << line << "\n";
}

bool search_rhyms(participle elem_1, participle elem_2)     // Поиск рифмы между парой слов
{                                                           // Возвращает true, если рифма обнаружена
    bool result = false;                                    // если рифма не найдена возвращает false
    if (elem_1.end_of_word == elem_2.end_of_word)
    {
        string end = elem_1.end_of_word;

        int default_1 = default_stress(elem_1.word);             
        int default_2 = default_stress(elem_2.word);
        bool special_1 = (elem_1.stress != -1) && (elem_1.stress != default_1);  
        bool special_2 = (elem_2.stress != -1) && (elem_2.stress != default_2);

        if (special_1 || special_2)// хотя бы у одного слова ударение особое, сравниваем от ударных гласных
        {                           
            if (!special_1)
                elem_1.stress = default_1;
            if (!special_2)
                elem_2.stress = default_2;
            return stress_rhyme(elem_1, elem_2);
        }

        if (end == "вшись" || end == "вши")            
            result = rhyme_with_ends_1(elem_1, elem_2, 2); 
        else if (end == "шись")
            result = rhyme_with_ends_1(elem_1, elem_2, 2);
        else if (end == "ши")
            result = rhyme_with_ends_1(elem_1, elem_2, 2, 2);                  

        else if (end == "ув" || end == "ыв" || end == "ав" || end == "яв" ||
            end == "ив" || end == "ев")
            result = rhyme_with_ends_1(elem_1, elem_2, 1);
        else if (end == "ая" || end == "яя" || end == "уя" || end == "юя")
            result = rhyme_with_ends_1(elem_1, elem_2, 1);

        else if (end == "ясь" || end == "ась")              
            result = rhyme_with_ends_2(elem_1, elem_2);                 

        else if (end == "ывая" || end == "ивая" || end == "евая")          
            result = rhyme_with_ends_1(elem_1, elem_2, 2);              
        else if (end.length() == 2)  // слова из словаря
            result = rhyme_with_ends_1(elem_1, elem_2, 0);
    }
    return result;
}

// Разделим функции поиска рифм под каждое из окончаний деепричастий
// Окончание 1 - "вшись", "шись", "вши"
bool rhyme_with_ends_1(participle elem_1, participle elem_2, int indent, int end_variant)// Сравнение слов на рифму по окончаниям
{                                                                                       // Возвращает true, если окончания дают потенциальную рифму                                 
    bool result = false;                                                                // иначе возвращает false
    string end = elem_1.end_of_word;
    int end_length = end.length() + indent;

    if (indent > 0)     // Выпоняем фонетический алгоритм в месте отступа
    {
        elem_1.word = phonetic_algorithm(elem_1, end_length);
        elem_2.word = phonetic_algorithm(elem_2, end_length);
    }
    int idx_1 = elem_1.word.find(end) - indent;
    int idx_2 = elem_2.word.find(end) - indent;
    if ((idx_1 < 0) || (idx_2 < 0))     // в слове не хватает букв перед окончанием
        return false;       // ("кивая": перед "ивая" только одна буква)

    string sub_string_1 = elem_1.word.substr(idx_1, end_length);
    string sub_string_2 = elem_2.word.substr(idx_2, end_length);
    if (sub_string_1 == sub_string_2)
        result = true;
    else
    {
        if (end_variant == 2)                          
        {
            if (replacement_algorithm(sub_string_1, sub_string_2, 2))
                result = true;
        }
    }

    return result;
}


// Окончания ясь/ась: сколько букв перед окончанием сравнивать,
// зависит от буквы, которая стоит перед "ясь"/"ась"
int sya_indent(string word, string end)
{
    int length = word.length();
    char before = word[length - end.length() - 1]; // буква перед "ясь"/"ась"
    int indent = 0; // смеясь, боясь: сравниваем "ясь"

    if ((length >= 6) && ((word.rfind("иваясь") == length - 6) || (word.rfind("ываясь") == length - 6)))
        indent = 4;                                           
    else if (before == 'а' || before == 'я')
        indent = 2;                                          
    else if (ru_consonants(before) || is_vowel(before))
        indent = 1;                                           
    return indent;
}

bool rhyme_with_ends_2(participle elem_1, participle elem_2)      // Сравнение слов на рифму по окончаниям на ась/ясь
{                                                                 // Возвращает true, если окончания дают потенциальную рифму
    bool result = false;                                          // иначе возвращает false
    int indent_1 = sya_indent(elem_1.word, elem_1.end_of_word);
    int indent_2 = sya_indent(elem_2.word, elem_2.end_of_word);

    if (indent_1 == indent_2)                                
        result = rhyme_with_ends_1(elem_1, elem_2, indent_1);

    return result;
}

bool replacement_algorithm(string end_1, string end_2, int reverse_position) // Алгоритм замены буквы для проверки равенства окончаний 
{                                         // Возвращает true, если из 1-го окончания можно получить 2-е заменив 1 букву (на позиции reverse_position)                               
    bool result = true;                   // иначе false                                                
    if (end_1.length() == end_2.length())
    {
        int true_pos = end_1.length() - reverse_position;
        for (int i = 0; i < end_1.length(); i++)
        {
            if (end_1[i] != end_2[i])
            {
                if (i != true_pos)
                {
                    result = false;
                    break;
                }
            }
        }
    }
    return result;
}

int default_stress(string str)      // Ударение "по умолчанию" для деепричастий
{                                                               
    if (ends_with(str, "сь"))                                   
        str.erase(str.length() - 2);
    int length = str.length();
    int result;

    if (ends_with(str, "вши"))                                  
        result = last_vowel_before(str, length - 3);
    else if (ends_with(str, "ши"))                             
        result = last_vowel_before(str, length - 2);
    else if (ends_with(str, "в"))                               
        result = last_vowel_before(str, length - 1);
    else if (is_vowel(str[length - 1]))                        
    {
        if ((length > 1) && is_vowel(str[length - 2]))         
            result = length - 2;
        else                                                    
            result = length - 1;
    }
    else
        result = last_vowel_before(str, length);
    return result;
}

bool stress_rhyme(participle elem_1, participle elem_2)         // Рифма по ударению. Сравниваем куски от ударной гласной до конца
{                                                               
    string part_1 = normalize_vowels(elem_1.word.substr(elem_1.stress));   
    string part_2 = normalize_vowels(elem_2.word.substr(elem_2.stress));   
    return part_1 == part_2;
}

string normalize_vowels(string str)// Приведение созвучных гласных к одной
{
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] == 'ё')
            str[i] = 'о';
        else if (str[i] == 'я')
            str[i] = 'а';
        else if (str[i] == 'ю')
            str[i] = 'у';
        else if (str[i] == 'ы')
            str[i] = 'и';
    }
    return str;
}
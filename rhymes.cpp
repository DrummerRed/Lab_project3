#include "header.h"

string phonetic_algorithm(participle elem, int end_length)               // Фонетический алгоритм
{
    // Все созвучные гласные/согласные приводим к одной конкретной
    string str = elem.word;
    int indent = end_length - elem.end_of_word.length();             // Отступ
    int index = str.find(elem.end_of_word);
    for (int i=index-indent; i<index; i++)
    {
        if (str[i] == 'ё')
            str[i] = 'о';
        else if (str[i] == 'я')
            str[i] = 'а';
        else if (str[i] == 'ы')
            str[i] = 'ы';
        else if (str[i] == 'к')
            str[i] == 'г';

        // д-т?
        // г-к-х?
    }
    return str;
}

void rhymes_output(vector<participle>* v)                      // Вывод всех найденных рифм
{
    int length = (*v).size();
    if (length > 1)
    {
        for (int i=0; i<length-1; i++)
        {
            participle elem_1 = (*v)[i];
            for (int j=i+1; j<length; j++)
            {
                participle elem_2 = (*v)[j];
                if (search_rhyms(elem_1, elem_2))
                    cout << elem_1.word << " - " << elem_2.word << endl;
            }
        }
    }

}

bool search_rhyms(participle elem_1, participle elem_2)                     // Поиск рифмы между парой слов
{
    bool result = false;
    if (elem_1.end_of_word == elem_2.end_of_word)
    {
        if (elem_1.end_of_word == "вшись")
            result = rhyme_with_ends_1(elem_1, elem_2, 1);
        else if (elem_1.end_of_word == "шись")
            result = rhyme_with_ends_1(elem_1, elem_2, 2);
        else if (elem_1.end_of_word == "вши")
            result = rhyme_with_ends_1(elem_1, elem_2, 1);
    }
    return result;
}

// Разделим функции поиска рифм под каждое из окончаний деепричастий
// Окончание 1 - "вшись", "шись", "вши"

bool rhyme_with_ends_1(participle elem_1, participle elem_2, int indent)         // Сравнение слов на рифму по окончанию "вшись"
{                                                                   // Возвращает true, если потенциальная рифма найдена
    bool result = false;                                            // Если не найдена, возвращает false
    string end = elem_1.end_of_word;
    int end_length = end.length() + indent;
    if (indent > 0)
    {
        elem_1.word = phonetic_algorithm(elem_1, end_length);       
        elem_2.word = phonetic_algorithm(elem_2, end_length);
    }       
    int idx_1 = elem_1.word.find(end) - indent;                                       
    int idx_2 = elem_2.word.find(end) - indent;                                       

    if (elem_1.word.substr(idx_1, end_length) == elem_2.word.substr(idx_2, end_length))
    {
        // cout << elem_1.word.substr(idx_1, end_length) << endl;                  ///////delete
        // cout <<elem_1.word<<endl;//
        // cout <<elem_2.word<<endl;//
        result = true;
    }
    return result;
}
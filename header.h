#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <conio.h>
#include <cctype>                       // tolower
#include <vector>
// #include <cstdio>

using namespace std;

const int KEY_ESC = 27;
const int KEY_ENTER = 13;
const int KEY_BACKSPACE = 8;

struct participle
{
    string word;                            // слово
    string end_of_word;                     // окончание
    // string word_after_transform;            // слово после фонетического преобразования
    // int sentence_numb = 0;               // номер предложения
};

void menu();                                                     // Главное меню программы
void print_menu(string input_file);                              // Вывод главного меню
int get_menu_key();                                              // Выбор пункта меню 
void search_rhymes_menu(string input_file);                      // Раздел меню "поиск рифм"

void preparation_and_reading(string file_name);               // Создание структур данных для запис слов, и чтение файла
int read_file(string file_name, vector<participle>* v);          // Чтение файла 
bool unique_elem(string str, vector<participle>* v);             // Проверка на наличие в векторе указанного слова
int check_symbol(char ch);                                       // Проверка на символ разделения 
bool is_perfective_participle(string str, string& end_of_word);  // Проверка, что слово является совершенным деепричастием
bool ru_consonants(char symb);                                   // Проверка, что символ является согласной буквой
bool check_exceptions(int variant, string str);                    // Проверка строки на исключения
bool check_dictionary(const int SIZE, string* array, string str);                   // Проверка на слова исключения 
void choose_input_file(string& input_file);                      // Считывание имени входного файла
void wait_key();                                                 // Ожидание нажатия клавиши
bool input_file_name(string& name);                              // Посимвольный ввод имени файла
string to_lower(string str);                                     // Приведение строки к нижнему регистру

string phonetic_algorithm(participle elem, int end_length);      // Фонетический алгоритм
void rhymes_output(vector<participle>* v);                      // Вывод всех найденных рифм
bool search_rhyms(participle elem_1, participle elem_2);                     // Поиск рифмы между парой слов
bool rhyme_with_ends_1(participle elem_1, participle elem_2, int indent);         // Сравнение слов на рифму по окончанию "вшись"
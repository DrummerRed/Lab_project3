#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <conio.h>
#include <cctype>                       
#include <vector>
#include <algorithm>                    
#include <iomanip>                      
// #include <cstdio>

using namespace std;

const int KEY_ESC = 27;
const int KEY_ENTER = 13;
const int KEY_BACKSPACE = 8;
const string DICT_FILE = "stress_dict.txt"; // словарь ударений 

struct stress_word          // слово из словаря ударений
{
    string word;                            
    int stress;                             
};

struct participle
{
    string word;                // слово
    string end_of_word;         // окончание
    int count;                  // сколько раз слово встретилось в тексте
    vector<int> sentences;      // номера предложений, где встречается слово (с 0)
    int stress;                 // номер ударной буквы, -1 - ударение неизвестно
};

void menu(); // Главное меню программы
void print_menu(string input_file, int dict_size); // Вывод главного меню
int get_menu_key(); // Выбор пункта меню 
void search_rhymes_menu(string input_file, vector<stress_word>* d); // Раздел меню "поиск рифм"

int read_file(string file_name, vector<participle>* v, vector<string>* s, vector<stress_word>* d); // Чтение файла: деепричастия и предложения
void process_word(string str, vector<participle>* v, int sentence_numb, vector<stress_word>* d); // Обработка одного слова текста
string trim(string str); // Удаление пробелов по краям строки
int find_elem(string str, vector<participle>* v); // Поиск слова в векторе (индекс или -1)
int check_symbol(char ch); // Проверка на символ разделения 
bool is_perfective_participle(string str, string& end_of_word); // Проверка, что слово является совершенным деепричастием
bool is_imperfective_participle(string str, string& end_of_word); // Проверка, что слово является несовершенным деепричастием
bool is_known_participle(string str, string& end_of_word); // Проверка по словарю деепричастий
bool is_adjective(string str); // Проверка, что слово на "ая"/"яя" является прилагательным
bool ru_consonants(char symb); // Проверка, что символ является согласной буквой
bool check_exceptions(int variant, string str); // Проверка строки на исключения
bool check_dictionary(const int SIZE, string* array, string str); // Проверка на слова исключения 
void choose_input_file(string& input_file); // Считывание имени входного файла
void wait_key(); // Ожидание нажатия клавиши
bool input_file_name(string& name); // Посимвольный ввод имени файла
void add_txt(string& name);  // Добавление ".txt", если пользователь его не ввел
bool ends_with(string str, string ending); // Проверка, что строка оканчивается на ending
bool input_report_name(string input_file, string& report_file); // Ввод имени файла для отчета
string to_lower(string str);                        // Приведение строки к нижнему регистру

int load_dictionary(string file_name, vector<stress_word>* d); // Загрузка словаря ударений (кол-во слов или -1)
bool compare_words(stress_word a, stress_word b);  // Правило сортировки словаря (по алфавиту)
int find_in_dictionary(string str, vector<stress_word>* d); // Бинарный поиск слова (ударная буква или -1)
int stress_by_rule(string str); // Ударение по правилам суффиксов (или -1)
int get_stress(string str, vector<stress_word>* d); // Ударение: сначала словарь, потом правила
int suffix_position(string str, string* array, int size); // Начало суффикса из списка (или -1)
int last_vowel_before(string str, int pos); // Последняя гласная перед позицией pos
bool is_vowel(char symb); // Проверка, что символ - гласная буква
string guess_end(string str); // Окончание для слова, найденного только в словаре
int default_stress(string str); // Ударение "по умолчанию" (перед окончанием)
bool stress_rhyme(participle elem_1, participle elem_2); // Рифма по ударению (от ударной гласной до конца)
string normalize_vowels(string str); // Приведение созвучных гласных к одной

string phonetic_algorithm(participle elem, int end_length); // Фонетический алгоритм
int count_rhymes(vector<participle>* v); // Подсчет рифмующихся пар
bool save_report(string report_file, vector<participle>* v, vector<string>* s); // Запись отчета в текстовый файл
void write_line(ofstream& f, char symb, int length); // Разделительная линия в отчете
void write_wrapped(ofstream& f, string text, int indent); // Запись текста с переносом строк
bool search_rhyms(participle elem_1, participle elem_2); // Поиск рифмы между парой слов
bool rhyme_with_ends_1(participle elem_1, participle elem_2, int indent, int end_variant = 1); // Сравнение слов на рифму по окончанию "вшись"
bool rhyme_with_ends_2(participle elem_1, participle elem_2); // Сравнение слов на "ясь"/"ась"
int sya_indent(string word, string end); // Сколько букв сравнивать перед "ясь"/"ась"
bool replacement_algorithm(string end_1, string end_2, int reverse_position); // Алгоритм замены буквы для проверки равенства окончаний

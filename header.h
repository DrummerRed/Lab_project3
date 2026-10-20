#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <conio.h>
// #include <cstdio>

using namespace std;

const int KEY_ESC = 27;
const int KEY_ENTER = 13;
const int KEY_BACKSPACE = 8;
const int MAX_WORDS = 1000;                     ///???

void menu();                                                     // Главное меню программы
void print_menu(string input_file);                              // Вывод главного меню
int get_menu_key();                                              // Выбор пункта меню 

int read_file(string file_name);                                 // Чтение файла 
int check_symbol(char ch);                                       // Проверка на символ разделения 
bool is_perfective_participle(string str);                       // Проверка, что слово является совершенным деепричастием
bool ru_consonants(char symb);                                   // Проверка, что символ является согласной буквой
bool check_exceptions(int variant, string str);                    // Проверка строки на исключения
bool check_dictionary(const int SIZE, string* array, string str);                   // Проверка на слова исключения 
void choose_input_file(string& input_file);                      // Считывание имени входного файла
void wait_key();                                                 // Ожидание нажатия клавиши
bool input_file_name(string& name);                              // Посимвольный ввод имени файла
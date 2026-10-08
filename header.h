#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
// #include <cstdio>

using namespace std;

int read_file(string file_name);                                 // Чтение файла 
int check_symbol(char ch);                                       // Проверка на символ разделения 
bool is_perfective_participle(string str);                       // Проверка, что слово является совершенным деепричастием
bool ru_consonants(char symb);                                   // Проверка, что символ является согласной буквой
bool check_exceptions(int variant, string str);                    // Проверка строки на исключения
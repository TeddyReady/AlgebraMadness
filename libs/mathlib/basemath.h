#ifndef BASEMATH_H
#define BASEMATH_H
#include <algorithm>
#include <iostream>
#include <string.h>
#include <limits>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <cmath>
#include <map>

//Number decomposition to simple
std::vector<std::pair<int, int>> decompositionToSimple(int num);

//Prime check
bool isPrime(int num);

//Great Common Divisor
int GCD(int a, int b);

//Little Common Multiply
int LCM(int a, int b);

//Faster binary power
long fastPower(long num, long deg);

//Faster binary module power
long modulePower(long num, long deg, int module);

// Num on module
int toMod(int num, int module);

// Convert size_t number to binary num with format
std::string to2(size_t dec, int format = 0);

int findMax2Degree(size_t number);

int countOfSymbol(const std::string &str, char symbol);

#endif // BASEMATH_H

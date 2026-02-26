#include <iostream>
#include <windows.h>
#include "Country.hpp"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    std::cout << "Создание объектов с динамическими полями" << std::endl;

    std::vector<std::string> citiesA = { "a", "b", "g" };
    Country countryA("A", "a", 150, citiesA);

    std::cout << "\nСтрана A:" << std::endl;
    countryA.displayInfo();

    std::vector<std::string> citiesB = { "B", "b", "M" };
    Country countryB("B", "B", 100, citiesB);

    std::cout << "\nСтрана B:" << std::endl;
    countryB.displayInfo();


    std::cout << "\n--- Конструктор копирования ---" << std::endl;
    Country countryC = countryA;
    std::cout << "Копия страны A:" << std::endl;
    countryC.displayInfo();

    std::cout << "\n--- Оператор присваивания ---" << std::endl;
    Country countryD;
    countryD = countryB;
    countryD.displayInfo();


    Country resultPlus = countryA + countryB;
    std::cout << "\nРезультат A + B:" << std::endl;
    resultPlus.displayInfo();


    Country resultMultiply = countryA * countryB;
    std::cout << "\nРезультат A * B:" << std::endl;
    resultMultiply.displayInfo();


    std::cout << "\nИзменяем оригинал A (добавляем город 'X'):\n" << std::endl;
    countryA.addCity("X");
    std::cout << "Оригинал A:" << std::endl;
    countryA.displayInfo();
    std::cout << "\nКопия C:\n" << std::endl;
    countryC.displayInfo();

    std::cout << "----------------------------------------" << std::endl;

    return 0;
}
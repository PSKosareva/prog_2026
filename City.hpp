#ifndef CITY_H
#define CITY_H

#include <iostream>
#include <string>

struct City {
    std::string name;
    long long population;
    double availableMoney;
    bool isCapital;

    // Конструкторы
    City();
    City(std::string n, long long pop, double money = 0.0, bool capital = false);
    City(const City& other);

    // Оператор присваивания
    City& operator=(const City& other);

    // Деструктор
    ~City();

    // Методы
    void displayInfo() const;
    void addMoney(double amount);
    bool operator==(const City& other) const;
};

#endif
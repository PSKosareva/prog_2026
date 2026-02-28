#ifndef BUDGET_H
#define BUDGET_H

#include <iostream>

class Budget {
private:
    double totalMoney;        // Общее количество денег в стране
    double capitalPriority;    // Коэффициент приоритета для столицы (k)
    double minThreshold;       // Минимальный порог (Mp)

public:
    // Конструкторы
    Budget();
    Budget(double money, double priority, double threshold = 1000.0);
    Budget(const Budget& other);

    // Оператор присваивания
    Budget& operator=(const Budget& other);

    // Деструктор
    ~Budget();

    // Геттеры
    double getTotalMoney() const;
    double getCapitalPriority() const;
    double getMinThreshold() const;

    // Сеттеры
    void setTotalMoney(double money);
    void setCapitalPriority(double priority);
    void setMinThreshold(double threshold);

    // Методы
    void changeMoney(double delta); // Изменение на X денег
    void displayInfo() const;

    // Дружественная функция для расчёта распределения
    friend double calculateCityShare(const Budget& budget, long long cityPopulation,
        long long totalPopulation, bool isCapital);
};

#endif
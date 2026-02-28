#ifndef COUNTRY_H
#define COUNTRY_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "Budget.hpp"
#include "City.hpp"

class Country {
private:
    std::string name;
    std::string capital;
    std::string foundationDate;
    double area;
    long long population;
    std::vector<City> cities;
    std::unique_ptr<Budget> budget; // Бюджет страны

public:
    // Конструкторы
    Country();
    Country(std::string n, std::string c, std::string date, double a, long long pop, std::vector<City> cityList);
    Country(const Country& other);

    // Операторы
    Country& operator=(const Country& other);
    ~Country();

    // Операторы из прошлой лабы
    Country operator+(const Country& other) const;
    Country& operator+=(const Country& other);
    Country operator*(const Country& other) const;

    // Геттеры
    std::string getName() const;
    std::string getCapital() const;
    std::string getFoundationDate() const;
    double getArea() const;
    long long getPopulation() const;
    std::vector<City> getCities() const;
    Budget getBudget() const;

    // Сеттеры
    void setName(std::string n);
    void setCapital(std::string c);
    void setFoundationDate(std::string date);
    void setArea(double a);
    void setPopulation(long long pop);
    void setBudget(const Budget& b);

    // Методы для работы с городами
    void addCity(const City& city);
    void removeCity(const std::string& cityName);
    City* findCity(const std::string& cityName);
    void updateCityPopulation(const std::string& cityName, long long newPopulation);

    // Новые методы
    void joinNewTerritory(const Country& territory); // Присоединение новой территории
    void changePopulation(long long delta); // Изменение населения
    void distributeBudget(double moneyChange); // Распределение бюджета

    long long calculateTotalPopulation() const; // Подсчёт общего населения
    void displayInfo() const; // Вывод информации

    // Метод для демонстрации работы библиотеки
    void demonstrateBudgetDistribution();
};

#endif
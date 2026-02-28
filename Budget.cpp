#include "Budget.hpp"
#include <cmath>

Budget::Budget() : totalMoney(0.0), capitalPriority(1.5), minThreshold(1000.0) {
    std::cout << "  [Budget] конструктор по умолчанию" << std::endl;
}

Budget::Budget(double money, double priority, double threshold)
    : totalMoney(money), capitalPriority(priority), minThreshold(threshold) {
    std::cout << "  [Budget] конструктор с параметрами: " << money << " млн. руб., k=" << priority << std::endl;
}

Budget::Budget(const Budget& other)
    : totalMoney(other.totalMoney), capitalPriority(other.capitalPriority), minThreshold(other.minThreshold) {
    std::cout << "  [Budget] конструктор копирования" << std::endl;
}

Budget& Budget::operator=(const Budget& other) {
    std::cout << "  [Budget] оператор присваивания" << std::endl;
    if (this != &other) {
        totalMoney = other.totalMoney;
        capitalPriority = other.capitalPriority;
        minThreshold = other.minThreshold;
    }
    return *this;
}

Budget::~Budget() {
    std::cout << "  [Budget] деструктор" << std::endl;
}

double Budget::getTotalMoney() const {
    return totalMoney;
}

double Budget::getCapitalPriority() const {
    return capitalPriority;
}

double Budget::getMinThreshold() const {
    return minThreshold;
}

void Budget::setTotalMoney(double money) {
    totalMoney = money;
}

void Budget::setCapitalPriority(double priority) {
    capitalPriority = priority;
}

void Budget::setMinThreshold(double threshold) {
    minThreshold = threshold;
}

void Budget::changeMoney(double delta) {
    totalMoney += delta;
    std::cout << "  [Budget] изменение бюджета на " << delta
        << " млн. руб. новый бюджет: " << totalMoney << " млн. руб." << std::endl;
}

void Budget::displayInfo() const {
    std::cout << "  бюджет страны: " << totalMoney << " млн. руб."
        << "\n  коэффициент столицы (k): " << capitalPriority
        << "\n  минимальный порог (Mp): " << minThreshold << std::endl;
}

double calculateCityShare(const Budget& budget, long long cityPopulation,
    long long totalPopulation, bool isCapital) {
    if (totalPopulation == 0 || budget.minThreshold == 0) return 0.0;

    //(Population/MinThreshold) * k
    //(Population/MinThreshold)
    double baseShare = static_cast<double>(cityPopulation) / budget.minThreshold;

    if (isCapital) {
        return baseShare * budget.capitalPriority;
    }
    else {
        return baseShare;
    }
}
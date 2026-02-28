#include "City.hpp"

City::City() : name("Неизвестно"), population(0), availableMoney(0.0), isCapital(false) {
    std::cout << "  [City] конструктор по умолчанию для города" << std::endl;
}

City::City(std::string n, long long pop, double money, bool capital)
    : name(n), population(pop), availableMoney(money), isCapital(capital) {
    std::cout << "  [City] конструктор с параметрами для города " << n << std::endl;
}

City::City(const City& other)
    : name(other.name), population(other.population),
    availableMoney(other.availableMoney), isCapital(other.isCapital) {
    std::cout << "  [City] конструктор копирования для города " << name << std::endl;
}

City& City::operator=(const City& other) {
    std::cout << "  [City] оператор присваивания для города" << std::endl;
    if (this != &other) {
        name = other.name;
        population = other.population;
        availableMoney = other.availableMoney;
        isCapital = other.isCapital;
    }
    return *this;
}

City::~City() {
    std::cout << "  [City] деструктор для города " << name << std::endl;
}

void City::displayInfo() const {
    std::cout << "    Город: " << name;
    if (isCapital) std::cout << " (СТОЛИЦА)";
    std::cout << "\n    Население: " << population
        << "\n    Доступно денег: " << availableMoney << " млн. руб." << std::endl;
}

void City::addMoney(double amount) {
    availableMoney += amount;
}

bool City::operator==(const City& other) const {
    return name == other.name;
}
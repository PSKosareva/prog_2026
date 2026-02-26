#include "Country.hpp"
#include <algorithm>

// по умолчанию
Country::Country() : name("Неизвестно"), capital("Неизвестно"), area(0) {
    cities = new std::vector<std::string>();
    std::cout << "\nконструктор по умолчанию:" << std::endl;
}

// с параметрами
Country::Country(std::string n, std::string c, double a, std::vector<std::string> cityList)
    : name(n), capital(c), area(a) {
    cities = new std::vector<std::string>(cityList);
    std::cout << "\nконструктор с параметрами:" << std::endl;
}

// копирования
Country::Country(const Country& other)
    : name(other.name), capital(other.capital), area(other.area) {
    cities = new std::vector<std::string>(*other.cities);
    std::cout << "\nконструктор копирования:" << std::endl;
}


Country& Country::operator=(const Country& other) {
    std::cout << "Оператор присваивания" << std::endl;

    if (this != &other) {
        name = other.name;
        capital = other.capital;
        area = other.area;

        delete cities;
        cities = new std::vector<std::string>(*other.cities);
    }
    return *this;
}

Country::~Country() {
    std::cout << "Деструктор.";

    if (cities) {
        std::cout << " Удаляем " << cities->size() << " городов: ";
        for (const auto& city : *cities) {
            std::cout << city << " ";
        }
        cities->clear();
        std::cout << "Очищено. ";

        delete cities;
        std::cout << "Память освобождена.";
    }

    std::cout << " Деструктор завершен." << std::endl;
}

// ИСПРАВЛЕННЫЙ оператор +=
Country& Country::operator+=(const Country& other) {
    std::cout << "Оператор +=" << std::endl;

    // Изменяем название
    this->name = this->name + "-" + other.name;

    // Столица большей страны
    if (other.area > this->area) {
        this->capital = other.capital;
    }

    // Площадь = сумма
    this->area = this->area + other.area;

    // 1. СНАЧАЛА убираем повторы в текущем векторе (работаем со старым вектором!)
    for (size_t i = 0; i < cities->size(); i++) {
        for (size_t j = i + 1; j < cities->size(); ) {
            if ((*cities)[i] == (*cities)[j]) {
                // Удаляем повтор через erase (работаем с текущим объектом!)
                cities->erase(cities->begin() + j);
            }
            else {
                j++;
            }
        }
    }

    // 2. ПОТОМ добавляем новые города из other
    for (const auto& city : *other.cities) {
        bool found = false;
        for (const auto& existingCity : *this->cities) {
            if (existingCity == city) {
                found = true;
                break;
            }
        }

        if (found) {
            this->cities->push_back(city + " новый");
        }
        else {
            this->cities->push_back(city);
        }
    }

    return *this;
}

// ИСПРАВЛЕННЫЙ оператор +
Country Country::operator+(const Country& other) const {
    std::string newName = this->name + "-" + other.name;

    std::string newCapital;
    if (this->area >= other.area) {
        newCapital = this->capital;
    }
    else {
        newCapital = other.capital;
    }

    double newArea = this->area + other.area;

    std::vector<std::string> newCities;

    // ИСПРАВЛЕНО: сначала убираем повторы в первом операнде
    std::vector<std::string> uniqueCitiesFirst;
    for (const auto& city : *this->cities) {
        bool found = false;
        for (const auto& uniqueCity : uniqueCitiesFirst) {
            if (uniqueCity == city) {
                found = true;
                break;
            }
        }
        if (!found) {
            uniqueCitiesFirst.push_back(city);
        }
    }

    // ИСПРАВЛЕНО: добавляем uniqueCitiesFirst, а не original cities
    for (const auto& city : uniqueCitiesFirst) {
        newCities.push_back(city);
    }

    // Добавляем города из второго операнда
    for (const auto& city : *other.cities) {
        bool found = false;
        for (const auto& existingCity : newCities) {
            if (existingCity == city) {
                found = true;
                break;
            }
        }

        if (found) {
            newCities.push_back(city + " новый");
        }
        else {
            newCities.push_back(city);
        }
    }

    return Country(newName, newCapital, newArea, newCities);
}

Country Country::operator*(const Country& other) const {
    std::string newName = "путь " + this->name + "->" + other.name;

    std::string newCapital = "нет";

    double newArea = (this->area < other.area) ? this->area : other.area;
    std::vector<std::string> newCities;

    if (this->capital != "нет" && !this->capital.empty()) {
        newCities.push_back(this->capital);
    }

    if (other.capital != "нет" && !other.capital.empty()) {
        bool found = false;
        for (const auto& city : newCities) {
            if (city == other.capital) {
                found = true;
                break;
            }
        }

        if (found) {
            newCities.push_back(other.capital + " новый");
        }
        else {
            newCities.push_back(other.capital);
        }
    }

    if (this->cities->size() > 0) {
        std::string firstCity = (*this->cities)[0];

        bool found = false;
        for (const auto& city : newCities) {
            if (city == firstCity) {
                found = true;
                break;
            }
        }

        if (found) {
            newCities.push_back(firstCity + " новый");
        }
        else {
            newCities.push_back(firstCity);
        }
    }

    if (other.cities->size() > 0) {
        std::string secondCity = (*other.cities)[0];

        bool found = false;
        for (const auto& city : newCities) {
            if (city == secondCity) {
                found = true;
                break;
            }
        }

        if (found) {
            newCities.push_back(secondCity + " новый");
        }
        else {
            newCities.push_back(secondCity);
        }
    }

    return Country(newName, newCapital, newArea, newCities);
}

// Метод для удаления повторяющихся городов
void Country::removeDuplicateCities() {
    std::vector<std::string> uniqueCities;

    for (const auto& city : *cities) {
        bool found = false;
        for (const auto& uniqueCity : uniqueCities) {
            if (uniqueCity == city) {
                found = true;
                break;
            }
        }
        if (!found) {
            uniqueCities.push_back(city);
        }
    }

    cities->clear();
    for (const auto& city : uniqueCities) {
        cities->push_back(city);
    }
}

std::string Country::getName() const {
    return name;
}

std::string Country::getCapital() const {
    return capital;
}

double Country::getArea() const {
    return area;
}

std::vector<std::string> Country::getCities() const {
    return *cities;
}

void Country::setName(std::string n) {
    name = n;
}

void Country::setCapital(std::string c) {
    capital = c;
}

void Country::setArea(double a) {
    area = a;
}

void Country::addCity(std::string city) {
    cities->push_back(city);
}

void Country::displayInfo() const {
    std::cout << "Название: " << name << std::endl;
    std::cout << "Столица: " << capital << std::endl;
    std::cout << "Площадь: " << area << " кв.км" << std::endl;
    std::cout << "Города: ";
    if (cities->empty()) {
        std::cout << "нет городов";
    }
    else {
        for (size_t i = 0; i < cities->size(); ++i) {
            std::cout << (*cities)[i];
            if (i < cities->size() - 1) {
                std::cout << ", ";
            }
        }
    }
    std::cout << std::endl;
}

bool Country::hasCity(const std::string& city) const {
    for (const auto& c : *cities) {
        if (c == city) {
            return true;
        }
    }
    return false;
}
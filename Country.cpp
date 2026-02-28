#include "Country.hpp"
#include <algorithm>
#include <numeric>
#include <cmath>

// по умолчанию
Country::Country() : name("Неизвестно"), capital("Неизвестно"),
foundationDate("01.01.2000"), area(0), population(0) {
    budget = std::make_unique<Budget>();
    std::cout << "\n[Country] конструктор по умолчанию" << std::endl;
}

// с параметрами
Country::Country(std::string n, std::string c, std::string date, double a, long long pop, std::vector<City> cityList)
    : name(n), capital(c), foundationDate(date), area(a), population(pop), cities(cityList) {

    for (auto& city : cities) {
        if (city.name == capital) {
            city = City(city.name, city.population, city.availableMoney, true);
        }
    }

    budget = std::make_unique<Budget>();
    std::cout << "\n[Country] конструктор с параметрами для страны " << name << std::endl;
}

//  копирования
Country::Country(const Country& other)
    : name(other.name), capital(other.capital), foundationDate(other.foundationDate),
    area(other.area), population(other.population), cities(other.cities) {

    if (other.budget) {
        budget = std::make_unique<Budget>(*other.budget);
    }
    std::cout << "\n[Country] конструктор копирования для страны " << name << std::endl;
}

// оператор присваивания
Country& Country::operator=(const Country& other) {
    std::cout << "\n[Country] оператор присваивания" << std::endl;

    if (this != &other) {
        name = other.name;
        capital = other.capital;
        foundationDate = other.foundationDate;
        area = other.area;
        population = other.population;
        cities = other.cities;

        if (other.budget) {
            budget = std::make_unique<Budget>(*other.budget);
        }
    }
    return *this;
}

Country::~Country() {
    std::cout << "\n[Country] деструктор для страны " << name << std::endl;
}

// объединение
Country Country::operator+(const Country& other) const {
    std::string newName = name + "-" + other.name;
    std::string newCapital = (area >= other.area) ? capital : other.capital;
    std::string newDate = foundationDate; // Оставляем дату первой страны
    double newArea = area + other.area;
    long long newPopulation = population + other.population;

    std::vector<City> newCities = cities;
    for (const auto& city : other.cities) {
        auto it = std::find_if(newCities.begin(), newCities.end(),
            [&city](const City& c) { return c.name == city.name; });
        if (it != newCities.end()) {
            // Город уже есть - создаём новый с пометкой
            City newCity(city.name + " новый", city.population, city.availableMoney);
            newCities.push_back(newCity);
        }
        else {
            newCities.push_back(city);
        }
    }

    for (auto& city : newCities) {
        if (city.name == newCapital) {
            city = City(city.name, city.population, city.availableMoney, true);
        }
    }

    Country result(newName, newCapital, newDate, newArea, newPopulation, newCities);

    if (budget && other.budget) {
        double totalMoney = budget->getTotalMoney() + other.budget->getTotalMoney();
        double avgPriority = (budget->getCapitalPriority() + other.budget->getCapitalPriority()) / 2.0;
        double minThreshold = std::min(budget->getMinThreshold(), other.budget->getMinThreshold());
        result.setBudget(Budget(totalMoney, avgPriority, minThreshold));
    }

    return result;
}

// +=
Country& Country::operator+=(const Country& other) {
    std::cout << "\n[Country] оператор +=" << std::endl;

    name = name + "-" + other.name;
    if (other.area > area) {
        capital = other.capital;
    }
    area += other.area;
    population += other.population;
    for (const auto& city : other.cities) {
        auto it = std::find_if(cities.begin(), cities.end(),
            [&city](const City& c) { return c.name == city.name; });
        if (it != cities.end()) {
            City newCity(city.name + " новый", city.population, city.availableMoney);
            cities.push_back(newCity);
        }
        else {
            cities.push_back(city);
        }
    }
    for (auto& city : cities) {
        if (city.name == capital) {
            city = City(city.name, city.population, city.availableMoney, true);
        }
    }
    if (budget && other.budget) {
        double totalMoney = budget->getTotalMoney() + other.budget->getTotalMoney();
        double avgPriority = (budget->getCapitalPriority() + other.budget->getCapitalPriority()) / 2.0;
        double minThreshold = std::min(budget->getMinThreshold(), other.budget->getMinThreshold());
        budget = std::make_unique<Budget>(totalMoney, avgPriority, minThreshold);
    }

    return *this;
}
Country Country::operator*(const Country& other) const {
    std::string newName = "путь " + name + "->" + other.name;
    std::string newCapital = "нет";
    std::string newDate = foundationDate;
    double newArea = (area < other.area) ? area : other.area;
    long long newPopulation = (population < other.population) ? population : other.population;

    std::vector<City> newCities;
    auto capitalIt = std::find_if(cities.begin(), cities.end(),
        [this](const City& c) { return c.name == capital; });
    if (capitalIt != cities.end()) {
        newCities.push_back(*capitalIt);
    }

    auto otherCapitalIt = std::find_if(other.cities.begin(), other.cities.end(),
        [&other](const City& c) { return c.name == other.capital; });
    if (otherCapitalIt != other.cities.end()) {
        auto it = std::find_if(newCities.begin(), newCities.end(),
            [&other](const City& c) { return c.name == other.capital; });
        if (it != newCities.end()) {
            City newCity(other.capital + " новый", otherCapitalIt->population, otherCapitalIt->availableMoney);
            newCities.push_back(newCity);
        }
        else {
            newCities.push_back(*otherCapitalIt);
        }
    }
    if (!cities.empty()) {
        const City& firstCity = cities[0];
        auto it = std::find_if(newCities.begin(), newCities.end(),
            [&firstCity](const City& c) { return c.name == firstCity.name; });
        if (it != newCities.end()) {
            City newCity(firstCity.name + " новый", firstCity.population, firstCity.availableMoney);
            newCities.push_back(newCity);
        }
        else {
            newCities.push_back(firstCity);
        }
    }

    if (!other.cities.empty()) {
        const City& secondCity = other.cities[0];
        auto it = std::find_if(newCities.begin(), newCities.end(),
            [&secondCity](const City& c) { return c.name == secondCity.name; });
        if (it != newCities.end()) {
            City newCity(secondCity.name + " новый", secondCity.population, secondCity.availableMoney);
            newCities.push_back(newCity);
        }
        else {
            newCities.push_back(secondCity);
        }
    }

    Country result(newName, newCapital, newDate, newArea, newPopulation, newCities);

    if (budget && other.budget) {
        double totalMoney = std::min(budget->getTotalMoney(), other.budget->getTotalMoney());
        double priority = (budget->getCapitalPriority() + other.budget->getCapitalPriority()) / 2.0;
        double threshold = std::min(budget->getMinThreshold(), other.budget->getMinThreshold());
        result.setBudget(Budget(totalMoney, priority, threshold));
    }

    return result;
}

std::string Country::getName() const { return name; }
std::string Country::getCapital() const { return capital; }
std::string Country::getFoundationDate() const { return foundationDate; }
double Country::getArea() const { return area; }
long long Country::getPopulation() const { return population; }
std::vector<City> Country::getCities() const { return cities; }
Budget Country::getBudget() const { return budget ? *budget : Budget(); }

void Country::setName(std::string n) { name = n; }
void Country::setCapital(std::string c) { capital = c; }
void Country::setFoundationDate(std::string date) { foundationDate = date; }
void Country::setArea(double a) { area = a; }
void Country::setPopulation(long long pop) { population = pop; }
void Country::setBudget(const Budget& b) { budget = std::make_unique<Budget>(b); }

void Country::addCity(const City& city) {
    cities.push_back(city);
    population += city.population;
}

void Country::removeCity(const std::string& cityName) {
    auto it = std::remove_if(cities.begin(), cities.end(),
        [&cityName](const City& c) { return c.name == cityName; });
    if (it != cities.end()) {
        population -= it->population;
        cities.erase(it, cities.end());
    }
}

City* Country::findCity(const std::string& cityName) {
    for (auto& city : cities) {
        if (city.name == cityName) {
            return &city;
        }
    }
    return nullptr;
}

void Country::updateCityPopulation(const std::string& cityName, long long newPopulation) {
    City* city = findCity(cityName);
    if (city) {
        long long diff = newPopulation - city->population;
        city->population = newPopulation;
        population += diff;
    }
}
void Country::joinNewTerritory(const Country& territory) {
    std::cout << "\n[Country] присоединение территории " << territory.name << std::endl;

    area += territory.area;

    for (const auto& city : territory.cities) {
        if (!findCity(city.name)) {
            cities.push_back(city);
            population += city.population;
        }
    }
    if (budget && territory.budget) {
        double totalMoney = budget->getTotalMoney() + territory.budget->getTotalMoney();
        budget->setTotalMoney(totalMoney);
    }
}

void Country::changePopulation(long long delta) {
    population += delta;
    std::cout << "\n[Country] изменение населения на " << delta
        << ". \nновое население: " << population << std::endl;
}

long long Country::calculateTotalPopulation() const {
    return std::accumulate(cities.begin(), cities.end(), 0LL,
        [](long long sum, const City& city) { return sum + city.population; });
}

void Country::distributeBudget(double moneyChange) {
    std::cout << "\n[Country] распределение бюджета: изменено на " << moneyChange << " млн. руб." << std::endl;

    if (!budget || cities.empty()) return;

    budget->changeMoney(moneyChange);

    long long totalPop = calculateTotalPopulation();
    if (totalPop == 0) return;

    double Mp = budget->getMinThreshold();
    double k = budget->getCapitalPriority();

    //((Ap*k + Bp + Cp)/Mp)*X
    double sumWithPriority = 0.0;
    for (const auto& city : cities) {
        if (city.isCapital) {
            sumWithPriority += city.population * k;
        }
        else {
            sumWithPriority += city.population;
        }
    }

    double balanceFactor = (sumWithPriority / Mp) * moneyChange;
    std::cout << "      коэффициент баланса: " << balanceFactor << std::endl;

    for (auto& city : cities) {
        //Am + ((Ap*k)/Mp)*X
        double cityShare;
        if (city.isCapital) {
            cityShare = (static_cast<double>(city.population) * k / Mp) * moneyChange;
        }
        else {
            cityShare = (static_cast<double>(city.population) / Mp) * moneyChange;
        }

        City updatedCity = city;
        updatedCity.addMoney(cityShare);

        for (auto& c : cities) {
            if (c.name == city.name) {
                c = updatedCity;
                break;
            }
        }

        std::cout << "      город " << city.name << " получил " << cityShare
            << " млн. руб. (теперь: " << city.availableMoney + cityShare << ")" << std::endl;
    }
}

void Country::displayInfo() const {
    std::cout << "\nИНФА О СТРАНЕ:" << std::endl;
    std::cout << "Название: " << name << std::endl;
    std::cout << "Столица: " << capital << std::endl;
    std::cout << "Дата основания: " << foundationDate << std::endl;
    std::cout << "Площадь: " << area << " кв.км" << std::endl;
    std::cout << "Население: " << population << " чел." << std::endl;

    if (budget) {
        budget->displayInfo();
    }

    std::cout << "города (" << cities.size() << "):" << std::endl;
    if (cities.empty()) {
        std::cout << "  нет городов:(" << std::endl;
    }
    else {
        for (const auto& city : cities) {
            city.displayInfo();
        }
    }
    std::cout << "\n крутая страна, да?\n" << std::endl;
}

void Country::demonstrateBudgetDistribution() {
    std::cout << "\nБЮДЖЕТ (ДЕМО):" << std::endl;
    displayInfo();

    std::cout << "\nраспределение +100 млн. руб." << std::endl;
    distributeBudget(100.0);

    std::cout << "\nраспределение -50 млн. руб." << std::endl;
    distributeBudget(-50.0);

    std::cout << "\nитог" << std::endl;
    displayInfo();
}
#include <iostream>
#include <string>
#include <windows.h>

class Country {
private:
    std::string name;
    std::string capital;
    std::string foundationDate;
    double area;
    long long population;

protected:
    std::string continent;

public:
    // по умолчанию
    Country() {
        name = "Неизвестно";
        capital = "Неизвестно";
        foundationDate = "Неизвестно";
        area = 0;
        population = 0;
        continent = "Неизвестно";
    }

    // полного заполнения
    Country(std::string n, std::string c, std::string fd, double a, long long p) {
        name = n;
        capital = c;
        foundationDate = fd;
        area = a;
        population = p;
        continent = "Европа";
    }

    // копирования
    Country(const Country& other) {
        name = other.name;
        capital = other.capital;
        foundationDate = other.foundationDate;
        area = other.area;
        population = other.population;
        continent = other.continent;
    }

    // деструктор
    ~Country() {
        std::cout << "Деструктор запущен" << std::endl;
    }

    // геттеры
    std::string getName() { return name; }
    std::string getCapital() { return capital; }
    std::string getFoundationDate() { return foundationDate; }
    double getArea() { return area; }
    long long getPopulation() { return population; }
    std::string getContinent() { return continent; }

    // сеттер для столицы
    bool setCapital(std::string newCapital) {
        if (newCapital.length() > 0) {
            capital = newCapital;
            return true;
        }
        return false;
    }

    // вывод инфы
    void displayInfo() {
        std::cout << "Название: " << name << std::endl;
        std::cout << "Столица: " << capital << std::endl;
        std::cout << "Дата основания: " << foundationDate << std::endl;
        std::cout << "Площадь: " << area << " кв.км" << std::endl;
        std::cout << "Население: " << population << " чел." << std::endl;
        std::cout << "Континент: " << continent << std::endl;
    }

    // +территория
    void addTerritory(double newArea) {
        if (newArea > 0) {
            area += newArea;
        }
    }

    // +население
    void populationGrowth() {
        if (population > 0) {
            population = population * 1.01;
        }
    }
};

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    // по умолчанию
    Country c1;
    std::cout << "Страна 1 (по умолчанию):" << std::endl;
    c1.displayInfo();

    std::cout << "\n-------------\n";

    // полное заполнение
    Country c2("Россия", "Москва", "862 год", 17100000, 146000000);
    std::cout << "Страна 2 (полное заполнение):" << std::endl;
    c2.displayInfo();

    std::cout << "\n-------------\n";

    // копирование
    Country c3 = c2;
    std::cout << "Страна 3 (копия страны 2):" << std::endl;
    c3.displayInfo();

    std::cout << "\n-------------\n";

    // изменение столицы
    std::cout << "Меняем столицу на 'Санкт-Петербург':" << std::endl;
    c3.setCapital("Санкт-Петербург");
    c3.displayInfo();

    std::cout << "\n-------------\n";

    // +территория
    std::cout << "Добавляем территорию 50000 кв.км:" << std::endl;
    c3.addTerritory(50000);
    std::cout << "Новая площадь: " << c3.getArea() << " кв.км" << std::endl;

    std::cout << "\n-------------\n";

    // +населения
    std::cout << "Рост населения на 1%:" << std::endl;
    c3.populationGrowth();
    std:: cout << "Новое население: " << c3.getPopulation() << " чел." << std::endl;

    return 0;

}

#include <iostream>
#include <windows.h>
#include <vector>
#include <memory>
#include "Country.hpp"
#include "City.hpp"
#include "Budget.hpp"

// Функция для оценки лабораторной работы
void evaluateLab() {
    std::cout << "\n~~~~ ОЦЕНКА ЛАБОРАТОРНОЙ РАБОТЫ ~~~" << std::endl;
    std::cout << "  Интерес: 8/10 - прикольно было " << std::endl;
    std::cout << "  Наполненность: 10/10 - много нового материала, есть с чем работать" << std::endl;
    std::cout << "  Сложность: 1000/10 - было довольно-таки. . . трудно, не уверена, что тут нет того, что пропустила :)" << std::endl;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    std::cout << "РАБОТА БИБЛИОТЕКИ (ДЕМО)" << std::endl;

    std::cout << "\nсоздание городов" << std::endl;
    City moscow("Москва", 12600000, 500.0, true);
    City spb("Санкт-Петербург", 5400000, 200.0);
    City kazan("Казань", 1250000, 50.0);
    City novosib("Новосибирск", 1620000, 60.0);

    moscow.displayInfo();
    spb.displayInfo();
    kazan.displayInfo();

    std::cout << "\nсоздание бюджетов" << std::endl;
    Budget russianBudget(10000.0, 1.8, 1000000.0);
    Budget tatarianBudget(2000.0, 1.2, 500000.0);

    russianBudget.displayInfo();
    tatarianBudget.displayInfo();

    std::cout << "\nРАБОТа МОДИФИЦИРОВАННОГО КЛАССА COUNTRY (ДЕМО)" << std::endl;

    std::vector<City> russianCities = { moscow, spb, novosib };
    Country russia("Россия", "Москва", "12.12.1991", 17098242, 146000000, russianCities);
    russia.setBudget(russianBudget);

    std::vector<City> tatarCities = { kazan, City("Набережные Челны", 530000, 30.0) };
    Country tatarstan("Татарстан", "Казань", "30.08.1990", 67847, 3890000, tatarCities);
    tatarstan.setBudget(tatarianBudget);

    std::cout << "\nисходные данные" << std::endl;
    russia.displayInfo();
    tatarstan.displayInfo();

    std::cout << "\nОПЕРАТОРЫ С ЛАБЫ 2Л2" << std::endl;

    std::cout << "\n--- оператор + (объединение) ---" << std::endl;
    Country united = russia + tatarstan;
    united.displayInfo();

    std::cout << "\n--- оператор * (пересечение) ---" << std::endl;
    Country intersection = russia * tatarstan;
    intersection.displayInfo();

    std::cout << "\n--- оператор += (присоединение) ---" << std::endl;
    Country russiaCopy = russia;
    russiaCopy += tatarstan;
    russiaCopy.displayInfo();

    std::cout << "\nДОБАВЛЕНИЕ НОВЫХ МЕТОДОВ" << std::endl;

    std::cout << "\n--- присоединение новой территории ---" << std::endl;
    Country russiaForJoin = russia;
    std::cout << "до присоединения:" << std::endl;
    russiaForJoin.displayInfo();

    russiaForJoin.joinNewTerritory(tatarstan);
    std::cout << "после присоединения Татарстана:" << std::endl;
    russiaForJoin.displayInfo();

    std::cout << "\nизменение населения" << std::endl;
    Country populationDemo = russia;
    populationDemo.displayInfo();
    populationDemo.changePopulation(1000000); 
    populationDemo.displayInfo();

    std::cout << "\nраспределение бюджета" << std::endl;
    Country budgetDemo("Демо", "Столица", "01.01.2020", 1000, 10000000,
        { City("Столица", 5000000, 1000.0, true),
         City("Город1", 3000000, 500.0),
         City("Город2", 2000000, 300.0) });
    budgetDemo.setBudget(Budget(5000.0, 2.0, 1000000.0));

    budgetDemo.demonstrateBudgetDistribution();

    std::cout << "\nКОНСТРУКТОРЫ И ДЕСТРУКТОРЫ" << std::endl;

    std::cout << "\nпо умолчанию" << std::endl;
    Country defaultCountry;

    std::cout << "\nкопирования" << std::endl;
    Country copiedCountry = russia;

    std::cout << "\nоператор присваивания" << std::endl;
    Country assignedCountry;
    assignedCountry = tatarstan;

    std::cout << "\nзавершение работы (вызов деструкторов)" << std::endl;

    evaluateLab();

    return 0;
}
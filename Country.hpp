#ifndef COUNTRY_H
#define COUNTRY_H

#include <iostream>
#include <string>
#include <vector>

class Country {
private:
    std::string name;
    std::string capital;
    double area;
    std::vector<std::string>* cities;

public:
    Country();

    Country(std::string n, std::string c, double a, std::vector<std::string> cityList);

    Country(const Country& other);

    Country& operator=(const Country& other);

    ~Country();

    Country operator+(const Country& other) const;

    Country& operator+=(const Country& other);

    Country operator*(const Country& other) const;

    std::string getName() const;
    std::string getCapital() const;
    double getArea() const;
    std::vector<std::string> getCities() const;

    void setName(std::string n);
    void setCapital(std::string c);
    void setArea(double a);
    void addCity(std::string city);

    void displayInfo() const;

    bool hasCity(const std::string& city) const;

    void removeDuplicateCities();
};

#endif
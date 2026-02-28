cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME 3LAB1)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Создаем библиотеку с новыми классами
add_library(CityBudgetLib 
    "City.cpp"
    "Budget.cpp"
)

# Добавляем все исходные файлы проекта
add_executable(${PROJECT_NAME} 
    "main.cpp"
    "Country.cpp"
)

# Линкуем библиотеку к основной программе
target_link_libraries(${PROJECT_NAME} CityBudgetLib)
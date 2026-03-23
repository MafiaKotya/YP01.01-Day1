#pragma once
#include <string>

// Перечисление Group: WORK, FAMILY, FRIENDS, OTHERS 
enum class Group {
    WORK,
    FAMILY,
    FRIENDS,
    OTHERS
};

// Структура Date: поля day, month, year (тип int) [cite: 7]
struct Date {
    int day;
    int month;
    int year;
};

// Структура Phone: поля countryCode (int), cityCode (int), number (long long) [cite: 8]
struct Phone {
    int countryCode;
    int cityCode;
    long long number;
};

// Структура Contact [cite: 10]
struct Contact {
    std::string lastName;     // Фамилия [cite: 11]
    std::string firstName;    // Имя [cite: 11]
    std::string patronymic;   // Отчество [cite: 11]
    Phone phone;              // Экземпляр структуры Phone [cite: 12]
    Date birthDate;           // Экземпляр структуры Date (день рождения) [cite: 13]
    std::string email;        // Email (string) [cite: 14]
    Group category;           // Категория (тип Group) [cite: 15]
};
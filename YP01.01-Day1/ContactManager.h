#pragma once
#include "Contact.h"
#include <vector>
#include <string>

class ContactManager {
private:
    std::vector<Contact> contacts; // Хранение динамического списка 
    const std::string dbFileName = "database.txt"; // Имя файла для сохранения 

    // Вспомогательные методы для безопасного ввода и валидации [cite: 30, 31, 33]
    int getValidInt(const std::string& prompt);
    long long getValidLongLong(const std::string& prompt);
    bool isValidEmail(const std::string& email) const;
    std::string groupToString(Group g) const;
    std::string truncateString(const std::string& str, size_t width) const; // Для обрезки фамилии 
    std::string toLower(const std::string& str) const; // Для регистронезависимого поиска 

public:
    ContactManager();
    ~ContactManager(); // Сохранение при выходе будет здесь 
     
    // Основные функциональные возможности [cite: 16, 17]
    void addContact();         // Добавление контакта [cite: 18]
    void displayAll() const;   // Просмотр всех записей (передача по ссылке const Contact& внутри) [cite: 19, 43]
    void searchByMask() const; // Поиск по маске (несколько ключевых слов) [cite: 20, 35]
    void deleteContact();      // Удаление по фамилии [cite: 21]
    void editContact();        // Редактирование [cite: 22]

    // Сортировки [cite: 23]
    void sortAlphabetically(); // По алфавиту [cite: 24]
    void sortByBirthday();     // По дате рождения [cite: 25]

    // Файловые операции [cite: 26, 39]
    void loadFromFile();
    void saveToFile() const;

    // 2 иные функции на выбор 
    void showStatistics() const;
    void clearAllContacts();
};
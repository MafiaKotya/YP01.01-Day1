
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>
#include <limits>

using namespace std;

// ========================== Структуры данных ==========================

struct Date {
    int day;
    int month;
    int year;
};

struct Phone {
    int countryCode;
    int cityCode;
    long long number;   // номер телефона (без кодов)
};

enum class Group { WORK, FAMILY, FRIENDS, OTHERS };

struct Contact {
    string lastName;
    string firstName;
    string middleName;
    Phone phone;
    Date birthday;
    string email;
    Group group;
};

// ========================== Вспомогательные функции ==========================

// Преобразование enum Group в строку
string groupToString(Group g) {
    switch (g) {
    case Group::WORK:   return "WORK";
    case Group::FAMILY: return "FAMILY";
    case Group::FRIENDS:return "FRIENDS";
    default:            return "OTHERS";
    }
}

// Преобразование строки в Group (для ввода)
Group stringToGroup(const string& s) {
    string lower = s;
    transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    if (lower == "work") return Group::WORK;
    if (lower == "family") return Group::FAMILY;
    if (lower == "friends") return Group::FRIENDS;
    return Group::OTHERS;
}

// Проверка корректности даты
bool isValidDate(const Date& d) {
    if (d.year < 1900 || d.year > 2100) return false;
    if (d.month < 1 || d.month > 12) return false;
    int daysInMonth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    // високосный год
    if (d.month == 2 && ((d.year % 4 == 0 && d.year % 100 != 0) || d.year % 400 == 0))
        daysInMonth[1] = 29;
    return d.day >= 1 && d.day <= daysInMonth[d.month - 1];
}

// Ввод целого числа с защитой от ошибок
int inputInt(const string& prompt, int minVal = INT_MIN, int maxVal = INT_MAX) {
    int val;
    while (true) {
        cout << prompt;
        cin >> val;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Ошибка: введите целое число.\n";
        }
        else if (val < minVal || val > maxVal) {
            cout << "Ошибка: число должно быть в диапазоне [" << minVal << ", " << maxVal << "].\n";
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return val;
        }
    }
}

// Ввод телефона с валидацией
Phone inputPhone() {
    Phone p;
    p.countryCode = inputInt("Код страны (1-999): ", 1, 999);
    p.cityCode = inputInt("Код города: ", 0, 999);
    // Номер телефона: long long, не может содержать букв (проверка через ввод числа)
    cout << "Номер телефона (только цифры): ";
    while (true) {
        cin >> p.number;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Ошибка: номер должен состоять только из цифр. Повторите: ";
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
    }
    return p;
}

// Ввод даты с валидацией
Date inputDate() {
    Date d;
    do {
        d.day = inputInt("День: ", 1, 31);
        d.month = inputInt("Месяц: ", 1, 12);
        d.year = inputInt("Год (1900-2100): ", 1900, 2100);
        if (!isValidDate(d)) {
            cout << "Некорректная дата. Повторите ввод.\n";
        }
    } while (!isValidDate(d));
    return d;
}

// Валидация email
bool isValidEmail(const string& email) {
    size_t atPos = email.find('@');
    if (atPos == string::npos) return false;
    size_t dotPos = email.find('.', atPos + 1);
    return dotPos != string::npos;
}

// Ввод email с проверкой
string inputEmail() {
    string email;
    while (true) {
        cout << "Email: ";
        getline(cin, email);
        if (isValidEmail(email)) break;
        cout << "Некорректный email (должен содержать '@' и точку после '@'). Повторите.\n";
    }
    return email;
}

// Ввод группы
Group inputGroup() {
    string groupStr;
    while (true) {
        cout << "Категория (WORK, FAMILY, FRIENDS, OTHERS): ";
        getline(cin, groupStr);
        string lower = groupStr;
        transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
        if (lower == "work" || lower == "family" || lower == "friends" || lower == "others")
            return stringToGroup(groupStr);
        cout << "Некорректная категория. Попробуйте снова.\n";
    }
}

// Ввод контакта с клавиатуры
Contact inputContact() {
    Contact c;
    cout << "Введите фамилию (на английском): ";
    getline(cin, c.lastName);
    cout << "Введите имя (на английском):";
    getline(cin, c.firstName);
    cout << "Введите отчество (на английском):";
    getline(cin, c.middleName);

    cout << "Телефон:\n";
    c.phone = inputPhone();

    cout << "Дата рождения:\n";
    c.birthday = inputDate();

    c.email = inputEmail();

    c.group = inputGroup();

    return c;
}

// ========================== Функции для работы с таблицей ==========================

// Обрезание строки до maxLen + "..."
string truncate(const string& str, size_t maxLen) {
    if (str.length() <= maxLen) return str;
    return str.substr(0, maxLen) + "...";
}

// Вывод одного контакта в строке таблицы (используя заданные ширины)
void printContact(const Contact& c, int wLast, int wFirst, int wMiddle, int wPhone, int wBirth, int wEmail, int wGroup) {
    // Форматируем телефон как строку
    string phoneStr = "+" + to_string(c.phone.countryCode) + "(" + to_string(c.phone.cityCode) + ")" + to_string(c.phone.number);
    // Дата
    string birthStr = to_string(c.birthday.day) + "." + to_string(c.birthday.month) + "." + to_string(c.birthday.year);
    // Email
    string emailStr = c.email;
    // Группа
    string groupStr = groupToString(c.group);

    cout << left
        << "| " << setw(wLast) << truncate(c.lastName, wLast) << " "
        << "| " << setw(wFirst) << truncate(c.firstName, wFirst) << " "
        << "| " << setw(wMiddle) << truncate(c.middleName, wMiddle) << " "
        << "| " << setw(wPhone) << truncate(phoneStr, wPhone) << " "
        << "| " << setw(wBirth) << truncate(birthStr, wBirth) << " "
        << "| " << setw(wEmail) << truncate(emailStr, wEmail) << " "
        << "| " << setw(wGroup) << truncate(groupStr, wGroup) << " |\n";
}

// Вывод всех контактов в виде таблицы
void printAllContacts(const vector<Contact>& contacts) {
    if (contacts.empty()) {
        cout << "Список контактов пуст.\n";
        return;
    }

    // Определяем максимальные ширины колонок (с учётом заголовков)
    int wLast = 8, wFirst = 5, wMiddle = 9, wPhone = 15, wBirth = 10, wEmail = 20, wGroup = 6;
    for (const auto& c : contacts) {
        wLast = max(wLast, (int)c.lastName.length());
        wFirst = max(wFirst, (int)c.firstName.length());
        wMiddle = max(wMiddle, (int)c.middleName.length());
        string phoneStr = "+" + to_string(c.phone.countryCode) + "(" + to_string(c.phone.cityCode) + ")" + to_string(c.phone.number);
        wPhone = max(wPhone, (int)phoneStr.length());
        string birthStr = to_string(c.birthday.day) + "." + to_string(c.birthday.month) + "." + to_string(c.birthday.year);
        wBirth = max(wBirth, (int)birthStr.length());
        wEmail = max(wEmail, (int)c.email.length());
        string groupStr = groupToString(c.group);
        wGroup = max(wGroup, (int)groupStr.length());
    }
    // Ограничим ширину для удобства (не обязательно)
    wLast = min(wLast, 20);
    wFirst = min(wFirst, 20);
    wMiddle = min(wMiddle, 20);
    wPhone = min(wPhone, 30);
    wBirth = min(wBirth, 12);
    wEmail = min(wEmail, 30);
    wGroup = min(wGroup, 10);

    // Вывод заголовка
    cout << string(2 + wLast + 2 + wFirst + 2 + wMiddle + 2 + wPhone + 2 + wBirth + 2 + wEmail + 2 + wGroup + 1, '-') << "\n";
    cout << left
        << "| " << setw(wLast) << "Фамилия" << " "
        << "| " << setw(wFirst) << "Имя" << " "
        << "| " << setw(wMiddle) << "Отчество" << " "
        << "| " << setw(wPhone) << "Телефон" << " "
        << "| " << setw(wBirth) << "Дата рожд." << " "
        << "| " << setw(wEmail) << "Email" << " "
        << "| " << setw(wGroup) << "Группа" << " |\n";
    cout << string(2 + wLast + 2 + wFirst + 2 + wMiddle + 2 + wPhone + 2 + wBirth + 2 + wEmail + 2 + wGroup + 1, '-') << "\n";

    // Вывод контактов
    for (const auto& c : contacts) {
        printContact(c, wLast, wFirst, wMiddle, wPhone, wBirth, wEmail, wGroup);
    }
    cout << string(2 + wLast + 2 + wFirst + 2 + wMiddle + 2 + wPhone + 2 + wBirth + 2 + wEmail + 2 + wGroup + 1, '-') << "\n";
}

// ========================== Поиск ==========================

// Регистронезависимое сравнение подстроки
bool containsIgnoreCase(const string& str, const string& substr) {
    if (substr.empty()) return true;
    auto it = search(str.begin(), str.end(), substr.begin(), substr.end(),
        [](char ch1, char ch2) { return tolower(ch1) == tolower(ch2); });
    return it != str.end();
}

// Поиск по маске (несколько ключевых слов, разделенных пробелами)
vector<int> searchContacts(const vector<Contact>& contacts, const string& mask) {
    vector<int> result;
    if (mask.empty()) return result;

    // Разбиваем маску на слова
    vector<string> keywords;
    istringstream iss(mask);
    string word;
    while (iss >> word) {
        keywords.push_back(word);
    }

    for (size_t i = 0; i < contacts.size(); ++i) {
        const auto& c = contacts[i];
        bool found = false;
        for (const auto& kw : keywords) {
            if (containsIgnoreCase(c.lastName, kw) || containsIgnoreCase(c.firstName, kw)) {
                found = true;
                break;
            }
        }
        if (found) result.push_back(i);
    }
    return result;
}

// ========================== Удаление ==========================

void deleteContact(vector<Contact>& contacts) {
    if (contacts.empty()) {
        cout << "Нет контактов для удаления.\n";
        return;
    }
    string lastName;
    cout << "Введите фамилию контакта для удаления: ";
    getline(cin, lastName);
    // Ищем все контакты с такой фамилией (без учета регистра)
    vector<size_t> indices;
    for (size_t i = 0; i < contacts.size(); ++i) {
        if (containsIgnoreCase(contacts[i].lastName, lastName)) {
            indices.push_back(i);
        }
    }
    if (indices.empty()) {
        cout << "Контакты с фамилией \"" << lastName << "\" не найдены.\n";
        return;
    }
    if (indices.size() == 1) {
        contacts.erase(contacts.begin() + indices[0]);
        cout << "Контакт удалён.\n";
        return;
    }
    // Если несколько, выводим список и предлагаем выбрать
    cout << "Найдено несколько контактов с такой фамилией:\n";
    for (size_t j = 0; j < indices.size(); ++j) {
        const auto& c = contacts[indices[j]];
        cout << j + 1 << ". " << c.lastName << " " << c.firstName << " " << c.middleName
            << " (" << groupToString(c.group) << ")\n";
    }
    cout << "Введите номер для удаления (0 - отмена): ";
    int choice;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (choice >= 1 && choice <= (int)indices.size()) {
        contacts.erase(contacts.begin() + indices[choice - 1]);
        cout << "Контакт удалён.\n";
    }
    else {
        cout << "Удаление отменено.\n";
    }
}

// ========================== Редактирование ==========================

void editContact(vector<Contact>& contacts) {
    if (contacts.empty()) {
        cout << "Нет контактов для редактирования.\n";
        return;
    }
    string lastName;
    cout << "Введите фамилию контакта для редактирования: ";
    getline(cin, lastName);
    vector<size_t> indices;
    for (size_t i = 0; i < contacts.size(); ++i) {
        if (containsIgnoreCase(contacts[i].lastName, lastName)) {
            indices.push_back(i);
        }
    }
    if (indices.empty()) {
        cout << "Контакты с фамилией \"" << lastName << "\" не найдены.\n";
        return;
    }
    size_t idx;
    if (indices.size() == 1) {
        idx = indices[0];
    }
    else {
        cout << "Найдено несколько контактов с такой фамилией:\n";
        for (size_t j = 0; j < indices.size(); ++j) {
            const auto& c = contacts[indices[j]];
            cout << j + 1 << ". " << c.lastName << " " << c.firstName << " " << c.middleName
                << " (" << groupToString(c.group) << ")\n";
        }
        cout << "Введите номер для редактирования (0 - отмена): ";
        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (choice < 1 || choice >(int)indices.size()) {
            cout << "Редактирование отменено.\n";
            return;
        }
        idx = indices[choice - 1];
    }

    Contact& c = contacts[idx];
    cout << "Редактирование контакта:\n";
    cout << "Фамилия (" << c.lastName << "): ";
    string input;
    getline(cin, input);
    if (!input.empty()) c.lastName = input;
    cout << "Имя (" << c.firstName << "): ";
    getline(cin, input);
    if (!input.empty()) c.firstName = input;
    cout << "Отчество (" << c.middleName << "): ";
    getline(cin, input);
    if (!input.empty()) c.middleName = input;

    cout << "Телефон:\n";
    cout << "Код страны (текущий " << c.phone.countryCode << "): ";
    string str;
    getline(cin, str);
    if (!str.empty()) c.phone.countryCode = stoi(str);
    cout << "Код города (текущий " << c.phone.cityCode << "): ";
    getline(cin, str);
    if (!str.empty()) c.phone.cityCode = stoi(str);
    cout << "Номер телефона (текущий " << c.phone.number << "): ";
    getline(cin, str);
    if (!str.empty()) c.phone.number = stoll(str);

    cout << "Дата рождения:\n";
    cout << "День (" << c.birthday.day << "): ";
    getline(cin, str);
    if (!str.empty()) c.birthday.day = stoi(str);
    cout << "Месяц (" << c.birthday.month << "): ";
    getline(cin, str);
    if (!str.empty()) c.birthday.month = stoi(str);
    cout << "Год (" << c.birthday.year << "): ";
    getline(cin, str);
    if (!str.empty()) c.birthday.year = stoi(str);
    if (!isValidDate(c.birthday)) {
        cout << "Некорректная дата, оставлена старая.\n";
        // можно восстановить старые значения, но мы просто предупредим
    }

    cout << "Email (" << c.email << "): ";
    getline(cin, str);
    if (!str.empty()) {
        if (isValidEmail(str)) c.email = str;
        else cout << "Некорректный email, оставлен старый.\n";
    }

    cout << "Категория (" << groupToString(c.group) << "): ";
    getline(cin, str);
    if (!str.empty()) {
        Group newGroup = stringToGroup(str);
        c.group = newGroup;
    }
    cout << "Контакт отредактирован.\n";
}

// ========================== Сортировка ==========================

void sortByName(vector<Contact>& contacts) {
    sort(contacts.begin(), contacts.end(), [](const Contact& a, const Contact& b) {
        string fullA = a.lastName + a.firstName;
        string fullB = b.lastName + b.firstName;
        return fullA < fullB;
        });
    cout << "Сортировка по алфавиту (фамилия+имя) выполнена.\n";
}

void sortByBirthday(vector<Contact>& contacts) {
    if (contacts.empty()) return;

    // Получаем текущую дату один раз перед сортировкой
    time_t t = time(nullptr);
    tm now_tm;
    localtime_s(&now_tm, &t);  // безопасная версия

    int currentDay = now_tm.tm_mday;
    int currentMonth = now_tm.tm_mon + 1;
    int currentYear = now_tm.tm_year + 1900;

    // Функция вычисления дней до следующего дня рождения
    auto daysUntilBirthday = [currentDay, currentMonth, currentYear](const Contact& c) {
        int day = c.birthday.day;
        int month = c.birthday.month;
        int year = c.birthday.year;

        // Количество дней в году (учитываем високосность текущего года)
        auto daysInYear = [](int y) -> int {
            return 365 + ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0) ? 1 : 0);
            };

        // Номер дня в году для заданной даты
        auto dayOfYear = [](int y, int m, int d) -> int {
            int daysPerMonth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
            if ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0))
                daysPerMonth[1] = 29;
            int doy = 0;
            for (int i = 0; i < m - 1; ++i)
                doy += daysPerMonth[i];
            doy += d;
            return doy;
            };

        int currentDOY = dayOfYear(currentYear, currentMonth, currentDay);
        int birthDOY = dayOfYear(currentYear, month, day);

        if (birthDOY >= currentDOY) {
            return birthDOY - currentDOY;
        }
        else {
            // День рождения в следующем году
            int daysLeftThisYear = daysInYear(currentYear) - currentDOY;
            int daysInNextYear = daysInYear(currentYear + 1);
            int daysUntilNextBirthday = daysLeftThisYear + dayOfYear(currentYear + 1, month, day);
            return daysUntilNextBirthday;
        }
        };

    // Сортировка по возрастанию дней до дня рождения
    sort(contacts.begin(), contacts.end(),
        [&daysUntilBirthday](const Contact& a, const Contact& b) {
            return daysUntilBirthday(a) < daysUntilBirthday(b);
        });

    cout << "Сортировка по дате рождения (от ближайших) выполнена.\n";
}

// ========================== Сохранение и загрузка ==========================

void saveToFile(const vector<Contact>& contacts, const string& filename) {
    ofstream out(filename);
    if (!out) {
        cout << "Ошибка открытия файла для сохранения.\n";
        return;
    }
    for (const auto& c : contacts) {
        out << c.lastName << '\n'
            << c.firstName << '\n'
            << c.middleName << '\n'
            << c.phone.countryCode << ' ' << c.phone.cityCode << ' ' << c.phone.number << '\n'
            << c.birthday.day << ' ' << c.birthday.month << ' ' << c.birthday.year << '\n'
            << c.email << '\n'
            << static_cast<int>(c.group) << '\n';
    }
    cout << "База сохранена в файл " << filename << "\n";
}

void loadFromFile(vector<Contact>& contacts, const string& filename) {
    ifstream in(filename);
    if (!in) {
        cout << "Файл " << filename << " не найден. Будет создан новый.\n";
        return;
    }
    contacts.clear();
    Contact c;
    int groupInt;
    while (getline(in, c.lastName) &&
        getline(in, c.firstName) &&
        getline(in, c.middleName) &&
        (in >> c.phone.countryCode >> c.phone.cityCode >> c.phone.number) &&
        (in >> c.birthday.day >> c.birthday.month >> c.birthday.year) &&
        in.ignore() &&
        getline(in, c.email) &&
        (in >> groupInt) &&
        in.ignore())
    {
        c.group = static_cast<Group>(groupInt);
        contacts.push_back(c);
    }
    cout << "Загружено " << contacts.size() << " контактов из файла " << filename << "\n";
}

// ========================== Дополнительные функции ==========================

// 1. Показать контакты выбранной категории
void showByGroup(const vector<Contact>& contacts) {
    if (contacts.empty()) {
        cout << "Нет контактов.\n";
        return;
    }
    Group g = inputGroup();
    vector<Contact> filtered;
    for (const auto& c : contacts) {
        if (c.group == g) filtered.push_back(c);
    }
    if (filtered.empty()) {
        cout << "Нет контактов в категории " << groupToString(g) << "\n";
    }
    else {
        printAllContacts(filtered);
    }
}

// 2. Статистика
void showStatistics(const vector<Contact>& contacts) {
    cout << "Общее количество контактов: " << contacts.size() << "\n";
    int work = 0, family = 0, friends = 0, others = 0;
    for (const auto& c : contacts) {
        switch (c.group) {
        case Group::WORK: ++work; break;
        case Group::FAMILY: ++family; break;
        case Group::FRIENDS: ++friends; break;
        default: ++others;
        }
    }
    cout << "По категориям:\n";
    cout << "  WORK:    " << work << "\n";
    cout << "  FAMILY:  " << family << "\n";
    cout << "  FRIENDS: " << friends << "\n";
    cout << "  OTHERS:  " << others << "\n";
}

// ========================== Главное меню ==========================

int main() {
    setlocale(LC_ALL, "Russian");
    vector<Contact> contacts;
    const string filename = "database.txt";

    loadFromFile(contacts, filename);

    int choice;
    do {
        cout << "\n=== Меню ===";
        cout << "\n1. Добавить контакт";
        cout << "\n2. Просмотр всех записей";
        cout << "\n3. Поиск по маске";
        cout << "\n4. Удаление контакта";
        cout << "\n5. Редактирование контакта";
        cout << "\n6. Сортировка";
        cout << "\n7. Сохранить и выйти";
        cout << "\n8. Показать контакты по категории (доп.)";
        cout << "\n9. Статистика (доп.)";
        cout << "\nВыберите действие: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
        case 1: {
            Contact newContact = inputContact();
            contacts.push_back(newContact);
            cout << "Контакт добавлен.\n";
            break;
        }
        case 2:
            printAllContacts(contacts);
            break;
        case 3: {
            if (contacts.empty()) {
                cout << "Нет контактов для поиска.\n";
                break;
            }
            string mask;
            cout << "Введите поисковую маску (ключевые слова через пробел): ";
            getline(cin, mask);
            vector<int> indices = searchContacts(contacts, mask);
            if (indices.empty()) {
                cout << "Контакты не найдены.\n";
            }
            else {
                cout << "Найдено " << indices.size() << " контактов:\n";
                vector<Contact> found;
                for (int idx : indices) found.push_back(contacts[idx]);
                printAllContacts(found);
            }
            break;
        }
        case 4:
            deleteContact(contacts);
            break;
        case 5:
            editContact(contacts);
            break;
        case 6: {
            int sortChoice;
            cout << "Сортировка:\n1. По алфавиту\n2. По дате рождения\nВыберите: ";
            cin >> sortChoice;
            cin.ignore();
            if (sortChoice == 1) sortByName(contacts);
            else if (sortChoice == 2) sortByBirthday(contacts);
            else cout << "Неверный выбор.\n";
            break;
        }
        case 7:
            saveToFile(contacts, filename);
            cout << "До свидания!\n";
            break;
        case 8:
            showByGroup(contacts);
            break;
        case 9:
            showStatistics(contacts);
            break;
        default:
            cout << "Неверный пункт меню.\n";
        }
    } while (choice != 7);

    return 0;
}
/*=============================================================================================================================================================
Данный код представляет класс ini_parser, который позволяет парсить ini-файлы и извлекать значения ключей.
Конструктор класса принимает имя файла в качестве аргумента, открывает файл и начинает построчно его обрабатывать.
Если файл не удается открыть, генерируется исключение std::runtime_error.
Внутри цикла while происходит чтение каждой строки, которая обрабатывается в соответствии с ее содержанием.
Если строка пустая или начинается с символа ';', то она игнорируется. Если же строка представляет собой заголовок секции, то создается новая секция.
Если строка содержит ключ и его значение, то они извлекаются и сохраняются в соответствующей секции.
Если же строка содержит ошибочный синтаксис, генерируется исключение std::runtime_error.
Функция get_value шаблонизирована и принимает в качестве аргумента строку в формате "секция.ключ".
Функция проверяет наличие секции и ключа внутри него и возвращает значение, которое было сохранено ранее.
Если секция или ключ не были найдены, генерируется исключение std::runtime_error.
===============================================================================================================================================================*/

// Подключаем необходимые библиотеки
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

// Определяем класс ini_parser
class ini_parser
{
public:
    // Конструктор класса, принимающий имя файла
    ini_parser(const std::string &filename)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("Не удалось открыть файл");
        }
        std::string line;
        std::string current_section;
        size_t line_number = 0;

        while (std::getline(file, line))
        {
            ++line_number;

            // Удаляем любые пробельные символы в конце строки
            line.erase(line.find_last_not_of(" \t") + 1);

            if (line.empty() || line.front() == ';')
            {
                continue; // Пропускаем пустые строки и комментарии
            }

            if (line.front() == '[' && line.back() == ']')
            {
                current_section = line.substr(1, line.size() - 2);
                continue;
            }

            auto delimiter_pos = line.find('=');
            if (delimiter_pos == std::string::npos)
            {
                throw std::runtime_error("Неверный синтаксис на строке " + std::to_string(line_number));
            }

            auto key = line.substr(0, delimiter_pos);
            auto value = line.substr(delimiter_pos + 1);

            // Удаляем любые пробельные символы вокруг ключа и значения
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));

            sections_[current_section][key] = value;
        }
    }

    // Шаблонный метод get_value для получения значения по ключу
    template <typename T>
    T get_value(const std::string &section_and_key) const
    {
        auto pos = section_and_key.find('.');
        if (pos == std::string::npos)
        {
            throw std::invalid_argument("Неверный формат секции/ключа");
        }

        auto section = section_and_key.substr(0, pos);
        auto key = section_and_key.substr(pos + 1);

        if (sections_.count(section) == 0 || sections_.at(section).count(key) == 0)
        {
            throw std::runtime_error("Секция/ключ не найдены");
        }

        T result;
        std::istringstream iss(sections_.at(section).at(key));
        iss >> result;

        if (iss.fail())
        {
            throw std::runtime_error("Неверное значение для секции/ключа");
        }

        return result;
    }

private:
    // Хранение секций и ключей в неупорядоченных ассоциативных массивах
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> sections_;
};

// Главная функция программы
int main()
{
    ini_parser parser("test.ini");
    auto value = parser.get_value<int>("Section1.var1");
    std::cout << value;
};
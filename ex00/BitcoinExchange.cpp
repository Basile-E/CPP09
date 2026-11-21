#include "BitcoinExchange.hpp"

#include <cfloat>
#include <cerrno>
#include <cstdlib>
#include <fstream>
#include <iostream>

BitcoinExchange::BitcoinExchange()
{
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other)
    : rates_(other.rates_)
{
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other)
{
    if (this != &other)
        rates_ = other.rates_;
    return *this;
}

BitcoinExchange::~BitcoinExchange()
{
}

std::string BitcoinExchange::trim(const std::string &value)
{
    std::string::size_type first = value.find_first_not_of(" \t\r\n");
    if (first == std::string::npos)
        return "";
    std::string::size_type last = value.find_last_not_of(" \t\r\n");
    return value.substr(first, last - first + 1);
}

bool BitcoinExchange::isValidDate(const std::string &date)
{
    if (date.size() != 10 || date[4] != '-' || date[7] != '-')
        return false;
    for (std::string::size_type i = 0; i < date.size(); ++i)
    {
        if (i != 4 && i != 7 && (date[i] < '0' || date[i] > '9'))
            return false;
    }

    int year = std::atoi(date.substr(0, 4).c_str());
    int month = std::atoi(date.substr(5, 2).c_str());
    int day = std::atoi(date.substr(8, 2).c_str());
    if (year == 0 || month < 1 || month > 12)
        return false;

    static const int daysInMonth[] =
        {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int maxDay = daysInMonth[month - 1];
    bool leapYear = (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
    if (month == 2 && leapYear)
        maxDay = 29;
    return day >= 1 && day <= maxDay;
}

BitcoinExchange::NumberStatus BitcoinExchange::parseNumber(
    const std::string &text, double &value)
{
    if (text.empty())
        return NUMBER_INVALID;

    std::string::size_type i = 0;
    if (text[i] == '+' || text[i] == '-')
        ++i;

    bool hasDigits = false;
    while (i < text.size() && text[i] >= '0' && text[i] <= '9')
    {
        hasDigits = true;
        ++i;
    }
    if (i < text.size() && text[i] == '.')
    {
        ++i;
        while (i < text.size() && text[i] >= '0' && text[i] <= '9')
        {
            hasDigits = true;
            ++i;
        }
    }
    if (!hasDigits)
        return NUMBER_INVALID;

    if (i < text.size() && (text[i] == 'e' || text[i] == 'E'))
    {
        ++i;
        if (i < text.size() && (text[i] == '+' || text[i] == '-'))
            ++i;
        std::string::size_type exponentStart = i;
        while (i < text.size() && text[i] >= '0' && text[i] <= '9')
            ++i;
        if (i == exponentStart)
            return NUMBER_INVALID;
    }
    if (i != text.size())
        return NUMBER_INVALID;

    errno = 0;
    char *end = NULL;
    value = std::strtod(text.c_str(), &end);
    if (end == text.c_str() || *end != '\0' || value != value)
        return NUMBER_INVALID;
    if (errno == ERANGE && (value == HUGE_VAL || value == -HUGE_VAL))
        return NUMBER_TOO_LARGE;
    if (value > DBL_MAX || value < -DBL_MAX)
        return NUMBER_INVALID;
    return NUMBER_VALID;
}

bool BitcoinExchange::loadDatabase(const std::string &path)
{
    std::ifstream file(path.c_str());
    if (!file.is_open())
    {
        std::cerr << "Error: could not open file." << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::string row = trim(line);
        if (row.empty() || row == "date,exchange_rate")
            continue;

        std::string::size_type separator = row.find(',');
        if (separator == std::string::npos
            || row.find(',', separator + 1) != std::string::npos)
        {
            std::cerr << "Error: bad database input => " << line << std::endl;
            continue;
        }

        std::string date = trim(row.substr(0, separator));
        std::string valueText = trim(row.substr(separator + 1));
        double rate;
        NumberStatus rateStatus = parseNumber(valueText, rate);
        if (!isValidDate(date) || rateStatus == NUMBER_INVALID
            || rateStatus == NUMBER_TOO_LARGE || rate < 0)
        {
            std::cerr << "Error: bad database input => " << line << std::endl;
            continue;
        }
        rates_[date] = rate;
    }

    if (rates_.empty())
    {
        std::cerr << "Error: database contains no valid exchange rates."
                  << std::endl;
        return false;
    }
    return true;
}

bool BitcoinExchange::getRate(const std::string &date, double &rate) const
{
    std::map<std::string, double>::const_iterator it = rates_.upper_bound(date);
    if (it == rates_.begin())
        return false;
    --it;
    rate = it->second;
    return true;
}

bool BitcoinExchange::processInput(const std::string &path) const
{
    std::ifstream file(path.c_str());
    if (!file.is_open())
    {
        std::cerr << "Error: could not open file." << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::string row = trim(line);
        if (row == "date | value")
            continue;

        std::string::size_type separator = row.find('|');
        if (separator == std::string::npos
            || row.find('|', separator + 1) != std::string::npos)
        {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }

        std::string date = trim(row.substr(0, separator));
        std::string valueText = trim(row.substr(separator + 1));
        double value;
        NumberStatus valueStatus = parseNumber(valueText, value);
        if (!isValidDate(date) || valueStatus == NUMBER_INVALID)
        {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }
        if (valueStatus == NUMBER_TOO_LARGE)
        {
            if (value < 0)
                std::cerr << "Error: not a positive number." << std::endl;
            else
                std::cerr << "Error: too large a number." << std::endl;
            continue;
        }
        if (value < 0)
        {
            std::cerr << "Error: not a positive number." << std::endl;
            continue;
        }
        if (valueText.size() > 0
            && trim(valueText)[0] == '-'
            && value == 0)
        {
            std::cerr << "Error: not a positive number." << std::endl;
            continue;
        }
        if (value > 1000)
        {
            std::cerr << "Error: too large a number." << std::endl;
            continue;
        }

        double rate;
        if (!getRate(date, rate))
        {
            std::cerr << "Error: no exchange rate available for date => "
                      << date << std::endl;
            continue;
        }
        double result = value * rate;
        if (result != result || result > DBL_MAX)
        {
            std::cerr << "Error: too large a number." << std::endl;
            continue;
        }
        std::cout << date << " => " << value << " = " << result << std::endl;
    }
    return true;
}

#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <map>
#include <string>

class BitcoinExchange
{
public:
    BitcoinExchange();
    BitcoinExchange(const BitcoinExchange &other);
    BitcoinExchange &operator=(const BitcoinExchange &other);
    ~BitcoinExchange();

    bool loadDatabase(const std::string &path);
    bool processInput(const std::string &path) const;

private:
    enum NumberStatus
    {
        NUMBER_INVALID,
        NUMBER_VALID,
        NUMBER_TOO_LARGE
    };

    std::map<std::string, double> rates_;

    static std::string trim(const std::string &value);
    static bool isValidDate(const std::string &date);
    static NumberStatus parseNumber(const std::string &text, double &value);
    bool getRate(const std::string &date, double &rate) const;
};

#endif

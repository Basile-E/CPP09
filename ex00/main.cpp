#include "BitcoinExchange.hpp"

#include <fstream>
#include <iostream>
#include <string>

static std::string getDatabasePath(const std::string &executablePath)
{
    std::string directory(".");
    std::string::size_type slash = executablePath.find_last_of('/');

    if (slash != std::string::npos)
        directory = executablePath.substr(0, slash);
    if (directory.empty())
        directory = "/";

    std::string executableRelativePath = directory + "/data/data.csv";
    std::ifstream database(executableRelativePath.c_str());
    if (database.is_open())
        return executableRelativePath;

    return "data/data.csv";
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Error: could not open file." << std::endl;
        return 1;
    }

    BitcoinExchange exchange;
    if (!exchange.loadDatabase(getDatabasePath(argv[0])))
        return 1;
    if (!exchange.processInput(argv[1]))
        return 1;
    return 0;
}

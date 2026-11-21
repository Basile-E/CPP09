#include "RPN.hpp"

#include <climits>
#include <sstream>
#include <stack>

RPN::RPN()
{
}

RPN::RPN(const RPN &other)
{
    (void)other;
}

RPN &RPN::operator=(const RPN &other)
{
    (void)other;
    return *this;
}

RPN::~RPN()
{
}

bool RPN::applyOperator(char op, long left, long right,
                        long &result, std::string &error)
{
    if (op == '+')
    {
        if ((right > 0 && left > LONG_MAX - right)
            || (right < 0 && left < LONG_MIN - right))
        {
            error = "integer overflow";
            return false;
        }
        result = left + right;
    }
    else if (op == '-')
    {
        if ((right < 0 && left > LONG_MAX + right)
            || (right > 0 && left < LONG_MIN + right))
        {
            error = "integer overflow";
            return false;
        }
        result = left - right;
    }
    else if (op == '*')
    {
        if (left > 0)
        {
            if ((right > 0 && left > LONG_MAX / right)
                || (right < 0 && right < LONG_MIN / left))
            {
                error = "integer overflow";
                return false;
            }
        }
        else if (left < 0)
        {
            if ((right > 0 && left < LONG_MIN / right)
                || (right < 0 && left < LONG_MAX / right))
            {
                error = "integer overflow";
                return false;
            }
        }
        result = left * right;
    }
    else if (op == '/')
    {
        if (right == 0)
        {
            error = "division by zero";
            return false;
        }
        if (left == LONG_MIN && right == -1)
        {
            error = "integer overflow";
            return false;
        }
        result = left / right;
    }
    else
    {
        error = "unsupported operator";
        return false;
    }
    return true;
}

bool RPN::evaluate(const std::string &expression, long &result,
                   std::string &error) const
{
    std::istringstream input(expression);
    std::stack<long> values;
    std::string token;

    while (input >> token)
    {
        if (token.size() == 1 && token[0] >= '0' && token[0] <= '9')
        {
            values.push(token[0] - '0');
            continue;
        }

        if (token.size() != 1
            || (token[0] != '+' && token[0] != '-'
                && token[0] != '*' && token[0] != '/'))
        {
            error = "invalid token";
            return false;
        }
        if (values.size() < 2)
        {
            error = "not enough operands";
            return false;
        }

        long right = values.top();
        values.pop();
        long left = values.top();
        values.pop();
        long operationResult;
        if (!applyOperator(token[0], left, right, operationResult, error))
            return false;
        values.push(operationResult);
    }

    if (values.size() != 1)
    {
        error = "invalid expression";
        return false;
    }
    result = values.top();
    return true;
}

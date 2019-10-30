#include "functions.hpp"

/**
 * Operator set after prepare work
 */
std::set<char> operators({'|', '&', '!', '*', '+', '-', '='}); 

void prepareWork(std::string &s)
{
    size_t plc = 0;
    while ((plc = s.find("&&")) != std::string::npos)
    {
        s = s.replace(plc, 2, "&");
    }
    while ((plc = s.find("||")) != std::string::npos)
    {
        s = s.replace(plc, 2, "|");
    }
    while ((plc = s.find("->")) != std::string::npos)
    {
        s = s.replace(plc, 2, "-");
    }
    while ((plc = s.find("==")) != std::string::npos)
    {
        s = s.replace(plc, 2, "=");
    }

    for (int i = 1; i < static_cast<int>(s.size()); i++)
    {
        while (s[i] == '!' && s[i - 1] == '!')
        {
            s = s.substr(0, i - 1) + s.substr(std::min(i + 1, static_cast<int>(s.size())));
        }
    }
}

bool isLegal(const std::string &expr)
{
    // Start error
    if (expr[0] != '!' && expr[0] != '(' && !isalpha(expr[0])) return false;
    // Tail error
    if (expr[expr.size() - 1] != ')' && !isalpha(expr[expr.size() - 1])) return false;
    
    for (int i = 1; i < static_cast<int>(expr.size()); i++)
    {
        if (expr[i - 1] == '!' || expr[i] == '!') continue;
        // Consequent operators
        if (operators.count(expr[i]) && operators.count(expr[i - 1])) return false;
        // Consequent variables
        if (isalpha(expr[i]) && isalpha(expr[i - 1])) return false;
    }

    return true;
}

int getPriority(const char &op)
{
    switch (op)
    {
        case '=': return -3;
        case '-': return -2;
        case '|': return -1;
        case '&': return 0;
        case '!': return 1;
        default: return -10;
    }
}

std::string toReverseNotation(const std::string &expr)
{
    if (!isLegal(expr))
    {
        throw std::overflow_error("Illegal Expression!");
    }

    // Result
    std::string res;
    // Stack of operators, Stack for cache data
    std::stack<char> soper, smid;
    // For every char in expressions
    for (int i = 0; i < static_cast<int>(expr.size()); i++)
    {
        const char &now = expr[i];
        // variables
        if (isalpha(now))
        {
            smid.push(now);
        }
        // operators
        else if (operators.count(now))
        {
            int flag = 0;
            while (!flag)
            {
                // Empty or top with (
                if (soper.empty() || soper.top() == '(')
                {
                    soper.push(now); flag = 1;
                }
                // Higher priority than top
                else if (getPriority(now) > getPriority(soper.top()))
                {
                    soper.push(now); flag = 1;
                }
                // Lower
                else
                {
                    smid.push(soper.top());
                    soper.pop();
                }
            }
        }
        // braces
        else if (now == '(')
        {
            soper.push(now);
        }
        else if (now == ')')
        {
            // pop!!!
            while (soper.size() && soper.top() != '(')
            {
                smid.push(soper.top());
                soper.pop();
            }
            if (soper.top() == '(') soper.pop();
        }
        else
        {
            throw std::overflow_error("Illegal Expression!");
        }
    }
    // Summary
    while (soper.size())
    {
        smid.push(soper.top());
        soper.pop();
    }
    while (smid.size())
    {
        res += smid.top();
        smid.pop();
    }
    std::reverse(res.begin(), res.end());

    return res;
}

bool getResultOf(const std::string &expr, std::map<char, int> &valueof)
{
    // Cache
    std::stack<bool> s;
    for (int i = 0; i < static_cast<int>(expr.size()); i++)
    {
        const char &now = expr[i];
        // Variables
        if (isalpha(now))
        {
            s.push(valueof[now]);
        }
        // Operators
        else if (now == '!')
        {
            if (s.size() < 1)
            {
                throw std::overflow_error("Illegal Expression!");
            }
            bool top = s.top(); s.pop();
            s.push(!top);
        }
        else if (now == '&' || now == '*')
        {
            if (s.size() < 2)
            {
                throw std::overflow_error("Illegal Expression!");
            }
            bool a = s.top(); s.pop();
            bool b = s.top(); s.pop();
            s.push(a && b);
        }
        else if (now == '|' || now == '+')
        {
            if (s.size() < 2)
            {
                throw std::overflow_error("Illegal Expression!");
            }
            bool a = s.top(); s.pop();
            bool b = s.top(); s.pop();
            s.push(a || b);
        }
        else if (now == '-')
        {
            if (s.size() < 2)
            {
                throw std::overflow_error("Illegal Expression!");
            }
            bool a = s.top(); s.pop();
            bool b = s.top(); s.pop();
            s.push((!a) || b);
        }
        else if (now == '=')
        {
            if (s.size() < 2)
            {
                throw std::overflow_error("Illegal Expression!");
            }
            bool a = s.top(); s.pop();
            bool b = s.top(); s.pop();
            s.push(a == b);
        }
    }
    if (s.size() == 1)
    {
        return s.top();
    }
    else
    {
        throw std::overflow_error("Illegal Expression!");
    }
}

int solve()
{
    std::cout << "Support: (, ), +(||, |), *(&&, &), ->(-), ==(=);\n\n";
    std::cout << "Please input the expression:\n";

    std::string s;
    std::cin >> s;

    prepareWork(s);
    // Try to get a reverse polish notation from s
    try
    {
        s = toReverseNotation(s);
        #ifdef DEBUG
            std::cout << s << "\n";
        #endif
    }
    catch(const std::exception& e)
    {
        // Illegal expression
        std::cerr << e.what() << '\n';
        return -1;
    }

    // For every permutation of variables
    std::map<char, int> varlist;
    // Variable list
    std::vector<char> var;

    // Get every char of s
    for (int i = 0; i < static_cast<int>(s.size()); i++)
    {
        if (isalpha(s[i]) && !varlist.count(s[i]))
        {
            var.emplace_back(s[i]);
            varlist[s[i]] = 0;
        }
    }

    // Cache the number of variables
    int numofvar = static_cast<int>(var.size());
    // Result of main DNF and main CNF
    std::vector<std::vector<int>> dnf, cnf;

    // Title of truth table
    std::cout << std::endl;
    for (int i = 0; i < numofvar; i++)
    {
        std::cout << std::left << std::setw(4) << var[i];
    }
    std::cout << std::left << std::setw(4) << "Res\n";
    // For every line of truth table
    for (int numof1 = 0; numof1 <= numofvar; numof1++)
    {
        std::vector<int> buffer(numofvar, 0);
        for (int i = 0; i < numof1; i++)
        {
            buffer[numofvar - 1 - i] = 1;
        }

        do
        {
            for (int i = 0; i < numofvar; i++)
            {
                varlist[var[i]] = buffer[i];
                std::cout << std::left << std::setw(4) << varlist[var[i]];
            }
            try
            {
                // Get result of current form
                int res = getResultOf(s, varlist);

                std::vector<int> truth(numofvar);
                for (int i = 0; i < numofvar; i++)
                {
                    truth[i] = varlist[var[i]];
                }
                // CNF
                if (res == 0)
                {
                    cnf.emplace_back(truth);
                }
                // DNF
                else if (res == 1)
                {
                    dnf.emplace_back(truth);
                }
                std::cout << " " << std::left << std::setw(4) << res << '\n';
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
                return 0;
            }
        }
        while (next_permutation(buffer.begin(), buffer.end()));
    }

    // Print DNF
    std::cout << "Disjunctive normal form:\n";
    int dnflen = static_cast<int>(dnf.size());
    if (dnflen == 0)
    {
        std::cout << "None\n";
    }
    for (int i = 0; i < dnflen; i++)
    {
        std::cout << "(";
        for (int j = 0; j < numofvar; j++)
        {
            std::cout << (dnf[i][j] ? " " : "!") << var[j] << (j != numofvar - 1 ? " & " : i < dnflen - 1 ? ") | " : ") \n");
        }
    }

    // Print CNF
    std::cout << "Canonical normal form:\n";
    int cnflen = static_cast<int>(cnf.size());
    if (cnflen == 0)
    {
        std::cout << "None\n";
    }
    for (int i = 0; i < cnflen; i++)
    {
        std::cout << "(";
        for (int j = 0; j < numofvar; j++)
        {
            std::cout << (cnf[i][j] ? "!" : " ") << var[j] << (j != numofvar - 1 ? " | " : i < cnflen - 1 ? ") & " : ") \n");
        }
    }

    return 0;
}

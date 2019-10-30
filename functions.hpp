#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

// I/O
#include <iostream>
#include <iomanip>
#include <cctype>

// Exceptions

#include <exception>
#include <stdexcept>

// STL

#include <map>
#include <set>
#include <stack>
#include <vector>
#include <string>
#include <algorithm>

/**
 * Prepare work for expression s
 * 
 * \param s Expressions from raw input
 */
void prepareWork(std::string &s);

/**
 * Primary check of expression expr
 * 
 * \param expr Expression after parpare work
 * \return Expression is legal
 */
bool isLegal(const std::string &expr);

/**
 * Get priority of operators
 * 
 * \param op Queried operator
 * \return Priority of operator
 */
int getPriority(const char &op);

/**
 * Get the reverse polish notation of expressions to simptify calculation
 * 
 * \param expr Mid notation expression
 * \return Reverse notation of param expr
 */
std::string toReverseNotation(const std::string &expr);

/**
 * Get result of expression with values
 * 
 * \param expr Reverse notation expr
 * \param valueof Value of every variables
 * \return Result of expr
 */
bool getResultOf(const std::string &expr, std::map<char, int> &valueof);

/**
 * Solve the problem once
 * 
 * \return Runtime status
 */
int solve();

#endif

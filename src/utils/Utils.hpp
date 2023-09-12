#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <cctype>
#include <exception>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "colors.hpp"

#define VERBOSE false

class Request;

std::string getContentDisposition(const Request &request,
                                  const std::string &key);
std::string trim(const std::string &s, const std::string &trim);
char last(std::string string);

template <typename Type>
bool isNumber(Type cont);

template <typename Type>
void printVerbose(const std::string &call, Type val);

template <typename Content>
std::ostream &operator<<(std::ostream &stream,
                         const std::vector<Content> container);

template <typename Content>
std::ostream &operator<<(std::ostream &stream,
                         const std::set<Content> container);

template <typename First, typename Second>
std::ostream &operator<<(std::ostream &stream,
                         const std::map<First, Second> container);

template <typename Key, typename Value>
std::map<Value, Key> swapColumns(std::map<Key, Value> input);

template <typename Type>
std::string toString(Type obj);

#include "IsNumber.tpp"
#include "PrintVerbose.tpp"
#include "StreamOperators.tpp"
#include "SwapColumns.tpp"
#include "ToString.tpp"

#endif  // UTILS_HPP_

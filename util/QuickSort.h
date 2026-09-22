// This template function is a QuickSort function that is used to sort the
// displayed Customers, Employees, and Investments.

#ifndef QUICK_SORT_H
#define QUICK_SORT_H

#include "Customer.h"
#include "Employee.h"
#include "Investment.h"

#include <vector>

namespace Custom {

// Each of these lambda functions are used to template the type of comparison,
// allowing for a quickSort for different types, and in different directions.
inline auto customerName = [](const Customer &a, const Customer &b) {
  return a.lastName < b.lastName;
};
inline auto reverseCustomerName = [](const Customer &a, const Customer &b) {
  return a.lastName > b.lastName;
};
inline auto customerId = [](const Customer &a, const Customer &b) {
  return a.customerID < b.customerID;
};
inline auto reverseCustomerId = [](const Customer &a, const Customer &b) {
  return a.customerID > b.customerID;
};

inline auto investmentWorth = [](const Investment &a, const Investment &b) {
  return a.currentInvestmentWorth < b.currentInvestmentWorth;
};
inline auto reverseInvestmentWorth = [](const Investment &a,
                                        const Investment &b) {
  return a.currentInvestmentWorth > b.currentInvestmentWorth;
};

inline auto employeeName = [](const Employee &a, const Employee &b) {
  return a.lastName < b.lastName;
};
inline auto reverseEmployeeName = [](const Employee &a, const Employee &b) {
  return a.lastName > b.lastName;
};

inline auto employeeId = [](const Employee &a, const Employee &b) {
  return a.accountID < b.accountID;
};
inline auto reverseEmployeeId = [](const Employee &a, const Employee &b) {
  return a.accountID > b.accountID;
};

// The actual template quickSort function. We pass the vector by reference to
// minimize unnecessary copying, and the comparator to tell the quicksort
// function how we want to sort.
template <typename T, typename Comparator>
void quickSort(std::vector<T> &vect, Comparator comparator);

} // namespace Custom

// The function definitions
#include "QuickSort.tpp"

#endif

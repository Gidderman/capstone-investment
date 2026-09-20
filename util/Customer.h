// This is a custom data structure used to hold information regarding a singular
// customer account. It includes a list of investments associated with the
// customer, and an accountID that matches the accountID of the employee that
// manages the account.

#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "Investment.h"

#include <string>
#include <vector>

enum ACCOUNT_TYPE { BROKERAGE, RETIREMENT };

struct Customer {
  int customerID;
  std::string firstName;
  std::string lastName;
  std::string phoneNum;
  std::string email;
  std::string
      dateAccountOpened; // TODO: Introduce a date data type, find a library
  ACCOUNT_TYPE accountType;
  float uninvestedFunds;
  std::vector<Investment> investments;
  int accountID;
};

#endif

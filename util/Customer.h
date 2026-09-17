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
  int phoneNum;
  std::string email;
  std::string
      dateAccountOpened; // TODO: Introduce a date data type, find a library
  ACCOUNT_TYPE accountType;
  float uninvestedFunds;
  std::vector<Investment> investments;
  int accountID;
};

#endif

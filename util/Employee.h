#ifndef TRADER_H
#define TRADER_H

#include "Role.h"

#include <string>

struct Employee {
  int accountID;
  std::string firstName;
  std::string lastName;
  ROLE role;
};

#endif

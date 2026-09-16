#ifndef TRADER_H
#define TRADER_H

#include "Credentials.h"
#include "Role.h"

#include <string>

struct Employee {
  int accountID;
  std::string firstName;
  std::string lastName;
  Credentials credentials;
  ROLE role;
};

#endif

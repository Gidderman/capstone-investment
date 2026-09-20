// This is a custom data structure that holds the information of an individual
// employee. The credentials of a given employee are stored seperately in order
// to keep the sensitive information more secure.

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

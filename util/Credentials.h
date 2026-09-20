// This is a custom data structure that is used to pass credential information
// where it is needed. This information is kept seperately from the Employee
// data structure to ensure that the sensitive user information is not passed
// around where it isn't needed.

#ifndef CREDENTIALS_H
#define CREDENTIALS_H

#include <string>

struct Credentials {
  int accountID;
  std::string username;
  std::string password;
  std::string salt;
  bool accountLocked;
};

#endif

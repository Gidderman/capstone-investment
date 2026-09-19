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

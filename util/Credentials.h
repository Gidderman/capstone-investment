#ifndef CREDENTIALS_H
#define CREDENTIALS_H

#include <string>

struct Credentials {
  std::string username;
  std::string password;
  std::string salt;
};

#endif

#ifndef LOG_IN_SERVICE_H
#define LOG_IN_SERVICE_H

#include "DataManager.h"
#include "Role.h"

#include <string>

class LogInService {
private:
  DataManager dataManager;

  std::string hashPassword(std::string rawPassword);
  bool authenticate(std::string enteredPassword, std::string validPassword);

public:
  LogInService();
  ~LogInService();
  ROLE handleLogInAttempt(std::string enteredUsername,
                          std::string enteredPassword);
};

#endif

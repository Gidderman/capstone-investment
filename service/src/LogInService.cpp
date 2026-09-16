#include "LogInService.h"

LogInService::LogInService() {}

LogInService::~LogInService() {}

ROLE LogInService::handleLogInAttempt(std::string enteredUsername,
                                      std::string enteredPassword) {
  Employee storedEmployee = dataManager.getEmployeeByUsername(enteredUsername);

  if (authenticate(hashPassword(enteredPassword),
                   storedEmployee.credentials.password)) {
    return storedEmployee.role;
  } else {
    return ROLE::INVALID;
  }
}

std::string LogInService::hashPassword(std::string rawPassword) {
  // TODO: Actually salt and hash the password
  return rawPassword;
}

bool LogInService::authenticate(std::string enteredPassword,
                                std::string validPassword) {
  return enteredPassword == validPassword;
}

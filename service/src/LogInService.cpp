#include "LogInService.h"

LogInService::LogInService() {}

LogInService::~LogInService() {}

std::tuple<Employee, ROLE>
LogInService::handleLogInAttempt(std::string enteredUsername,
                                 std::string enteredPassword) {
  std::tuple<Employee, Credentials> logInInformation =
      dataManager.getEmployeeByUsername(enteredUsername);

  if (authenticate(hashPassword(enteredPassword),
                   std::get<1>(logInInformation).password)) {
    return std::tuple<Employee, ROLE>{std::get<0>(logInInformation),
                                      std::get<0>(logInInformation).role};
  } else {
    return std::tuple<Employee, ROLE>{std::get<0>(logInInformation),
                                      ROLE::INVALID};
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

// This class implements LogInService.h, see that header file for further
// information.

// TODO:
// THIS CLASS CURRENTLY DOES NOT PERFORM ANY PASSWORD HASHING AND REQUIRES MORE
// WORK, NOR DOES IT HANDLE FIRST TIME LOG INS.

#include "LogInService.h"

LogInService::LogInService() {}

LogInService::~LogInService() {}

// This function gets the associated employee and credentials based on the
// entred username, and returns a tuple of the employee and the allowed ROLE
// if the log in was successful.
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

// Hashes the entered password using the stored salt value
std::string LogInService::hashPassword(std::string rawPassword) {
  // TODO: Actually salt and hash the password
  return rawPassword;
}

// Authenticates the hashed, salted password against the stored salted, hashed
// password
bool LogInService::authenticate(std::string enteredPassword,
                                std::string validPassword) {
  return enteredPassword == validPassword;
}

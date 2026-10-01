// This class implements LogInService.h, see that header file for further
// information.

// TODO:
// THIS CLASS CURRENTLY DOES NOT PERFORM ANY PASSWORD HASHING AND REQUIRES MORE
// WORK, NOR DOES IT HANDLE FIRST TIME LOG INS.

#include "LogInService.h"

#include <iostream>

LogInService::LogInService(DataManager &dataManager)
    : dataManager(dataManager), logInAttempts(0) {}

LogInService::~LogInService() {}

// Returns two bools, one for if the account has a valid password or needs
// to make one, the second bool is for if the account is locked.
std::tuple<bool, bool>
LogInService::doesAccountHaveValidCredentials(std::string username) {
  // TODO: THIS
  // Create a member variable for prospective employee and the credentials, that
  // is stored and can be checked in later functions to limit database calls.
}

// This function gets the associated employee and credentials based on the
// entred username, and returns a tuple of the employee and the allowed ROLE
// if the log in was successful.
std::tuple<Employee, ROLE>
LogInService::handleLogInAttempt(std::string enteredUsername,
                                 std::string enteredPassword) {
  std::tuple<Employee, Credentials> logInInformation =
      dataManager.getEmployeeByUsername(enteredUsername);

  std::cout << "LogInService::handleLogInAttempt - attempting log in"
            << std::endl;

  if (std::get<0>(logInInformation).accountID == -1) {
    throw std::logic_error("LogInService::handleLogInAttempt -> " +
                           dataManager.getErrorInfo());
  }

  if (authenticate(hashPassword(enteredPassword),
                   std::get<1>(logInInformation).password)) {

    std::cout << "Returning employee " << std::get<0>(logInInformation).lastName
              << " with a role of " << std::get<0>(logInInformation).role
              << std::endl;

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

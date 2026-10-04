// This class implements LogInService.h, see that header file for further
// information.

// TODO:
// THIS CLASS CURRENTLY DOES NOT PERFORM ANY PASSWORD HASHING AND REQUIRES MORE
// WORK, NOR DOES IT HANDLE FIRST TIME LOG INS.

#include "LogInService.h"
#include "Credentials.h"
#include "Employee.h"

#include <iostream>

LogInService::LogInService(DataManager &dataManager)
    : dataManager(dataManager), logInAttempts(0), countAttemptedLogIns(true),
      employeeAttemptingToLogIn(Employee()),
      credentialsForAttemptingToLogInEmployee(Credentials()) {}

LogInService::~LogInService() {}

// Returns three bools, first is true if an account exists does not exist, the
// second for if the account needs to make a password, the third if the account
// is locked. If both are true that indicates there is not an account associated
// with that username, if both are false then the log in can commence
std::tuple<bool, bool, bool>
LogInService::doesAccountHaveValidCredentials(std::string username) {
  std::tuple<Employee, Credentials> logInInformation =
      dataManager.getEmployeeByUsername(username);

  if (std::get<0>(logInInformation).accountID == -1) {
    // There is not an account with that username.
    return {true, false, false};
  } else if (std::get<1>(logInInformation).password == "NEW_PASSWORD_NEEDED") {
    // User needs to create a password, we store the credentials and employee
    // information temporarily in the service layer to limit database calls.
    employeeAttemptingToLogIn = std::get<0>(logInInformation);
    credentialsForAttemptingToLogInEmployee = std::get<1>(logInInformation);
    return {false, true, false};
  } else if (std::get<1>(logInInformation).accountLocked == true) {
    // Account is locked
    return {false, false, true};
  }

  // We store the credentials and employee information temporarily to limit
  // database calls
  employeeAttemptingToLogIn = std::get<0>(logInInformation);
  credentialsForAttemptingToLogInEmployee = std::get<1>(logInInformation);

  // A valid log in is commencing
  return {false, false, false};
}

// This function gets the associated employee and credentials based on the
// entred username, and returns a tuple of the employee and the allowed ROLE
// if the log in was successful.
std::optional<std::tuple<Employee, ROLE>>
LogInService::handleLogInAttempt(std::string enteredPassword) {
  std::cout << "LogInService::handleLogInAttempt - attempting log in"
            << std::endl;

  if (authenticate(hashPassword(enteredPassword),
                   credentialsForAttemptingToLogInEmployee.password)) {
    if (countAttemptedLogIns) {
      logInAttempts = 0;
    }

    return std::tuple<Employee, ROLE>{employeeAttemptingToLogIn,
                                      employeeAttemptingToLogIn.role};
  } else {
    // Increment the number of log in attempts if set to log the log in attempts
    if (countAttemptedLogIns) {
      logInAttempts++;
    }

    // We have had too many invalid log in attempts. Lock the account
    if (logInAttempts >= 3) {
      credentialsForAttemptingToLogInEmployee.accountLocked = true;

      if (!dataManager.updateEmployeeCredentials(
              credentialsForAttemptingToLogInEmployee.accountID,
              credentialsForAttemptingToLogInEmployee)) {
        throw std::logic_error("LogInService::handleLogInAttempt -> " +
                               dataManager.getErrorInfo());
      }

      // We return a blank employee with an invalid role to notify the
      // MasterController to start displaying error messages.
      return std::tuple<Employee, ROLE>{Employee(), INVALID};
    }

    return {};
  }
}

bool LogInService::verifyPasswordsMatch(std::string enteredPassword,
                                        std::string verificationPassword) {
  return enteredPassword == verificationPassword;
}

bool LogInService::verifyPasswordComplexityRequirements(
    std::string enteredPassword, std::string verificationPassword) {
  if (enteredPassword.size() < 8 || enteredPassword.size() > 12) {
    return false;
  }

  for (char c : enteredPassword) {
    int ascii = int(c);

    // Valid characters for a password are:
    // a-z A-Z 0-9 ! @ # $ % & *
    if (!((ascii >= 65 && ascii <= 90) || (ascii >= 97 && ascii <= 122) ||
          (ascii >= 48 && ascii <= 57) || ascii == 33 || ascii == 64 ||
          ascii == 35 || ascii == 36 || ascii == 37 || ascii == 38 ||
          ascii == 42)) {
      // The entered character is not a valid character
      return false;
    }
  }

  // All the characters where valid and the password was the within the correct
  // length;
  return true;
}

void LogInService::createPassword(std::string enteredPassword) {
  std::string newSalt = "123"; // TODO: GENERATE A SALT;
  credentialsForAttemptingToLogInEmployee.salt = newSalt;

  std::string saltedPassword = enteredPassword; // TODO: salt the password
  credentialsForAttemptingToLogInEmployee.password =
      hashPassword(saltedPassword);

  if (!dataManager.updateEmployeeCredentials(
          credentialsForAttemptingToLogInEmployee.accountID,
          credentialsForAttemptingToLogInEmployee)) {
    throw std::logic_error("LogInService::createPassword -> " +
                           dataManager.getErrorInfo());
  }
}

void LogInService::setLogAttemptedLogIns(bool countLogIns) {
  this->countAttemptedLogIns = countLogIns;
}

//******************PRIVATE FUNCTIONS************************************
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

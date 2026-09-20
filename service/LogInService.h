// This class handles the logic behind a log in. Specifically it authenticates
// the users credentials. It salts and hashes the user entered password and
// compares it to the salted, hashed password stored in the database. If the log
// in is valid then it returns a tuple of the employee and associated role. This
// is to prevent the credentials of a user, to include the stored password and
// salt from being passed around more than necessary, while also not relying on
// the data from the employee struct without authentication for role access in
// the controllers. Additionally, if it is the users first time loggin in,
// then it will have the user create a password.

// TODO:
// THIS CLASS CURRENTLY DOES NOT PERFORM ANY PASSWORD HASHING AND REQUIRES MORE
// WORK, NOR DOES IT HANDLE FIRST TIME LOG INS.

#ifndef LOG_IN_SERVICE_H
#define LOG_IN_SERVICE_H

#include "DataManager.h"
#include "Role.h"

#include <string>
#include <tuple>

class LogInService {
private:
  DataManager dataManager;

  // This function hashes the password provided by the password, utilizing the
  // salt stored within the database that is associated with the entered
  // username.
  std::string hashPassword(std::string rawPassword);

  // This function authenticates the user based on the entered password and the
  // stored password.
  bool authenticate(std::string enteredPassword, std::string validPassword);

public:
  LogInService();
  ~LogInService();
  // This is the entry point into this class, accepting the entered username and
  // password. If the login attempt is valid, it returns a tuple of the employee
  // and the associated role.
  std::tuple<Employee, ROLE> handleLogInAttempt(std::string enteredUsername,
                                                std::string enteredPassword);
};

#endif

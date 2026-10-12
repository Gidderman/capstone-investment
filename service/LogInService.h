// This class handles the logic behind a log in. Specifically it authenticates
// the users credentials. It salts and hashes the user entered password and
// compares it to the salted, hashed password stored in the database utilizing
// sodium's password hashing library. If the log in is valid then it returns a
// tuple of the employee and associated role. This is to prevent the credentials
// of a user, to include the stored password from being passed around more than
// necessary, while also not relying on the data from the employee struct
// without authentication for role access in the controllers. Additionally, if
// it is the users first time loggin in, then it will have the user create a
// password.

#ifndef LOG_IN_SERVICE_H
#define LOG_IN_SERVICE_H

#include "DataManager.h"
#include "Role.h"

#include <string>
#include <tuple>

class LogInService {
private:
  DataManager &dataManager;
  int logInAttempts;
  bool countAttemptedLogIns;

  Employee employeeAttemptingToLogIn;
  Credentials credentialsForAttemptingToLogInEmployee;

  // This function hashes the password provided, utilizing sodium's password
  // hashing library.
  bool hashPassword(std::string &rawPassword,
                    unsigned char *hashedPasswordArray);

  // This function authenticates the user based on the entered password and the
  // stored password.
  bool authenticate(std::string enteredPassword, std::string validPassword);

public:
  LogInService(DataManager &dataManager);
  ~LogInService();

  // First bool is true if an account does not exist, second is true if
  // the account needs to make a password, the third is true if
  // the account is locked.
  std::tuple<bool, bool, bool>
  doesAccountHaveValidCredentials(std::string username);

  // Returns a tuple of the employee and the associated role on successful log
  // in
  std::optional<std::tuple<Employee, ROLE>>
  handleLogInAttempt(std::string enteredPassword);
  bool verifyPasswordsMatch(std::string enteredPassword,
                            std::string verificationPassword);
  bool verifyPasswordComplexityRequirements(std::string enteredPassword,
                                            std::string verificationPassword);
  void createPassword(std::string enteredPassword);

  // We don't need to count log ins there isn't a valid account to lock,
  // as such this function is used to tell the class whether or not to log
  // attempted log ins.
  void setLogAttemptedLogIns(bool countLogIns);
};

#endif

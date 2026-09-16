#include "DataManager.h"

#include <vector> //TODO: REMOVE AFTER TESTING

DataManager::DataManager() {}

DataManager::~DataManager() {}

Employee DataManager::getEmployeeByUsername(std::string username) {
  // TODO: Format the data from CRUDManager into an Employee struct.
  Employee currentEmployee;

  std::vector<std::string> testingVector = crudManager.runQuery();

  if (testingVector.at(3) == username) {
    currentEmployee.accountID = stoi(testingVector.at(0));
    currentEmployee.firstName = testingVector.at(1);
    currentEmployee.lastName = testingVector.at(2);
    currentEmployee.credentials.username = testingVector.at(3);
    currentEmployee.credentials.password = testingVector.at(4);
    currentEmployee.credentials.salt = testingVector.at(5);
    if (testingVector.at(6) == "ADMIN") {
      currentEmployee.role = ROLE::ADMIN;
    } else {
      currentEmployee.role = ROLE::TRADER;
    }
  }

  return currentEmployee;
}

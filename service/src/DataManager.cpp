#include "DataManager.h"
#include "Credentials.h"

#include <vector> //TODO: REMOVE AFTER TESTING

DataManager::DataManager() {}

DataManager::~DataManager() {}

std::tuple<Employee, Credentials>
DataManager::getEmployeeByUsername(std::string username) {
  // TODO: Format the data from CRUDManager into an Employee struct.
  Employee currentEmployee;
  Credentials employeeCredentials;

  std::vector<std::string> testingVector = crudManager.runQuery();

  if (testingVector.at(3) == username) {
    currentEmployee.accountID = stoi(testingVector.at(0));
    currentEmployee.firstName = testingVector.at(1);
    currentEmployee.lastName = testingVector.at(2);
    employeeCredentials.username = testingVector.at(3);
    employeeCredentials.password = testingVector.at(4);
    employeeCredentials.salt = testingVector.at(5);
    if (testingVector.at(6) == "ADMIN") {
      currentEmployee.role = ROLE::ADMIN;
    } else {
      currentEmployee.role = ROLE::TRADER;
    }
  }

  return {currentEmployee, employeeCredentials};
}

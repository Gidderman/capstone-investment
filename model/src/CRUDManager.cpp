// TODO:
// THIS CLASS IS NOT COMPLETE. CURRENTLY IT IS SIMULATING A DATABASE USING
// UNORDERED_MAPS FOR TESTING THE REMAINDER OF THE APPLICATION, THERE WILL BE
// MANY CHANGES TO THIS CLASS

#include "CRUDManager.h"

#include <iostream>
#include <string>

CRUDManager::CRUDManager()
    : nextEmployeeId(4), nextCustomerId(3), nextInvestmentId(10) {
  // TODO: DELETE FOLLOWING TESTING VECTOR

  // Vector order for employee:
  //  id
  //  First name
  //  Last name
  //  username
  //  password
  //  salt
  //  role
  //  accountLocked?

  std::vector<std::string> populatingVector;
  populatingVector.push_back("1");
  populatingVector.push_back("John");
  populatingVector.push_back("Doe");
  populatingVector.push_back("jdoe");
  populatingVector.push_back("pass123");
  populatingVector.push_back("321");
  populatingVector.push_back("ADMIN");
  populatingVector.push_back("FALSE");

  testingEmployeeDatabase.emplace(00001, populatingVector);

  populatingVector.clear();
  populatingVector.push_back("2");
  populatingVector.push_back("Harry");
  populatingVector.push_back("Styles");
  populatingVector.push_back("hstyle");
  populatingVector.push_back("pass1234");
  populatingVector.push_back("321");
  populatingVector.push_back("TRADER");
  populatingVector.push_back("FALSE");

  testingEmployeeDatabase.emplace(00002, populatingVector);

  populatingVector.clear();
  populatingVector.push_back("3");
  populatingVector.push_back("Tommy");
  populatingVector.push_back("Screwup");
  populatingVector.push_back("tscrewup");
  populatingVector.push_back("pass12345");
  populatingVector.push_back("321");
  populatingVector.push_back("TRADER");
  populatingVector.push_back("TRUE");

  testingEmployeeDatabase.emplace(00003, populatingVector);

  populatingVector.clear();
  // Vector order for customers:
  //  id
  //  first name
  //  last name
  //  phone number
  //  email
  //  date account opened
  //  account type
  //  uninvested funds
  // NUM OF INVESTMENTS (purely for data formatting) (10 per stock)
  //  investment id 0
  // STOCK
  //   stock id 1
  //   stock name 2
  //   stock code 3
  //   stockPrice 4
  //  number held 5
  //  current worth 6
  //  initial investment 7
  //  customerID 8
  //  account id 9
  populatingVector.push_back("1");
  populatingVector.push_back("Bill");
  populatingVector.push_back("Nye");
  populatingVector.push_back("8008008000");
  populatingVector.push_back("email@email.com");
  populatingVector.push_back("20Feb2010");
  populatingVector.push_back("BROKERAGE");
  populatingVector.push_back("200000.00");
  populatingVector.push_back("3");
  populatingVector.push_back("1");
  populatingVector.push_back("1");
  populatingVector.push_back("Nividia");
  populatingVector.push_back("NVD");
  populatingVector.push_back("130.00");
  populatingVector.push_back("2");
  populatingVector.push_back("260.00");
  populatingVector.push_back("75.00");
  populatingVector.push_back("1");
  populatingVector.push_back("2");
  populatingVector.push_back("2");
  populatingVector.push_back("5");
  populatingVector.push_back("Google");
  populatingVector.push_back("GOOG");
  populatingVector.push_back("170.00");
  populatingVector.push_back("3");
  populatingVector.push_back("410.00");
  populatingVector.push_back("15.00");
  populatingVector.push_back("1");
  populatingVector.push_back("2");
  populatingVector.push_back("3");
  populatingVector.push_back("6");
  populatingVector.push_back("Company");
  populatingVector.push_back("CMP");
  populatingVector.push_back("70.00");
  populatingVector.push_back("3");
  populatingVector.push_back("210.00");
  populatingVector.push_back("10.00");
  populatingVector.push_back("1");
  populatingVector.push_back("2");

  testingCustomerDatabase.emplace(00001, populatingVector);

  populatingVector.clear();
  populatingVector.push_back("2");
  populatingVector.push_back("Jim");
  populatingVector.push_back("Halpert");
  populatingVector.push_back("8008008000");
  populatingVector.push_back("email@email.com");
  populatingVector.push_back("20Feb2001");
  populatingVector.push_back("RETIREMENT");
  populatingVector.push_back("200000.00");
  populatingVector.push_back("1");
  populatingVector.push_back("2");
  populatingVector.push_back("7");
  populatingVector.push_back("Dunder Mifflin");
  populatingVector.push_back("DNDR");
  populatingVector.push_back("130.00");
  populatingVector.push_back("2");
  populatingVector.push_back("260.00");
  populatingVector.push_back("75.00");
  populatingVector.push_back("2");
  populatingVector.push_back("2");

  testingCustomerDatabase.emplace(2, populatingVector);
  populatingVector.clear();

  populatingVector.push_back("1");
  populatingVector.push_back("Nividia");
  populatingVector.push_back("NVD");
  populatingVector.push_back("130.00");

  testingStockDatabase.emplace(1, populatingVector);
  populatingVector.clear();

  populatingVector.push_back("2");
  populatingVector.push_back("Apple");
  populatingVector.push_back("APP");
  populatingVector.push_back("180.00");

  testingStockDatabase.emplace(2, populatingVector);
  populatingVector.clear();

  populatingVector.push_back("3");
  populatingVector.push_back("Nintendo");
  populatingVector.push_back("NIN");
  populatingVector.push_back("100.00");

  testingStockDatabase.emplace(3, populatingVector);
  populatingVector.clear();

  populatingVector.push_back("4");
  populatingVector.push_back("Samsung");
  populatingVector.push_back("SSG");
  populatingVector.push_back("200.00");

  testingStockDatabase.emplace(4, populatingVector);
  populatingVector.clear();

  populatingVector.push_back("5");
  populatingVector.push_back("Google");
  populatingVector.push_back("GOOG");
  populatingVector.push_back("170.00");

  testingStockDatabase.emplace(5, populatingVector);
  populatingVector.clear();

  populatingVector.push_back("6");
  populatingVector.push_back("Company");
  populatingVector.push_back("CMP");
  populatingVector.push_back("70.00");

  testingStockDatabase.emplace(6, populatingVector);
  populatingVector.clear();

  populatingVector.push_back("7");
  populatingVector.push_back("Dunder Mifflin");
  populatingVector.push_back("DNDR");
  populatingVector.push_back("130.00");

  testingStockDatabase.emplace(7, populatingVector);
  populatingVector.clear();

  populatingVector.push_back("8");
  populatingVector.push_back("Paramount");
  populatingVector.push_back("PRMT");
  populatingVector.push_back("10.00");

  testingStockDatabase.emplace(8, populatingVector);
  populatingVector.clear();

  populatingVector.push_back("9");
  populatingVector.push_back("Microsoft");
  populatingVector.push_back("MCR");
  populatingVector.push_back("1000.00");

  testingStockDatabase.emplace(9, populatingVector);
  populatingVector.clear();

  populatingVector.push_back("10");
  populatingVector.push_back("Sony");
  populatingVector.push_back("SNY");
  populatingVector.push_back("900.00");

  testingStockDatabase.emplace(10, populatingVector);
  populatingVector.clear();
}

CRUDManager::~CRUDManager() {}

std::vector<std::string> CRUDManager::runLogInQuery(std::string username) {
  for (auto &item : testingEmployeeDatabase) {
    if (item.second.at(3) == username) {
      return item.second;
    }
  }
  std::vector<std::string> blankVector;
  return blankVector;
}

std::vector<std::string> CRUDManager::runQuery(int id, bool isCustomer,
                                               std::string queryType) {

  if (queryType == "read") {
    return readSelection(id, isCustomer);
  } else if (queryType == "delete") {
    return deleteSelection(id, isCustomer);
  } else {
    std::cout << "INVALID QUERY" << std::endl;
    std::vector<std::string> blankVector;
    return blankVector;
  }
}

std::vector<std::string> CRUDManager::runQuery(int id, bool isCustomer,
                                               std::string queryType,
                                               std::vector<std::string> data) {
  std::cout << "CRUDManager::runQuery - entering" << std::endl;

  if (queryType == "create") {
    std::cout << "CRUDManager::runQuery (create) - entering" << std::endl;

    std::cout << "CRUDManager::runQuery (create) - exiting" << std::endl;

    return createSelection(id, isCustomer, data);
  } else if (queryType == "update") {

    std::cout << "CRUDManager::runQuery (update) - entering" << std::endl;
    std::cout << "CRUDManager::runQuery (update) - exiting" << std::endl;

    return updateSelection(id, isCustomer, data);
  } else {
    std::cout << "INVALID QUERY" << std::endl;
    std::vector<std::string> blankVector;
    return blankVector;
  }
}

std::vector<std::string>
CRUDManager::getCredentialsByEmployeeId(int employeeId) {
  return testingEmployeeDatabase.at(employeeId);
}

std::vector<std::string>
CRUDManager::createSelection(int id, bool isCustomer,
                             std::vector<std::string> userToCreate) {
  if (isCustomer) {
    userToCreate.at(0) = std::to_string(nextCustomerId);
    nextCustomerId++;
    testingCustomerDatabase.emplace(std::stoi(userToCreate.at(0)),
                                    userToCreate);
    return testingCustomerDatabase.at(std::stoi(userToCreate.at(0)));
  } else {
    userToCreate.at(0) = std::to_string(nextEmployeeId);
    nextEmployeeId++;
    testingEmployeeDatabase.emplace(std::stoi(userToCreate.at(0)),
                                    userToCreate);
    return testingEmployeeDatabase.at(std::stoi(userToCreate.at(0)));
  }
}

std::vector<std::string> CRUDManager::readSelection(int id, bool isCustomer) {

  if (isCustomer) {
    return testingCustomerDatabase.at(id);
  } else {
    return testingEmployeeDatabase.at(id);
  }
}

std::vector<std::string>
CRUDManager::updateSelection(int id, bool isCustomer,
                             std::vector<std::string> update) {
  std::cout << "CRUDManager::updateSelection - entering" << std::endl;

  if (isCustomer) {

    std::cout << "CRUDManager::updateSelection - exiting" << std::endl;
    testingCustomerDatabase.at(id) = update;
    return testingCustomerDatabase.at(id);
  } else {
    testingEmployeeDatabase.at(id) = update;

    return testingEmployeeDatabase.at(id);
  }
}

std::vector<std::string> CRUDManager::deleteSelection(int id, bool isCustomer) {
  if (isCustomer) {
    testingCustomerDatabase.erase(id);
    if (testingCustomerDatabase.count(id) == 0) {
      std::vector<std::string> blankVector;
      return blankVector;
    } else {
      return testingCustomerDatabase.at(id);
    }
  } else {
    testingEmployeeDatabase.erase(id);
    if (testingEmployeeDatabase.count(id) == 0) {
      std::vector<std::string> blankVector;
      return blankVector;
    } else {
      return testingEmployeeDatabase.at(id);
    }
  }
}

std::unordered_map<int, std::vector<std::string>>
CRUDManager::getAllEmployees() {
  return testingEmployeeDatabase;
}

std::unordered_map<int, std::vector<std::string>>
CRUDManager::getAllCustomers() {
  std::cout << "CRUDManager::getAllCustomers - entering" << std::endl;
  std::cout << "CRUDManager::getAllCustomers - exiting" << std::endl;

  return testingCustomerDatabase;
}

std::unordered_map<int, std::vector<std::string>> CRUDManager::getAllStocks() {
  return testingStockDatabase;
}

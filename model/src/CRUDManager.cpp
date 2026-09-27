// TODO:
// THIS CLASS IS NOT COMPLETE. CURRENTLY IT IS SIMULATING A DATABASE USING
// UNORDERED_MAPS FOR TESTING THE REMAINDER OF THE APPLICATION, THERE WILL BE
// MANY CHANGES TO THIS CLASS

#include "CRUDManager.h"

#include <iostream>
#include <string>
#include <unordered_map>

CRUDManager::CRUDManager()
    : nextEmployeeId(4), nextCustomerId(3), nextInvestmentId(5),
      nextStockId(10) {
  // TODO: DELETE FOLLOWING TESTING VECTORS

  // Role database:
  //    0 - invalid
  //    1 - ADMIN
  //    2 - TRADER
  testingRolesDatabase.emplace(0, "INVALID");
  testingRolesDatabase.emplace(1, "ADMIN");
  testingRolesDatabase.emplace(2, "TRADER");

  // Vector order for employee:
  //  account id
  //  First name
  //  Last name
  //  role id

  std::vector<std::string> populatingVector;
  populatingVector.push_back("1");
  populatingVector.push_back("John");
  populatingVector.push_back("Doe");
  populatingVector.push_back("1");

  testingEmployeeDatabase.emplace(1, populatingVector);

  populatingVector.clear();
  populatingVector.push_back("2");
  populatingVector.push_back("Harry");
  populatingVector.push_back("Styles");
  populatingVector.push_back("2");

  testingEmployeeDatabase.emplace(2, populatingVector);

  populatingVector.clear();
  populatingVector.push_back("3");
  populatingVector.push_back("Tommy");
  populatingVector.push_back("Screwup");
  populatingVector.push_back("2");

  testingEmployeeDatabase.emplace(3, populatingVector);
  populatingVector.clear();

  // Credential database
  //   account id
  //   username
  //   password
  //   salt
  //   accountLocked
  populatingVector.push_back("1");
  populatingVector.push_back("jdoe");
  populatingVector.push_back("pass123");
  populatingVector.push_back("321");
  populatingVector.push_back("0");

  testingCredentialDatabase.emplace(1, populatingVector);
  populatingVector.clear();

  populatingVector.push_back("2");
  populatingVector.push_back("hstyles");
  populatingVector.push_back("pass1234");
  populatingVector.push_back("4321");
  populatingVector.push_back("0");

  testingCredentialDatabase.emplace(2, populatingVector);
  populatingVector.clear();

  populatingVector.push_back("3");
  populatingVector.push_back("tscrewup");
  populatingVector.push_back("pass12345");
  populatingVector.push_back("54321");
  populatingVector.push_back("1");

  testingCredentialDatabase.emplace(3, populatingVector);
  populatingVector.clear();

  // Vector order for customers:
  //  customer id
  //  first name
  //  last name
  //  phone number
  //  email
  //  date account opened
  //  account type
  //  uninvested funds
  //  account id
  populatingVector.push_back("1");
  populatingVector.push_back("Bill");
  populatingVector.push_back("Nye");
  populatingVector.push_back("8008008000");
  populatingVector.push_back("email@email.com");
  populatingVector.push_back("20Feb2010");
  populatingVector.push_back("BROKERAGE");
  populatingVector.push_back("200000.00");
  populatingVector.push_back("2");

  testingCustomerDatabase.emplace(1, populatingVector);
  populatingVector.clear();

  populatingVector.push_back("2");
  populatingVector.push_back("Jim");
  populatingVector.push_back("Halpert");
  populatingVector.push_back("8008008000");
  populatingVector.push_back("email@email.com");
  populatingVector.push_back("20Feb2001");
  populatingVector.push_back("RETIREMENT");
  populatingVector.push_back("200000.00");
  populatingVector.push_back("2");

  testingCustomerDatabase.emplace(2, populatingVector);
  populatingVector.clear();

  // NUM OF INVESTMENTS
  //  investment id 0
  //   stock code 1
  //  number held 2
  //  current worth 3
  //  initial investment 4
  //  customerID 5

  populatingVector.push_back("1");
  populatingVector.push_back("NVD");
  populatingVector.push_back("2");
  populatingVector.push_back("260.00");
  populatingVector.push_back("75.00");
  populatingVector.push_back("1");

  testingInvestmentDatabase.emplace(1, populatingVector);
  populatingVector.clear();

  populatingVector.push_back("2");
  populatingVector.push_back("APP");
  populatingVector.push_back("5");
  populatingVector.push_back("410.00");
  populatingVector.push_back("15.00");
  populatingVector.push_back("1");

  testingInvestmentDatabase.emplace(2, populatingVector);
  populatingVector.clear();

  populatingVector.push_back("3");
  populatingVector.push_back("NIN");
  populatingVector.push_back("6");
  populatingVector.push_back("210.00");
  populatingVector.push_back("10.00");
  populatingVector.push_back("1");

  testingInvestmentDatabase.emplace(3, populatingVector);
  populatingVector.clear();

  populatingVector.push_back("4");
  populatingVector.push_back("DNDR");
  populatingVector.push_back("2");
  populatingVector.push_back("260.00");
  populatingVector.push_back("75.00");
  populatingVector.push_back("2");

  testingInvestmentDatabase.emplace(4, populatingVector);
  populatingVector.clear();

  // Stock database:
  //   stock id
  //   stock name
  //   stock code
  //   stock price

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

std::optional<std::unordered_map<std::string, std::vector<std::string>>>
CRUDManager::runLogInQuery(std::string username) {
  for (auto &item : testingCredentialDatabase) {
    if (item.second.at(1) == username) {
      std::unordered_map<std::string, std::vector<std::string>>
          employeeAndCredentials;

      employeeAndCredentials.emplace("employee",
                                     testingEmployeeDatabase.at(item.first));
      employeeAndCredentials.emplace("credentials", item.second);

      return employeeAndCredentials;
    }
  }
  return {};
}

std::optional<std::unordered_map<std::string, std::vector<std::string>>>
CRUDManager::runQuery(int id, bool isCustomer, std::string queryType) {

  if (queryType == "read") {
    return readSelection(id, isCustomer);
  } else if (queryType == "delete") {
    if (deleteSelection(id, isCustomer)) {
      std::unordered_map<std::string, std::vector<std::string>>
          emptyReturnVector;
      return emptyReturnVector;
    }
    return readSelection(id, isCustomer);
  } else {
    std::cout << "INVALID QUERY" << std::endl;
    return {};
  }
}

std::optional<std::unordered_map<std::string, std::vector<std::string>>>
CRUDManager::runQuery(
    int id, bool isCustomer, std::string queryType,
    std::unordered_map<std::string, std::vector<std::string>> data) {
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
    return {};
  }
}

std::vector<std::string>
CRUDManager::getCredentialsByEmployeeId(int employeeId) {
  return testingCredentialDatabase.at(employeeId);
}

std::optional<std::unordered_map<std::string, std::vector<std::string>>>
CRUDManager::createSelection(
    int id, bool isCustomer,
    std::unordered_map<std::string, std::vector<std::string>> userToCreate) {
  if (isCustomer) {

    std::cout << "CRUDManager::createSelection (customer) - entering"
              << std::endl;
    std::cout
        << "CRUDManager::createSelection (customer) - size of user to create: "
        << userToCreate.size() << std::endl;

    std::cout << "CRUDManager::createSelection (customer) - size of user to "
                 "create customer: "
              << userToCreate.at("customer").size() << std::endl;

    std::cout << "CRUDManager::createSelection (customer) - size of user to "
                 "create investments: "
              << userToCreate.at("investments").size() << std::endl;

    // CREATE CUSTOMER
    userToCreate.at("customer").at(0) = std::to_string(nextCustomerId);
    testingCustomerDatabase.emplace(nextCustomerId,
                                    userToCreate.at("customer"));

    if (userToCreate.at("investments").size() > 0) {

      userToCreate.at("investments").at(0) = std::to_string(nextInvestmentId);
      testingInvestmentDatabase.emplace(nextInvestmentId,
                                        userToCreate.at("investments"));
      nextInvestmentId++;
    }

    nextCustomerId++;

    if (userToCreate.at("customer").at(0) == "-1") {
      return {};
    }

    std::cout << "CRUDManager::createSelection (customer) - exiting"
              << std::endl;

    return userToCreate;

  } else {
    // CREATE EMPLOYEE
    userToCreate.at("employee").at(0) = std::to_string(nextEmployeeId);
    userToCreate.at("credentials").at(0) = std::to_string(nextEmployeeId);
    testingEmployeeDatabase.emplace(nextEmployeeId,
                                    userToCreate.at("employee"));
    testingCredentialDatabase.emplace(nextEmployeeId,
                                      userToCreate.at("credentials"));
    nextEmployeeId++;

    if (userToCreate.at("employee").at(0) == "-1" ||
        userToCreate.at("credentials").at(0) == "-1") {
      return {};
    }

    return userToCreate;
  }
}

std::optional<std::unordered_map<std::string, std::vector<std::string>>>
CRUDManager::readSelection(int id, bool isCustomer) {
  if (isCustomer) {
    // READ CUSTOMER
    std::unordered_map<std::string, std::vector<std::string>>
        customerInformation;
    customerInformation.emplace("customer", testingCustomerDatabase.at(id));

    std::vector<std::string> investments;
    for (auto &item : testingInvestmentDatabase) {
      if (item.second.at(5) == std::to_string(id)) {
        for (std::string investmentData : item.second) {
          std::cout << "CRUDManager::readSelection - pushing " << investmentData
                    << " to update vector." << std::endl;
          investments.push_back(investmentData);
        }
      }
    }
    customerInformation.emplace("investments", investments);

    if (customerInformation.empty()) {
      return {};
    }
    return customerInformation;

  } else {
    std::unordered_map<std::string, std::vector<std::string>>
        employeeInformation;
    employeeInformation.emplace("employee", testingEmployeeDatabase.at(id));
    employeeInformation.emplace("credentials",
                                testingCredentialDatabase.at(id));

    if (employeeInformation.empty()) {
      return {};
    }
    return employeeInformation;
  }
}

std::optional<std::unordered_map<std::string, std::vector<std::string>>>
CRUDManager::updateSelection(
    int id, bool isCustomer,
    std::unordered_map<std::string, std::vector<std::string>> update) {

  std::cout << "CRUDManager::updateSelection - entering" << std::endl;

  if (isCustomer) {
    testingCustomerDatabase.at(id) = update.at("customer");

    std::vector<int> listOfIdsAttachedToCustomer;
    for (auto items : testingInvestmentDatabase) {
      if (std::stoi(items.second.at(5)) == id) {
        listOfIdsAttachedToCustomer.push_back(items.first);
      }
    }

    std::vector<int> listsOfIdsAttachedToCustomerAfterEdit;
    if (update.at("investments").size() != 0) {
      std::vector<std::string> vectorToAdd;
      for (unsigned int i = 0; i < update.at("investments").size(); i++) {
        vectorToAdd.push_back(update.at("investments").at(i));

        if (i % 6 == 5) {
          if (testingInvestmentDatabase.count(std::stoi(vectorToAdd.at(0))) ==
              0) {
            vectorToAdd.at(0) = std::to_string(nextInvestmentId);
            nextInvestmentId++;
            testingInvestmentDatabase.emplace(std::stoi(vectorToAdd.at(0)),
                                              vectorToAdd);
            listsOfIdsAttachedToCustomerAfterEdit.push_back(
                std::stoi(vectorToAdd.at(0)));
          } else {
            testingInvestmentDatabase.at(std::stoi(vectorToAdd.at(0))) =
                vectorToAdd;
            listsOfIdsAttachedToCustomerAfterEdit.push_back(
                std::stoi(vectorToAdd.at(0)));
          }
          vectorToAdd.clear();
        }
      }
    }

    for (int &ids : listOfIdsAttachedToCustomer) {
      for (int newIds : listsOfIdsAttachedToCustomerAfterEdit) {
        if (ids == newIds) {
          ids = -1;
        }
      }
    }

    for (int ids : listOfIdsAttachedToCustomer) {
      if (!(ids == -1)) {
        testingInvestmentDatabase.erase(ids);
      }
    }

    if (testingCustomerDatabase.at(id) != update.at("customer")) {
      return {};
    }
    return update;
  } else {
    testingEmployeeDatabase.at(id) = update.at("employee");
    if (update.count("credentials") != 0) {
      testingCredentialDatabase.at(id) = update.at("credentials");
    }

    if (testingEmployeeDatabase.at(id) != update.at("employee")) {
      return {};
    }

    return update;
  }
}

bool CRUDManager::deleteSelection(int id, bool isCustomer) {
  if (isCustomer) {
    testingCustomerDatabase.erase(id);
    for (auto &investment : testingInvestmentDatabase) {
      if (investment.second.at(5) == std::to_string(id)) {
        testingInvestmentDatabase.erase(std::stoi(investment.second.at(0)));
      }
    }

    if (testingCustomerDatabase.count(id) == 0) {
      return true;
    }
    return false;

  } else {
    testingEmployeeDatabase.erase(id);
    testingCredentialDatabase.erase(id);

    for (auto &customer : testingCustomerDatabase) {
      if (customer.second.at(8) == std::to_string(id)) {
        customer.second.at(8) = "0";
      }
    }

    if (testingEmployeeDatabase.count(id) == 0) {
      return true;
    }
    return false;
  }
}

std::unordered_map<int, std::vector<std::string>>
CRUDManager::getAllEmployees() {
  return testingEmployeeDatabase;
}

std::tuple<std::unordered_map<int, std::vector<std::string>>,
           std::unordered_map<int, std::vector<std::string>>>
CRUDManager::getAllCustomers() {
  std::cout << "CRUDManager::getAllCustomers - entering" << std::endl;
  std::cout << "CRUDManager::getAllCustomers - exiting" << std::endl;

  return {testingCustomerDatabase, testingInvestmentDatabase};
}

std::unordered_map<int, std::vector<std::string>> CRUDManager::getAllStocks() {
  return testingStockDatabase;
}

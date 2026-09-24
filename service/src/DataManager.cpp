// This class implements DataManager.h, see that header file for details
// regarding the purpose of this class.

// TODO:
// CURRENTLY THIS CLASS FORMATS DATA IN A WAY THAT MY SIMULATED DATABASE CAN
// INTERFACE WITH IT, I.E. MOSTLY INTO VECTORS OF STRINGS. THERE WILL BE
// REFACTORING REQUIRED WHEN AN ACTUAL DATABASE CONNECTION IS MADE

#include "DataManager.h"
#include "Credentials.h"
#include "Customer.h"
#include "Investment.h"

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector> //TODO: REMOVE AFTER TESTING

DataManager::DataManager() { refreshHashTableStockData(); }

DataManager::~DataManager() {}

std::tuple<Employee, Credentials>
DataManager::getEmployeeByUsername(std::string username) {
  std::vector<std::string> returnedVector = crudManager.runLogInQuery(username);

  return formatEmployeeQueriedData(returnedVector);
}

Credentials DataManager::getCredentialsByEmployeeId(int employeeId) {
  std::vector<std::string> returnedVector =
      crudManager.getCredentialsByEmployeeId(employeeId);

  return std::get<1>(formatEmployeeQueriedData(returnedVector));
}

Customer DataManager::getCustomer(int customerId) {
  return formatCustomerQueriedData(
      crudManager.runQuery(customerId, true, "read"));
}

Employee DataManager::getEmployee(int accountId) {
  return std::get<0>(formatEmployeeQueriedData(
      crudManager.runQuery(accountId, false, "read")));
}

std::vector<Employee> DataManager::getAllEmployees() {
  std::unordered_map<int, std::vector<std::string>> allEmployeeData =
      crudManager.getAllEmployees();

  std::vector<Employee> formatedEmployeeData;
  for (auto &employee : allEmployeeData) {
    formatedEmployeeData.push_back(
        std::get<0>(formatEmployeeQueriedData(employee.second)));
  }

  return formatedEmployeeData;
}

std::vector<Customer> DataManager::getAllCustomers() {
  std::unordered_map<int, std::vector<std::string>> allCustomerData =
      crudManager.getAllCustomers();

  std::vector<Customer> formattedCustomerData;
  for (auto &customer : allCustomerData) {
    formattedCustomerData.push_back(formatCustomerQueriedData(customer.second));
  }

  return formattedCustomerData;
}

std::vector<Customer> DataManager::getAllCustomersByTraders(int traderID) {
  std::vector<Customer> customers = getAllCustomers();
  std::cout << customers.size() << " total customers" << std::endl;
  for (unsigned int i = 0; i < customers.size(); i++) {
    if (customers.at(i).accountID != traderID) {
      std::cout << "Erasing customer " << customers.at(i).lastName << " id num "
                << customers.at(i).accountID << std::endl;
      customers.erase(customers.begin() + i);
    }
  }

  std::cout << "Final list size " << customers.size() << std::endl;

  return customers;
}

bool DataManager::createEmployee(Employee employeeToCreate,
                                 Credentials credentials) {
  std::vector<std::string> queryFormattedData =
      formatEmployeeForQuery(employeeToCreate, credentials);

  std::vector<std::string> returnedVector = crudManager.runQuery(
      std::stoi(queryFormattedData.at(0)), false, "create", queryFormattedData);

  return std::stoi(returnedVector.at(0)) != -1;
}

bool DataManager::createCustomer(Customer customerToCreate) {
  std::vector<std::string> queryFormattedData =
      formatCustomerForQuery(customerToCreate);

  std::vector<std::string> returnedVector = crudManager.runQuery(
      std::stoi(queryFormattedData.at(0)), true, "create", queryFormattedData);

  return std::stoi(returnedVector.at(0)) != -1;
}

bool DataManager::updateEmployee(Employee employeeToUpdate, Employee update,
                                 Credentials updateCredentials) {
  std::vector<std::string> queryFormattedData =
      formatEmployeeForQuery(update, updateCredentials);

  std::vector<std::string> returnedVector = crudManager.runQuery(
      employeeToUpdate.accountID, false, "update", queryFormattedData);

  std::cout << "DataManager::updateEmployee - Data sent to CRUD Manager: "
            << std::endl;
  for (std::string item : queryFormattedData) {
    std::cout << "   " << item << std::endl;
  }

  std::cout << "DataManager::updateEmployee - Data returned from CRUD Manager: "
            << std::endl;
  for (std::string item : returnedVector) {
    std::cout << "   " << item << std::endl;
  }

  return queryFormattedData == returnedVector;
}

bool DataManager::updateCustomer(Customer customerToUpdate, Customer update) {
  std::cout << "DataManager::updateCustomer - beginning update: " << std::endl;
  std::cout << "Edited data: " << std::endl;

  std::vector<std::string> queryFormattedData = formatCustomerForQuery(update);

  for (std::string item : queryFormattedData) {
    std::cout << item << std::endl;
  }

  std::vector<std::string> returnedVector = crudManager.runQuery(
      customerToUpdate.customerID, true, "update", queryFormattedData);

  std::cout << "Returned vector: " << std::endl;
  for (std::string item : returnedVector) {
    std::cout << item << std::endl;
  }

  return queryFormattedData == returnedVector;
}

bool DataManager::deleteEmployee(Employee employeeToDelete) {
  std::vector<std::string> returnedVector =
      crudManager.runQuery(employeeToDelete.accountID, false, "delete");

  return returnedVector.size() == 0;
}

bool DataManager::deleteCustomer(Customer customerToDelete) {
  std::vector<std::string> returnedVector =
      crudManager.runQuery(customerToDelete.customerID, true, "delete");

  return returnedVector.size() == 0;
}

bool DataManager::updateStoredStocks(std::vector<Stock>) {
  // TODO: store updated stock data into the database, and store into the hash
  // map for quick access.
  refreshHashTableStockData();
  return true;
}

std::vector<Stock> DataManager::getAllStoredStocks() {
  return stockHashTable.getAll();
}

//*******************PRIVATE FUNCTIONS****************************
std::tuple<Employee, Credentials>
DataManager::formatEmployeeQueriedData(std::vector<std::string> queriedData) {
  Employee employee;
  Credentials credentials;

  employee.accountID = std::stoi(queriedData.at(0));
  employee.firstName = queriedData.at(1);
  employee.lastName = queriedData.at(2);
  if (queriedData.at(6) == "ADMIN") {
    employee.role = ADMIN;
  } else {
    employee.role = TRADER;
  }

  credentials.accountID = std::stoi(queriedData.at(0));
  credentials.username = queriedData.at(3);
  credentials.password = queriedData.at(4);
  credentials.salt = queriedData.at(5);
  if (queriedData.at(7) == "TRUE") {
    credentials.accountLocked = true;
  } else {
    credentials.accountLocked = false;
  }

  std::tuple<Employee, Credentials> employeeInformation = {employee,
                                                           credentials};
  return employeeInformation;
}

Customer
DataManager::formatCustomerQueriedData(std::vector<std::string> queriedData) {
  Customer customer;

  customer.customerID = std::stoi(queriedData.at(0));
  customer.firstName = queriedData.at(1);
  customer.lastName = queriedData.at(2);
  customer.phoneNum = queriedData.at(3);
  customer.email = queriedData.at(4);
  customer.dateAccountOpened = queriedData.at(5);
  if (queriedData.at(6) == "BROKERAGE") {
    customer.accountType = BROKERAGE;
  } else {
    customer.accountType = RETIREMENT;
  }
  customer.uninvestedFunds = std::stof(queriedData.at(7));

  int numOfInvestments = std::stoi(queriedData.at(8));

  for (unsigned int i = 0; i < numOfInvestments; i++) {
    Investment investment;
    Stock stock;
    for (unsigned int j = 0; j < 10; j++) {
      switch (j) {
      case 0:
        investment.investmentID = std::stoi(queriedData.at(9 + (i * 10) + j));
        break;
      case 1:
        stock.stockID = std::stoi(queriedData.at(9 + (i * 10) + j));
        break;
      case 2:
        stock.stockName = queriedData.at(9 + (i * 10) + j);
        break;
      case 3:
        stock.stockCode = queriedData.at(9 + (i * 10) + j);
        break;
      case 4:
        stock.stockPrice = std::stof(queriedData.at(9 + (i * 10) + j));
        break;
      case 5:
        investment.numHeld = std::stoi(queriedData.at(9 + (i * 10) + j));
        break;
      case 6:
        investment.currentInvestmentWorth =
            stock.stockPrice * investment.numHeld;
        break;
      case 7:
        investment.initialInvestment =
            std::stof(queriedData.at(9 + (i * 10) + j));
        break;
      case 8:
        investment.customerID = std::stoi(queriedData.at(9 + (i * 10) + j));
        break;
      case 9:
        customer.accountID =
            std::stoi(queriedData.at(9 + (i * 10) + j)); // This is unnecessary
      default:
        break;
      }
    }
    investment.stock = stock;
    customer.investments.push_back(investment);
  }
  return customer;
}

std::vector<std::string>
DataManager::formatEmployeeForQuery(Employee employee, Credentials credential) {
  std::vector<std::string> formatedData;
  formatedData.push_back(std::to_string(employee.accountID));
  formatedData.push_back(employee.firstName);
  formatedData.push_back(employee.lastName);
  formatedData.push_back(credential.username);
  formatedData.push_back(credential.password);
  formatedData.push_back(credential.salt);
  if (employee.role == ADMIN) {
    formatedData.push_back("ADMIN");
  } else {
    formatedData.push_back("TRADER");
  }
  if (credential.accountLocked) {
    formatedData.push_back("TRUE");
  } else {
    formatedData.push_back("FALSE");
  }

  return formatedData;
}

std::vector<std::string>
DataManager::formatCustomerForQuery(Customer customer) {
  std::vector<std::string> formatedData;
  formatedData.push_back(std::to_string(customer.customerID));
  formatedData.push_back(customer.firstName);
  formatedData.push_back(customer.lastName);
  formatedData.push_back(customer.phoneNum);
  formatedData.push_back(customer.email);
  formatedData.push_back(customer.dateAccountOpened);
  if (customer.accountType == RETIREMENT) {
    formatedData.push_back("RETIREMENT");
  } else {
    formatedData.push_back("BROKERAGE");
  }
  formatedData.push_back(std::to_string(customer.uninvestedFunds));
  formatedData.push_back(std::to_string(customer.investments.size()));

  for (unsigned int i = 0; i < customer.investments.size(); i++) {
    for (unsigned int j = 0; j < 10; j++) {
      switch (j) {
      case 0:
        formatedData.push_back(
            std::to_string(customer.investments.at(i).investmentID));
        break;
      case 1:
        formatedData.push_back(
            std::to_string(customer.investments.at(i).stock.stockID));
        break;
      case 2:
        formatedData.push_back(customer.investments.at(i).stock.stockName);
        break;
      case 3:
        formatedData.push_back(customer.investments.at(i).stock.stockCode);
        break;
      case 4:
        formatedData.push_back(
            std::to_string(customer.investments.at(i).stock.stockPrice));
        break;
      case 5:
        formatedData.push_back(
            std::to_string(customer.investments.at(i).numHeld));
        break;
      case 6:
        formatedData.push_back(
            std::to_string(customer.investments.at(i).currentInvestmentWorth));
        break;
      case 7:
        formatedData.push_back(
            std::to_string(customer.investments.at(i).initialInvestment));
        break;
      case 8:
        formatedData.push_back(
            std::to_string(customer.investments.at(i).customerID));
        break;
      case 9:
        formatedData.push_back(std::to_string(customer.accountID));
      default:
        break;
      }
    }
  }

  return formatedData;
}

void DataManager::refreshHashTableStockData() {
  stockHashTable.clear();

  std::unordered_map<int, std::vector<std::string>> stockData =
      crudManager.getAllStocks();

  for (const auto &item : stockData) {
    Stock newStock = {std::stoi(item.second.at(0)), item.second.at(1),
                      item.second.at(2), std::stof(item.second.at(3))};

    stockHashTable.insert(newStock.stockCode, newStock);
  }
}

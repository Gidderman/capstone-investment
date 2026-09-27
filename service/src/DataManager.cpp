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
  std::cout << "DataManager::getEmployeeByUsername - entering" << std::endl;
  Employee fetchedEmployee;
  Credentials fetchedCredentials;

  std::optional<std::unordered_map<std::string, std::vector<std::string>>>
      returnedValue = crudManager.runLogInQuery(username);

  if (!returnedValue.has_value()) {
    return {fetchedEmployee, fetchedCredentials};
  }

  std::vector<std::string> employeeStrings =
      returnedValue.value().at("employee");
  std::vector<std::string> credentialStrings =
      returnedValue.value().at("credentials");

  std::cout << "DataManager::getEmployeeByUsername - exiting" << std::endl;
  return {formatEmployeeQueriedData(employeeStrings),
          formatCredentialsQueriedData(credentialStrings)};
}

Credentials DataManager::getCredentialsByEmployeeId(int employeeId) {
  std::vector<std::string> returnedVector =
      crudManager.getCredentialsByEmployeeId(employeeId);

  return formatCredentialsQueriedData(returnedVector);
}

Customer DataManager::getCustomer(int customerId) {
  std::optional<std::unordered_map<std::string, std::vector<std::string>>>
      returnedData = crudManager.runQuery(customerId, true, "read");
  Customer customer;
  customer.customerID = -1;

  if (!returnedData.has_value()) {
    return customer;
  }

  customer = formatInvestmentsForCustomer(
      formatCustomerQueriedData(returnedData.value().at("customer")),
      returnedData.value().at("investments"));

  return customer;
}

Employee DataManager::getEmployee(int accountId) {
  std::optional<std::unordered_map<std::string, std::vector<std::string>>>
      returnedData = crudManager.runQuery(accountId, false, "read");

  Employee employee;
  employee.accountID = -1;

  if (!returnedData.has_value()) {
    return employee;
  }

  return formatEmployeeQueriedData(returnedData.value().at("employee"));
}

Employee DataManager::getEmployeeByLastName(std::string name) {
  for (Employee employee : getAllEmployees()) {
    if (employee.lastName == name) {
      return employee;
    }
  }
  return {-1, "", "", INVALID, -1};
}

std::vector<Employee> DataManager::getAllEmployees() {
  std::unordered_map<int, std::vector<std::string>> allEmployeeData =
      crudManager.getAllEmployees();

  std::vector<Employee> formatedEmployeeData;
  for (auto &employee : allEmployeeData) {
    formatedEmployeeData.push_back(formatEmployeeQueriedData(employee.second));
  }

  return formatedEmployeeData;
}

std::vector<Customer> DataManager::getAllCustomers() {
  std::cout << "DataManager::getAllCustomers - entering" << std::endl;

  std::tuple<std::unordered_map<int, std::vector<std::string>>,
             std::unordered_map<int, std::vector<std::string>>>
      allCustomerAndInvestmentData = crudManager.getAllCustomers();

  std::cout << "Data from database: " << std::endl;
  std::cout << "Customer size: "
            << std::get<0>(allCustomerAndInvestmentData).size() << std::endl;
  std::cout << "Investment size: "
            << std::get<1>(allCustomerAndInvestmentData).size() << std::endl;

  std::vector<Customer> formattedCustomerData;
  for (auto &customer : std::get<0>(allCustomerAndInvestmentData)) {
    Customer formattedCustomer = formatCustomerQueriedData(customer.second);

    std::vector<std::string> customerInvestments;
    for (auto &investment : std::get<1>(allCustomerAndInvestmentData)) {
      if (std::stoi(investment.second.at(5)) == formattedCustomer.customerID) {
        customerInvestments.insert(customerInvestments.end(),
                                   investment.second.begin(),
                                   investment.second.end());
      }
    }
    formattedCustomer =
        formatInvestmentsForCustomer(formattedCustomer, customerInvestments);
    formattedCustomerData.push_back(formattedCustomer);
  }

  std::cout << "DataManager::getAllCustomers - exiting" << std::endl;
  std::cout << "DataMangager::getAllCustomers - customer size "
            << formattedCustomerData.size() << std::endl;

  return formattedCustomerData;
}

std::vector<Customer> DataManager::getAllCustomersByTraders(int traderID) {
  std::vector<Customer> customers = getAllCustomers();
  for (unsigned int i = 0; i < customers.size(); i++) {
    if (customers.at(i).accountID != traderID) {
      customers.erase(customers.begin() + i);
    }
  }

  return customers;
}

bool DataManager::createEmployee(Employee employeeToCreate,
                                 Credentials credentials) {
  std::unordered_map<std::string, std::vector<std::string>> queryFormattedData =
      formatEmployeeForQuery(employeeToCreate, credentials);

  std::optional<std::unordered_map<std::string, std::vector<std::string>>>
      returnedData = crudManager.runQuery(
          std::stoi(queryFormattedData.at("employee").at(0)), false, "create",
          queryFormattedData);

  return returnedData.has_value();
}

bool DataManager::createCustomer(Customer customerToCreate) {
  std::unordered_map<std::string, std::vector<std::string>> queryFormattedData =
      formatCustomerForQuery(customerToCreate);

  std::optional<std::unordered_map<std::string, std::vector<std::string>>>
      returnedData = crudManager.runQuery(
          std::stoi(queryFormattedData.at("customer").at(0)), true, "create",
          queryFormattedData);

  return returnedData.has_value();
}

bool DataManager::updateEmployee(Employee employeeToUpdate, Employee update,
                                 Credentials updateCredentials) {
  std::unordered_map<std::string, std::vector<std::string>> queryFormattedData =
      formatEmployeeForQuery(update, updateCredentials);

  std::optional<std::unordered_map<std::string, std::vector<std::string>>>
      returnedData = crudManager.runQuery(employeeToUpdate.accountID, false,
                                          "update", queryFormattedData);

  return returnedData.has_value();
}

bool DataManager::updateCustomer(Customer customerToUpdate, Customer update) {
  std::cout << "DataManager::updateCustomer - entering" << std::endl;

  std::unordered_map<std::string, std::vector<std::string>> queryFormattedData =
      formatCustomerForQuery(update);

  std::cout << "DataManager::updateCustomer - queryFormattedData investment "
               "size / original size"
            << std::endl;
  std::cout << queryFormattedData.at("investments").size() / 6 << " / "
            << customerToUpdate.investments.size() << std::endl;

  std::optional<std::unordered_map<std::string, std::vector<std::string>>>
      returnedData = crudManager.runQuery(customerToUpdate.customerID, true,
                                          "update", queryFormattedData);

  std::cout << "DataManager::updateCustomer - exiting" << std::endl;

  return returnedData.has_value();
}

bool DataManager::deleteEmployee(Employee employeeToDelete) {
  std::optional<std::unordered_map<std::string, std::vector<std::string>>>
      returnedData =
          crudManager.runQuery(employeeToDelete.accountID, false, "delete");

  return returnedData.has_value();
}

bool DataManager::deleteCustomer(Customer customerToDelete) {
  std::optional<std::unordered_map<std::string, std::vector<std::string>>>
      returnedVector =
          crudManager.runQuery(customerToDelete.customerID, true, "delete");

  return returnedVector.has_value();
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
Employee
DataManager::formatEmployeeQueriedData(std::vector<std::string> queriedData) {
  std::cout << "DataManager::formatEmployeeQueriedData - entering" << std::endl;

  Employee employee;

  employee.accountID = std::stoi(queriedData.at(0));
  employee.firstName = queriedData.at(1);
  employee.lastName = queriedData.at(2);
  if (queriedData.at(3) == "1") {
    employee.role = ADMIN;
  } else if (queriedData.at(3) == "2") {
    employee.role = TRADER;
  } else {
    employee.role = INVALID;
  }
  employee.numAccountsManaged = 0;

  for (Customer customer : getAllCustomers()) {
    std::cout << "DataManager::formatEmployeeQueriedData - customer accountID "
                 "/ employee accountID"
              << std::endl;
    std::cout << customer.accountID << " / " << employee.accountID << std::endl;
    if (customer.accountID == employee.accountID) {
      employee.numAccountsManaged++;
    }
  }

  std::cout << "DataManager::formatEmployeeQueriedData - exiting" << std::endl;

  return employee;
}

Credentials DataManager::formatCredentialsQueriedData(
    std::vector<std::string> queriedData) {
  std::cout << "DataManager::formatCredentialsQueriedData - entering"
            << std::endl;

  Credentials credentials;

  credentials.accountID = std::stoi(queriedData.at(0));
  credentials.username = queriedData.at(1);
  credentials.password = queriedData.at(2);
  credentials.salt = queriedData.at(3);
  if (queriedData.at(4) == "0") {
    credentials.accountLocked = false;
  } else {
    credentials.accountLocked = true;
  }

  std::cout << "DataManager::formatCredentialsQueriedData - exiting"
            << std::endl;

  return credentials;
}

Customer
DataManager::formatCustomerQueriedData(std::vector<std::string> queriedData) {
  std::cout << "DataManager::formatCustomerQueriedData - entering" << std::endl;

  Customer customer;

  customer.customerID = std::stoi(queriedData.at(0));
  customer.firstName = queriedData.at(1);
  customer.lastName = queriedData.at(2);
  customer.phoneNum = queriedData.at(3);
  customer.email = queriedData.at(4);
  customer.dateAccountOpened = queriedData.at(5);
  if (queriedData.at(6) == "RETIREMENT") {

    customer.accountType = RETIREMENT;
  } else {
    customer.accountType = BROKERAGE;
  }
  customer.uninvestedFunds = std::stof(queriedData.at(7));
  customer.accountID = std::stoi(queriedData.at(8));

  std::cout << "DataManager::formatCustomerQueriedData - exiting" << std::endl;

  return customer;
}

Customer DataManager::formatInvestmentsForCustomer(
    Customer customerToFormat, std::vector<std::string> investmentData) {
  Investment investment;
  investment.investmentID = -1;
  std::optional<Stock> stock;
  for (unsigned int i = 0; i < investmentData.size(); i++) {
    std::cout << "On iteration " << i << " of " << investmentData.size()
              << std::endl;

    switch (i % 6) {
    case 0:
      investment.investmentID = std::stoi(investmentData.at(i));
      break;
    case 1:
      stock = stockHashTable.get(investmentData.at(i));
      if (!stock.has_value()) {
        Stock blankStock = {-1, "NOT FOUND", "***", -1.00f};
        investment.stock = blankStock;
      }
      investment.stock = stock.value();
      break;
    case 2:
      investment.numHeld = std::stoi(investmentData.at(i));
      break;
    case 3:
      investment.currentInvestmentWorth = std::stof(investmentData.at(i));
      break;
    case 4:
      investment.initialInvestment = std::stof(investmentData.at(i));
      break;
    case 5:
      investment.customerID = std::stoi(investmentData.at(i));

      if (investment.investmentID != -1) {
        std::cout << "Should have a complete investment now." << std::endl;

        if (investment.customerID == customerToFormat.customerID) {
          std::cout << "At " << i
                    << " iteration, investment is: " << "CustomerID "
                    << investment.customerID << std::endl
                    << investment.stock.stockName << std::endl;

          customerToFormat.investments.push_back(investment);
        }
        investment.investmentID = -1;
      }
    }
  }

  std::cout << "Total Investments for " << customerToFormat.firstName << " are "
            << customerToFormat.investments.size() << std::endl;

  return customerToFormat;
}

std::unordered_map<std::string, std::vector<std::string>>
DataManager::formatEmployeeForQuery(Employee employee, Credentials credential) {
  std::unordered_map<std::string, std::vector<std::string>> formatedData;
  std::vector<std::string> employeeData;
  std::vector<std::string> credentialsData;

  employeeData.push_back(std::to_string(employee.accountID));
  employeeData.push_back(employee.firstName);
  employeeData.push_back(employee.lastName);
  if (employee.role == ADMIN) {
    employeeData.push_back("1");
  } else {
    employeeData.push_back("2");
  }
  formatedData.emplace("employee", employeeData);

  credentialsData.push_back(std::to_string(credential.accountID));
  credentialsData.push_back(credential.username);
  credentialsData.push_back(credential.password);
  credentialsData.push_back(credential.salt);
  if (!credential.accountLocked) {
    credentialsData.push_back("0");
  } else {
    credentialsData.push_back("1");
  }
  formatedData.emplace("credentials", credentialsData);

  return formatedData;
}

std::unordered_map<std::string, std::vector<std::string>>
DataManager::formatCustomerForQuery(Customer customer) {
  std::cout << "DataManager::FormatCustomerForQuery - entering" << std::endl;
  std::unordered_map<std::string, std::vector<std::string>> formatedData;

  std::vector<std::string> populatingVector;
  // First format the customer
  populatingVector.push_back(std::to_string(customer.customerID));
  populatingVector.push_back(customer.firstName);
  populatingVector.push_back(customer.lastName);
  populatingVector.push_back(customer.phoneNum);
  populatingVector.push_back(customer.email);
  populatingVector.push_back(customer.dateAccountOpened);
  if (customer.accountType == RETIREMENT) {
    populatingVector.push_back("RETIREMENT");
  } else {
    populatingVector.push_back("BROKERAGE");
  }
  populatingVector.push_back(std::to_string(customer.uninvestedFunds));
  populatingVector.push_back(std::to_string(customer.accountID));

  formatedData.emplace("customer", populatingVector);
  populatingVector.clear();

  // Format the investments
  for (Investment investment : customer.investments) {
    populatingVector.push_back(std::to_string(investment.investmentID));
    populatingVector.push_back(investment.stock.stockCode);
    populatingVector.push_back(std::to_string(investment.numHeld));
    populatingVector.push_back(
        std::to_string(investment.currentInvestmentWorth));
    populatingVector.push_back(std::to_string(investment.initialInvestment));
    populatingVector.push_back(std::to_string(investment.customerID));
  }

  formatedData.emplace("investments", populatingVector);
  populatingVector.clear();

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

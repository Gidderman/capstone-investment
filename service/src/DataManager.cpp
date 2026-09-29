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
#include <qsqlquery.h>
#include <qvariant.h>
#include <string>
#include <unordered_map>
#include <vector> //TODO: REMOVE AFTER TESTING

DataManager::DataManager() { refreshHashTableStockData(); }

DataManager::~DataManager() {}

std::tuple<Employee, Credentials>
DataManager::getEmployeeByUsername(std::string username) {
  Employee employeeToReturn;
  Credentials employeeCredentials;

  QString query = "SELECT "
                  "e.account_id, "
                  "e.first_name, "
                  "e.last_name, "
                  "COUNT(cu.customer_id) AS num_customers, "
                  "e.role_id, "
                  "c.username, "
                  "c.password, "
                  "c.salt, "
                  "c.account_locked "
                  "FROM employees e "
                  "INNER JOIN credentials c ON c.account_id = e.account_id "
                  "LEFT JOIN customers cu ON cu.account_id = e.account_id "
                  "WHERE c.username = :username "
                  "GROUP BY e.account_id";

  std::unordered_map<QString, QVariant> queryArgs;
  queryArgs.emplace(":username", QString::fromStdString(username));

  std::optional<QSqlQuery> employeeAndCredentials =
      crudManager.runQuery(query, queryArgs);

  if (!employeeAndCredentials.has_value()) {
    errorInfo = crudManager.getErrorCode();
    return {employeeToReturn, employeeCredentials};
  }

  if (employeeAndCredentials.value().next()) {
    employeeToReturn.accountID =
        employeeAndCredentials.value().value("account_id").toInt();
    employeeToReturn.firstName = employeeAndCredentials.value()
                                     .value("first_name")
                                     .toString()
                                     .toStdString();
    employeeToReturn.lastName = employeeAndCredentials.value()
                                    .value("last_name")
                                    .toString()
                                    .toStdString();
    employeeToReturn.numAccountsManaged =
        employeeAndCredentials.value().value("num_customers").toInt();
    employeeToReturn.role =
        (ROLE)employeeAndCredentials.value().value("role_id").toInt();

    employeeCredentials.accountID =
        employeeAndCredentials.value().value("account_id").toInt();
    employeeCredentials.username = employeeAndCredentials.value()
                                       .value("username")
                                       .toString()
                                       .toStdString();
    employeeCredentials.password = employeeAndCredentials.value()
                                       .value("password")
                                       .toString()
                                       .toStdString();
    employeeCredentials.salt =
        employeeAndCredentials.value().value("salt").toString().toStdString();
    employeeCredentials.accountLocked =
        employeeAndCredentials.value().value("accountLocked").toBool();
  }

  return {employeeToReturn, employeeCredentials};
}

Credentials DataManager::getCredentialsByEmployeeId(int employeeId) {
  Credentials credentials;

  QString query = "SELECT * FROM credentials "
                  "WHERE account_id = :accound_id";
  std::unordered_map<QString, QVariant> queryArgs;
  queryArgs.emplace(":accound_id", employeeId);

  std::optional<QSqlQuery> credentialData =
      crudManager.runQuery(query, queryArgs);

  if (!credentialData.has_value()) {
    errorInfo = crudManager.getErrorCode();
    return credentials;
  }

  if (credentialData.value().next()) {
    credentials.accountID = credentialData.value().value("account_id").toInt();
    credentials.username =
        credentialData.value().value("username").toString().toStdString();
    credentials.password =
        credentialData.value().value("password").toString().toStdString();
    credentials.salt =
        credentialData.value().value("salt").toString().toStdString();
    credentials.accountLocked =
        credentialData.value().value("account_locked").toBool();
  }

  return credentials;
}

Customer DataManager::getCustomer(int customerId) {
  Customer customer;

  QString query = "SELECT *"
                  "FROM customers "
                  "WHERE customer_id = :customer_id";
  std::unordered_map<QString, QVariant> queryArgs;
  queryArgs.emplace(":customer_id", customerId);

  std::optional<QSqlQuery> customerData =
      crudManager.runQuery(query, queryArgs);

  if (!customerData.has_value()) {
    errorInfo = crudManager.getErrorCode();
    return customer;
  }

  if (customerData.value().next()) {
    customer.customerID = customerData.value().value("customer_id").toInt();
    customer.firstName =
        customerData.value().value("first_name").toString().toStdString();
    customer.lastName =
        customerData.value().value("last_name").toString().toStdString();
    customer.phoneNum =
        customerData.value().value("phone").toString().toStdString();
    customer.email =
        customerData.value().value("email").toString().toStdString();
    customer.dateAccountOpened = customerData.value()
                                     .value("date_account_opened")
                                     .toString()
                                     .toStdString();
    if (customerData.value().value("account_type").toString().toStdString() ==
        "Retirement") {
      customer.accountType = RETIREMENT;
    } else {
      customer.accountType = BROKERAGE;
    }
    customer.uninvestedFunds =
        customerData.value().value("uninvested_funds").toFloat();
    customer.accountID = customerData.value().value("account_id").toInt();
  }

  query = "SELECT "
          "investment_id, "
          "stock_code, "
          "number_held, "
          "initial_investment"
          "FROM investments "
          "WHERE customer_id = :customer_id";
  // We don't need to rewrite queryArgs, as customer_id remains the same

  customerData = crudManager.runQuery(query, queryArgs);

  if (!customerData.has_value()) {
    errorInfo = crudManager.getErrorCode();
    Customer blankCustomer;
    return blankCustomer;
  }

  while (customerData.value().next()) {
    Investment investment;
    investment.investmentID =
        customerData.value().value("investment_id").toInt();
    investment.stock = stockHashTable
                           .get(customerData.value()
                                    .value("stock_code")
                                    .toString()
                                    .toStdString())
                           .value();
    investment.numHeld = customerData.value().value("number_held").toInt();
    investment.currentInvestmentWorth =
        investment.numHeld * investment.stock.stockPrice;
    investment.initialInvestment =
        customerData.value().value("initial_investment").toFloat();
    investment.customerID = customerId;

    customer.investments.push_back(investment);
  }

  return customer;
}

Employee DataManager::getEmployee(int accountId) {
  // TODO: THIS
}

Employee DataManager::getEmployeeByLastName(std::string name) {
  // TODO: THIS
}

std::vector<Employee> DataManager::getAllEmployees() {
  std::vector<Employee> allEmployees;

  QString query = "SELECT "
                  "e.account_id, "
                  "e.first_name, "
                  "e.last_name, "
                  "COUNT(c.customer_id) AS num_customers, "
                  "role_id "
                  "FROM employees e "
                  "LEFT JOIN customers c "
                  "ON c.account_id = e.account_id "
                  "GROUP BY e.account_id";
  std::unordered_map<QString, QVariant> queryArgs;

  std::optional<QSqlQuery> employeeData =
      crudManager.runQuery(query, queryArgs);

  if (!employeeData.has_value()) {
    errorInfo = crudManager.getErrorCode();
    return allEmployees; // Empty
  }

  while (employeeData.value().next()) {
    Employee employeeToAdd;
    employeeToAdd.accountID = employeeData.value().value("account_id").toInt();
    employeeToAdd.firstName =
        employeeData.value().value("first_name").toString().toStdString();
    employeeToAdd.lastName =
        employeeData.value().value("last_name").toString().toStdString();
    employeeToAdd.numAccountsManaged =
        employeeData.value().value("num_customers").toInt();
    employeeToAdd.role = (ROLE)employeeData.value().value("role_id").toInt();

    allEmployees.push_back(employeeToAdd);
  }

  return allEmployees;
}

std::vector<Customer> DataManager::getAllCustomers() {
  // TODO: THIS
}

std::vector<Customer> DataManager::getAllCustomersByTraders(int traderID) {
  // TODO: THIS
}

bool DataManager::createEmployee(Employee employeeToCreate,
                                 // TODO: THIS
                                 Credentials credentials) {}

bool DataManager::createCustomer(Customer customerToCreate) {
  // TODO: THIS
}

bool DataManager::updateEmployee(Employee employeeToUpdate, Employee update,
                                 // TODO: THIS
                                 Credentials updateCredentials) {}

bool DataManager::updateCustomer(Customer customerToUpdate, Customer update) {
  // TODO: THIS
}

bool DataManager::deleteEmployee(Employee employeeToDelete) {
  // TODO: THIS
}

bool DataManager::deleteCustomer(Customer customerToDelete) {
  // TODO: THIS
}

bool DataManager::updateStoredStocks(std::vector<Stock>) {
  // TODO: THIS
}

std::vector<Stock> DataManager::getAllStoredStocks() {
  // TODO: THIS
}

std::string DataManager::getErrorInfo() { return errorInfo; }

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
  // TODO: THIS
}

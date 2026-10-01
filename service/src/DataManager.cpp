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
#include <optional>
#include <qsqlquery.h>
#include <qvariant.h>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector> //TODO: REMOVE AFTER TESTING

// We pass in by reference the CRUD Manager to prevent multiple copies and
//  multiple database connections
DataManager::DataManager(CRUDManager &crudManager) : crudManager(crudManager) {
  if (!updateStoredStocks(getAllStoredStocks())) {
    errorInfo = crudManager.getErrorCode();
    // TODO: notify of error
  }
  refreshHashTableStockData();
}

DataManager::~DataManager() {}

std::tuple<Employee, Credentials>
DataManager::getEmployeeByUsername(std::string username) {
  std::cout << "DataManager::getEmployeeByUsername - entering" << std::endl;

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
        employeeAndCredentials.value().value("account_locked").toBool();
  }

  std::cout << "DataManager::getEmployeeByUsername - exiting" << std::endl;

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
          "initial_investment "
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
  Employee fetchedEmployee;

  QString query = "SELECT "
                  "e.*, "
                  "COUNT(c.customer_id) AS num_customer "
                  "FROM employees e "
                  "LEFT JOIN customers c "
                  "ON c.account_id = e.account_id "
                  "WHERE e.account_id = :account_id";

  std::unordered_map<QString, QVariant> queryArgs;
  queryArgs.emplace(":account_id", accountId);

  std::optional<QSqlQuery> employeeData =
      crudManager.runQuery(query, queryArgs);

  if (!employeeData.has_value()) {
    errorInfo = crudManager.getErrorCode();
    return fetchedEmployee;
  }

  if (employeeData.value().next()) {
    fetchedEmployee.accountID =
        employeeData.value().value("account_id").toInt();
    fetchedEmployee.firstName =
        employeeData.value().value("first_name").toString().toStdString();
    fetchedEmployee.lastName =
        employeeData.value().value("last_name").toString().toStdString();
    fetchedEmployee.numAccountsManaged =
        employeeData.value().value("num_customer").toInt();
    fetchedEmployee.numAccountsManaged =
        (ROLE)employeeData.value().value("role_id").toInt();
  }

  return fetchedEmployee;
}

Employee DataManager::getEmployeeByLastName(std::string name) {
  Employee fetchedEmployee;

  QString query = "SELECT "
                  "e.*, "
                  "COUNT(c.customer_id) AS num_customer "
                  "FROM employees e "
                  "LEFT JOIN customers c "
                  "ON c.account_id = e.account_id "
                  "WHERE e.last_name = :last_name "
                  "GROUP BY e.account_id";

  std::unordered_map<QString, QVariant> queryArgs;
  queryArgs.emplace(":last_name", QString::fromStdString(name));

  std::optional<QSqlQuery> employeeData =
      crudManager.runQuery(query, queryArgs);

  if (!employeeData.has_value()) {
    errorInfo = crudManager.getErrorCode();
    return fetchedEmployee;
  }

  if (employeeData.value().next()) {
    fetchedEmployee.accountID =
        employeeData.value().value("account_id").toInt();
    fetchedEmployee.firstName =
        employeeData.value().value("first_name").toString().toStdString();
    fetchedEmployee.lastName =
        employeeData.value().value("last_name").toString().toStdString();
    fetchedEmployee.numAccountsManaged =
        employeeData.value().value("num_customer").toInt();
    fetchedEmployee.numAccountsManaged =
        (ROLE)employeeData.value().value("role_id").toInt();
  }

  return fetchedEmployee;
}

Stock DataManager::getStock(std::string stockCode) {
  std::optional<Stock> fetchedStock = stockHashTable.get(stockCode);

  if (!fetchedStock.has_value()) {
    throw std::logic_error("DataManager::getStock -> stock not found.");
  }

  return fetchedStock.value();
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
  std::vector<Customer> customers;

  QString query = "SELECT * FROM customers";
  std::unordered_map<QString, QVariant> queryArgs;

  std::optional<QSqlQuery> fetchedData = crudManager.runQuery(query, queryArgs);

  if (!fetchedData.has_value()) {
    errorInfo = crudManager.getErrorCode();
    return customers;
  }

  while (fetchedData.value().next()) {
    Customer customerToAdd;
    customerToAdd.customerID = fetchedData.value().value("customer_id").toInt();
    customerToAdd.firstName =
        fetchedData.value().value("first_name").toString().toStdString();
    customerToAdd.lastName =
        fetchedData.value().value("last_name").toString().toStdString();
    customerToAdd.phoneNum =
        fetchedData.value().value("phone").toString().toStdString();
    customerToAdd.dateAccountOpened = fetchedData.value()
                                          .value("date_account_opened")
                                          .toString()
                                          .toStdString();
    if (fetchedData.value().value("account_type").toString().toStdString() ==
        "Retirement") {
      customerToAdd.accountType = RETIREMENT;
    } else {
      customerToAdd.accountType = BROKERAGE;
    }
    customerToAdd.uninvestedFunds =
        fetchedData.value().value("uninvested_funds").toFloat();
    customerToAdd.accountID = fetchedData.value().value("account_id").toInt();

    customers.push_back(customerToAdd);
  }

  query = "SELECT * FROM investments";

  fetchedData = crudManager.runQuery(query, queryArgs);

  if (!fetchedData.has_value()) {
    errorInfo = crudManager.getErrorCode();
    std::vector<Customer> blankCustomers;
    return blankCustomers;
  }

  while (fetchedData.value().next()) {
    Investment investment;
    investment.investmentID =
        fetchedData.value().value("investment_id").toInt();
    investment.stock = stockHashTable
                           .get(fetchedData.value()
                                    .value("stock_code")
                                    .toString()
                                    .toStdString())
                           .value();
    investment.numHeld = fetchedData.value().value("number_held").toInt();
    investment.currentInvestmentWorth =
        investment.numHeld * investment.stock.stockPrice;
    investment.initialInvestment =
        fetchedData.value().value("initial_investment").toFloat();
    investment.customerID = fetchedData.value().value("customer_id").toInt();

    for (Customer &customer : customers) {
      if (customer.customerID == investment.customerID) {
        customer.investments.push_back(investment);
      }
    }
  }

  return customers;
}

std::vector<Customer> DataManager::getAllCustomersByTraders(int traderID) {
  std::vector<Customer> customers;

  QString query = "SELECT * FROM customers "
                  "WHERE account_id = :account_id";
  std::unordered_map<QString, QVariant> queryArgs;
  queryArgs.emplace(":account_id", traderID);

  std::optional<QSqlQuery> fetchedData = crudManager.runQuery(query, queryArgs);

  if (!fetchedData.has_value()) {
    errorInfo = crudManager.getErrorCode();
    return customers;
  }

  while (fetchedData.value().next()) {
    Customer customerToAdd;
    customerToAdd.customerID = fetchedData.value().value("customer_id").toInt();
    customerToAdd.firstName =
        fetchedData.value().value("first_name").toString().toStdString();
    customerToAdd.lastName =
        fetchedData.value().value("last_name").toString().toStdString();
    customerToAdd.phoneNum =
        fetchedData.value().value("phone").toString().toStdString();
    customerToAdd.dateAccountOpened = fetchedData.value()
                                          .value("date_account_opened")
                                          .toString()
                                          .toStdString();
    if (fetchedData.value().value("account_type").toString().toStdString() ==
        "Retirement") {
      customerToAdd.accountType = RETIREMENT;
    } else {
      customerToAdd.accountType = BROKERAGE;
    }
    customerToAdd.uninvestedFunds =
        fetchedData.value().value("uninvested_funds").toFloat();
    customerToAdd.accountID = fetchedData.value().value("account_id").toInt();

    customers.push_back(customerToAdd);
  }

  query = "SELECT * FROM investments";
  queryArgs.clear();

  fetchedData = crudManager.runQuery(query, queryArgs);

  if (!fetchedData.has_value()) {
    errorInfo = crudManager.getErrorCode();
    std::vector<Customer> blankCustomers;
    return blankCustomers;
  }

  while (fetchedData.value().next()) {
    Investment investment;

    for (Customer &customer : customers) {
      investment.customerID = fetchedData.value().value("customer_id").toInt();

      if (customer.customerID == investment.customerID) {
        investment.investmentID =
            fetchedData.value().value("investment_id").toInt();
        investment.stock = stockHashTable
                               .get(fetchedData.value()
                                        .value("stock_code")
                                        .toString()
                                        .toStdString())
                               .value();
        investment.numHeld = fetchedData.value().value("number_held").toInt();
        investment.currentInvestmentWorth =
            investment.numHeld * investment.stock.stockPrice;
        investment.initialInvestment =
            fetchedData.value().value("initial_investment").toFloat();

        customer.investments.push_back(investment);
      }
    }
  }

  return customers;
}

bool DataManager::createEmployee(Employee employeeToCreate,
                                 Credentials credentials) {
  QString query = "INSERT INTO employees "
                  "(first_name, last_name, role_id) "
                  "VALUES "
                  "(:first_name, :last_name, :role_id)";
  std::unordered_map<QString, QVariant> queryArgs;
  queryArgs.emplace(":first_name",
                    QString::fromStdString(employeeToCreate.firstName));
  queryArgs.emplace(":last_name",
                    QString::fromStdString(employeeToCreate.lastName));
  queryArgs.emplace(":role_id", employeeToCreate.role);

  std::optional<QSqlQuery> queryStatus = crudManager.runQuery(query, queryArgs);

  if (!queryStatus.has_value()) {
    errorInfo = crudManager.getErrorCode();
    return false;
  }

  credentials.accountID = queryStatus.value().lastInsertId().toInt();

  query = "INSERT INTO credentials "
          "VALUES "
          "(:account_id, "
          ":username, "
          ":password, "
          ":salt, "
          ":account_locked)";
  queryArgs.clear();
  queryArgs.emplace(":account_id", credentials.accountID);
  queryArgs.emplace(":username", QString::fromStdString(credentials.username));
  queryArgs.emplace(":password", QString::fromStdString(credentials.password));
  queryArgs.emplace(":salt", QString::fromStdString(credentials.salt));
  queryArgs.emplace(":account_locked", credentials.accountLocked);

  queryStatus = crudManager.runQuery(query, queryArgs);

  if (!queryStatus.has_value()) {
    errorInfo = crudManager.getErrorCode();
    return false;
  }

  return true;
}

bool DataManager::createCustomer(Customer customerToCreate) {
  QString query = "INSERT INTO customers "
                  "(first_name, "
                  "last_name, "
                  "phone, "
                  "email, "
                  "date_account_opened, "
                  "account_type, "
                  "uninvested_funds, "
                  "account_id) "
                  "VALUES "
                  "(:first_name, "
                  ":last_name, "
                  ":phone, "
                  ":email, "
                  "CURDATE(), "
                  ":account_type, "
                  ":uninvested_funds, "
                  ":account_id) ";
  std::unordered_map<QString, QVariant> queryArgs;
  queryArgs.emplace(":first_name",
                    QString::fromStdString(customerToCreate.firstName));
  queryArgs.emplace(":last_name",
                    QString::fromStdString(customerToCreate.lastName));
  queryArgs.emplace(":phone",
                    QString::fromStdString(customerToCreate.phoneNum));
  queryArgs.emplace(":email", QString::fromStdString(customerToCreate.email));
  if (customerToCreate.accountType == RETIREMENT) {
    queryArgs.emplace(":account_type", "Retirement");
  } else {
    queryArgs.emplace(":account_type", "Brokerage");
  }
  queryArgs.emplace(":uninvested_funds", customerToCreate.uninvestedFunds);
  queryArgs.emplace(":account_id", customerToCreate.accountID);

  std::optional<QSqlQuery> queryStatus = crudManager.runQuery(query, queryArgs);

  if (!queryStatus.has_value()) {
    errorInfo = crudManager.getErrorCode();
    return false;
  }

  return true;
}

bool DataManager::createInvestment(Investment investment) {
  QString query = "INSERT INTO investments "
                  "(stock_code, number_held, current_worth, "
                  "initial_investment, customer_id) "
                  "VALUES "
                  "(:stock_code, :number_held, :current_worth, "
                  ":initial_investment, :customer_id)";
  std::unordered_map<QString, QVariant> queryArgs;
  queryArgs.emplace(":stock_code",
                    QString::fromStdString(investment.stock.stockCode));
  queryArgs.emplace(":number_held", investment.numHeld);
  queryArgs.emplace(":current_worth", investment.currentInvestmentWorth);
  queryArgs.emplace(":initial_investment", investment.initialInvestment);
  queryArgs.emplace(":customer_id", investment.customerID);

  std::optional<QSqlQuery> queryStatus = crudManager.runQuery(query, queryArgs);

  if (!queryStatus.has_value()) {
    errorInfo = crudManager.getErrorCode();
    return false;
  }

  return true;
}

bool DataManager::updateEmployee(int employeeId, Employee update) {
  QString query = "UPDATE employees "
                  "SET first_name = :first_name, "
                  "last_name = :last_name, "
                  "role_id = :role_id "
                  "WHERE account_id = :account_id";
  std::unordered_map<QString, QVariant> queryArgs;
  queryArgs.emplace(":first_name", QString::fromStdString(update.firstName));
  queryArgs.emplace(":last_name", QString::fromStdString(update.lastName));
  queryArgs.emplace(":role_id", (int)update.role);
  queryArgs.emplace(":account_id", employeeId);

  std::optional<QSqlQuery> queryStatus = crudManager.runQuery(query, queryArgs);

  if (!queryStatus.has_value()) {
    errorInfo = crudManager.getErrorCode();
    return false;
  }

  return true;
}

bool DataManager::updateEmployeeCredentials(int id, Credentials update) {
  QString query = "UPDATE credentials "
                  "SET username = :username, "
                  "password = :password, "
                  "salt = :salt, "
                  "account_locked = :account_locked "
                  "WHERE account_id = :account_id";
  std::unordered_map<QString, QVariant> queryArgs;
  queryArgs.emplace(":username", QString::fromStdString(update.username));
  queryArgs.emplace(":password", QString::fromStdString(update.password));
  queryArgs.emplace(":salt", QString::fromStdString(update.salt));
  queryArgs.emplace(":account_locked", update.accountLocked);
  queryArgs.emplace(":account_id", id);

  std::optional<QSqlQuery> queryStatus = crudManager.runQuery(query, queryArgs);

  if (!queryStatus.has_value()) {
    errorInfo = crudManager.getErrorCode();
    return false;
  }

  return true;
}

bool DataManager::updateCustomer(int customerId, Customer update) {
  QString query = "UPDATE customers "
                  "SET first_name = :first_name, "
                  "last_name = :last_name, "
                  "phone = :phone, "
                  "email = :email, "
                  "date_account_opened = :date_account_opened, "
                  "account_type = :account_type, "
                  "uninvested_funds = :uninvested_funds, "
                  "account_id = :account_id "
                  "WHERE customer_id = :customer_id";
  std::unordered_map<QString, QVariant> queryArgs;
  queryArgs.emplace(":first_name", QString::fromStdString(update.firstName));
  queryArgs.emplace(":last_name", QString::fromStdString(update.lastName));
  queryArgs.emplace(":phone", QString::fromStdString(update.phoneNum));
  queryArgs.emplace(":email", QString::fromStdString(update.email));
  queryArgs.emplace(":date_account_opened",
                    QString::fromStdString(update.dateAccountOpened));
  if (update.accountType == RETIREMENT) {
    queryArgs.emplace(":date_account_opened", "Retirement");
  } else {
    queryArgs.emplace(":date_account_opened", "Brokerage");
  }
  queryArgs.emplace(":uninvested_funds", update.uninvestedFunds);
  queryArgs.emplace(":account_id", update.accountID);
  queryArgs.emplace(":customer_id", customerId);

  std::optional<QSqlQuery> queryStatus = crudManager.runQuery(query, queryArgs);

  if (!queryStatus.has_value()) {
    errorInfo = crudManager.getErrorCode();
    return false;
  }

  return true;
}

bool DataManager::updateInvestment(int investmentId, Investment update) {
  QString query = "UPDATE investments "
                  "SET stock_code = :stock_code, "
                  "number_held = :number_held, "
                  "current_worth = :current_worth, "
                  "initial_investment = :initial_investment, "
                  "customer_id = :customer_id "
                  "WHERE investment_id = :investment_id";
  std::unordered_map<QString, QVariant> queryArgs;
  queryArgs.emplace(":stock_code",
                    QString::fromStdString(update.stock.stockCode));
  queryArgs.emplace(":number_held", update.numHeld);
  queryArgs.emplace(":current_worth", update.currentInvestmentWorth);
  queryArgs.emplace(":initial_investment", update.initialInvestment);
  queryArgs.emplace(":customer_id", update.customerID);
  queryArgs.emplace(":investment_id", update.investmentID);

  std::optional<QSqlQuery> queryStatus = crudManager.runQuery(query, queryArgs);

  if (!queryStatus.has_value()) {
    errorInfo = crudManager.getErrorCode();
    return false;
  }

  return true;
}

bool DataManager::deleteEmployee(int employeeId) {
  QString query = "DELETE FROM employees "
                  "WHERE account_id = :account_id";

  std::unordered_map<QString, QVariant> queryArgs;
  queryArgs.emplace(":account_id", employeeId);

  std::optional<QSqlQuery> queryStatus = crudManager.runQuery(query, queryArgs);

  if (!queryStatus.has_value()) {
    errorInfo = crudManager.getErrorCode();
    return false;
  }

  return true;
}

bool DataManager::deleteCustomer(int customerId) {
  QString query = "DELETE FROM customers "
                  "WHERE customer_id = :customer_id";

  std::unordered_map<QString, QVariant> queryArgs;
  queryArgs.emplace(":customer_id", customerId);

  std::optional<QSqlQuery> queryStatus = crudManager.runQuery(query, queryArgs);

  if (!queryStatus.has_value()) {
    errorInfo = crudManager.getErrorCode();
    return false;
  }

  return true;
}

bool DataManager::deleteInvestment(int investmentId) {
  QString query = "DELETE FROM investments "
                  "WHERE investment_id = :investment_id";

  std::unordered_map<QString, QVariant> queryArgs;
  queryArgs.emplace(":investment_id", investmentId);

  std::optional<QSqlQuery> queryStatus = crudManager.runQuery(query, queryArgs);

  if (!queryStatus.has_value()) {
    errorInfo = crudManager.getErrorCode();
    return false;
  }

  return true;
}

bool DataManager::updateStoredStocks(std::vector<Stock> stocks) {
  QString query = "UPDATE stocks "
                  "SET stock_price = :stock_price, "
                  "SET last_updated = NOW() "
                  "WHERE stock_code = :stock_code";
  std::unordered_map<QString, QVariant> queryArgs;
  std::optional<QSqlQuery> queryStatus;

  // This is used in case an error throws to let the user
  // know how many stocks were successfully updated
  int numStocksSuccessfullyUpdated = 0;

  for (Stock stock : stocks) {
    queryArgs.emplace(":stock_price", stock.stockPrice);
    queryArgs.emplace(":stock_code", QString::fromStdString(stock.stockCode));

    queryStatus = crudManager.runQuery(query, queryArgs);

    if (!queryStatus.has_value()) {
      errorInfo = crudManager.getErrorCode() + "\n Successfully updated " +
                  std::to_string(numStocksSuccessfullyUpdated) + " stocks.";

      return false;
    }

    queryArgs.clear();
    numStocksSuccessfullyUpdated++;
  }

  refreshHashTableStockData();

  return true;
}

std::vector<Stock> DataManager::getAllStoredStocks() {
  return stockHashTable.getAll();
}

std::string DataManager::getErrorInfo() { return errorInfo; }

//*******************PRIVATE FUNCTIONS****************************

void DataManager::refreshHashTableStockData() {
  QString query = "SELECT * FROM stocks";
  std::unordered_map<QString, QVariant> queryArgs;

  std::optional<QSqlQuery> allStocks = crudManager.runQuery(query, queryArgs);

  if (!allStocks.has_value()) {
    errorInfo = crudManager.getErrorCode();
    // TODO: figure out how to notify of issue
  }

  while (allStocks.value().next()) {
    Stock stock;
    stock.stockCode =
        allStocks.value().value("stock_code").toString().toStdString();
    stock.stockName =
        allStocks.value().value("stock_name").toString().toStdString();
    stock.stockPrice = allStocks.value().value("stock_price").toFloat();

    stockHashTable.insert(stock.stockCode, stock);
  }
}

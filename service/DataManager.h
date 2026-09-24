// The DataManager class is in charge of formating information in a way that the
// CRUDManager can utilize it for Queries, and formating the return data in a
// way that the various Service classes can use it. It is the bridge between our
// service classes and the model layer in the architecture.

// TODO:
// CURRENTLY THIS CLASS FORMATS DATA IN A WAY THAT MY SIMULATED DATABASE CAN
// INTERFACE WITH IT, I.E. MOSTLY INTO VECTORS OF STRINGS. THERE WILL BE
// REFACTORING REQUIRED WHEN AN ACTUAL DATABASE CONNECTION IS MADE

#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include "CRUDManager.h"
#include "Credentials.h"
#include "Customer.h"
#include "Employee.h"
#include "HashTable.h"

#include <string>
#include <tuple>

class DataManager {
private:
  CRUDManager crudManager;

  // Data structure to store stocks for quick access. Due to the limits
  // of the free API that is used to pull stock information, we will be
  // dealing with stale stock data (at most approximately 24 hours old)
  // and as such I have chosen to limit the number of database calls in
  // this instance, instead opting for speed with "stale" data.
  Custom::HashTable stockHashTable;

  // These private functions format data either into an acceptable format
  // for the CRUDManager to make queries with it, or formats the returning
  // data into a format usable to the calling service classes
  std::tuple<Employee, Credentials>
  formatEmployeeQueriedData(std::vector<std::string> queriedData);
  Customer formatCustomerQueriedData(std::vector<std::string> queriedData);
  std::vector<std::string> formatEmployeeForQuery(Employee employee,
                                                  Credentials credential);
  std::vector<std::string> formatCustomerForQuery(Customer customer);

  // This pulls all stocks and populates them within the hash table
  void refreshHashTableStockData();

public:
  DataManager();
  ~DataManager();

  // Used primarily during log in to get employee information and credentials by
  // username
  std::tuple<Employee, Credentials> getEmployeeByUsername(std::string username);

  // Used to get credentials of an employee based on the employee id. This is
  // necessary for whenever an account needs to be unlocked or if the user is
  // setting their password after creation.
  Credentials getCredentialsByEmployeeId(int employeeId);

  // Individual get functions
  Customer getCustomer(int customerId);
  Employee getEmployee(int accountId);

  std::vector<Employee> getAllEmployees();
  std::vector<Customer> getAllCustomers();
  std::vector<Customer> getAllCustomersByTraders(int traderId);

  bool createEmployee(Employee employeeToCreate,
                      Credentials employeeCredentials);
  bool createCustomer(Customer customerToCreate);

  bool updateEmployee(Employee employeeToUpdate, Employee update,
                      Credentials credential);
  bool updateCustomer(Customer customerToUpdate, Customer update);

  bool deleteEmployee(Employee employeeToDelete);
  bool deleteCustomer(Customer customerToDelete);

  // This function is called upon initialization to get the most up to date
  // stock data into the database.
  bool updateStoredStocks(std::vector<Stock>);

  // When purchasing a stock, this function is used to get the list of
  // available stocks.
  std::vector<Stock> getAllStoredStocks();
};

#endif

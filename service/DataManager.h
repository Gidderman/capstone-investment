#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include "CRUDManager.h"
#include "Credentials.h"
#include "Customer.h"
#include "Employee.h"

#include <string>
#include <tuple>

class DataManager {
private:
  CRUDManager crudManager;
  std::tuple<Employee, Credentials>
  formatEmployeeQueriedData(std::vector<std::string> queriedData);
  Customer formatCustomerQueriedData(std::vector<std::string> queriedData);
  std::vector<std::string> formatEmployeeForQuery(Employee employee,
                                                  Credentials credential);
  std::vector<std::string> formatCustomerForQuery(Customer customer);

public:
  DataManager();
  ~DataManager();
  std::tuple<Employee, Credentials> getEmployeeByUsername(std::string username);
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
  bool updateStoredStocks(std::vector<Stock>);
  std::vector<Stock> getAllStoredStocks();
};

#endif

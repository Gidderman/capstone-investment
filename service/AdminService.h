// The AdminService class handles all business logic for the Admin Portion of
// the program. It performs actions as directed by the Admin Controller, and
// communicates with the DataManager class to get information from the Database.

#ifndef ADMIN_SERVICE_H
#define ADMIN_SERVICE_H

#include "DataManager.h"

#include <vector>

class AdminService {
private:
  DataManager &dataManager;

  std::string errorInfo;

public:
  AdminService(DataManager &dataManager);
  ~AdminService();
  std::vector<Employee> getAllEmployees();
  Employee getEmployeeById(int id);
  Employee getEmployeeByName(std::string name);
  std::vector<Customer> getAllCustomers();
  Customer getCustomerById(int id);
  void createEmployee(Employee employeeToCreate);
  void editEmployee(Employee employeeToEdit, Employee edit);
  void createCustomer(Customer customer);
  void editCustomer(Customer customer, Customer edit);
};

#endif

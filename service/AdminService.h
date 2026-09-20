// The AdminService class handles all business logic for the Admin Portion of
// the program. It performs actions as directed by the Admin Controller, and
// communicates with the DataManager class to get information from the Database.

#ifndef ADMIN_SERVICE_H
#define ADMIN_SERVICE_H

#include "DataManager.h"

#include <vector>

class AdminService {
private:
  DataManager dataManager;

public:
  AdminService();
  ~AdminService();
  std::vector<Employee> getAllEmployees();
  std::vector<Customer> getAllCustomers();
  bool createEmployee(Employee employeeToCreate);
  bool editEmployee(Employee employeeToEdit, Employee edit);
  bool createCustomer(Customer customer);
  bool editCustomer(Customer customer, Customer edit);
};

#endif

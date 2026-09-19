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
};

#endif

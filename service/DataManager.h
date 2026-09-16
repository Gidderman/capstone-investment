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

public:
  DataManager();
  ~DataManager();
  std::tuple<Employee, Credentials> getEmployeeByUsername(std::string username);
};

#endif

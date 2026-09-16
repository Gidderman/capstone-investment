#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include "CRUDManager.h"
#include "Customer.h"
#include "Employee.h"

#include <string>

class DataManager {
private:
  CRUDManager crudManager;

public:
  DataManager();
  ~DataManager();
  Employee getEmployeeByUsername(std::string username);
};

#endif

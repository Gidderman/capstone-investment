#ifndef TRADER_SERVICE_H
#define TRADER_SERVICE_H

#include "DataManager.h"

#include <vector>

class TraderService {
private:
  DataManager dataManager;

public:
  TraderService();
  ~TraderService();
  std::vector<Customer> getListOfManagedCustomers(int employeeId);
};

#endif

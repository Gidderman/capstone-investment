// This class handles all the business logic related to the trader functionality
// of the application.

// TODO:
// THIS CLASS IS LARGELY UNIMPLEMENTED AND REQUIRES MORE WORK.

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
  // Used to get the list of customers to display on the MainTraderView
  std::vector<Customer> getListOfManagedCustomers(int employeeId);
  std::vector<Stock> getListOfAvailableStocks();
  float calculatePriceOfStockPurchase(std::string stockCode, int num);
};

#endif

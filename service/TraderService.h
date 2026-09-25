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
  std::tuple<Employee, int>
  getEmployeeByAndNumManagedCustomersById(int employeeId);
  // Used to get the list of customers to display on the MainTraderView
  std::vector<Customer> getListOfManagedCustomers(int employeeId);
  Customer getCustomerById(int customerId);
  // Returns a list of available stocks for purchasing
  std::vector<Stock> getListOfAvailableStocks();
  // Calculates the price for purchasing stocks based on the number of stocks
  // and the selected stock from the purchase stock window
  bool executeStockPurchase(int customerId,
                            std::tuple<std::string, int, float> transaction);
  float calculatePriceOfStockPurchase(std::string stockCode, int num);

  bool executeStockSale(int customerId,
                        std::tuple<std::string, int, float> transaction);
  std::vector<float> calculateResultOfSale(int customerId,
                                           std::string stockCode, int num);
};

#endif

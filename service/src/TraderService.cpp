// This class implements TraderService.h, refer to that header file for further
// information

// TODO:
// THIS CLASS IS LARGELY UNIMPLEMENTED AND REQUIRES MORE WORK.

#include "TraderService.h"
#include <string>

TraderService::TraderService() {}

TraderService::~TraderService() {}

// Directs the dataManager to provide a list of all customers based on the
// passed in employee id.
std::vector<Customer> TraderService::getListOfManagedCustomers(int employeeId) {
  return dataManager.getAllCustomersByTraders(employeeId);
}

std::vector<Stock> TraderService::getListOfAvailableStocks() {
  return dataManager.getAllStoredStocks();
}

// Calculate the price of a given number of stocks and return a string for
// display
float TraderService::calculatePriceOfStockPurchase(std::string stockCode,
                                                   int num) {
  for (Stock stock : dataManager.getAllStoredStocks()) {
    if (stock.stockCode == stockCode) {
      return (float)stock.stockPrice * num;
    }
  }
  // If the stock code doesn't match, return this display error
  return 0.0f;
}

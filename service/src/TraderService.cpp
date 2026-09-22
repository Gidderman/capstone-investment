// This class implements TraderService.h, refer to that header file for further
// information

// TODO:
// THIS CLASS IS LARGELY UNIMPLEMENTED AND REQUIRES MORE WORK.

#include "TraderService.h"
#include "Investment.h"
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

// Recieves the stock code, the number of stocks purchased, and the total cost
// of the transactions as arguments in the tuple
bool TraderService::executeStockPurchase(
    Customer customer, std::tuple<std::string, int, float> transaction) {
  Customer editedCustomer = customer;

  for (Investment investment : editedCustomer.investments) {
    if (std::get<0>(transaction) == investment.stock.stockCode) {
      investment.numHeld += std::get<1>(transaction);
      investment.initialInvestment += std::get<2>(transaction);
      investment.currentInvestmentWorth =
          (float)investment.numHeld * investment.stock.stockPrice;
      return dataManager.updateCustomer(customer, editedCustomer);
    }
  }
  for (Stock stock : dataManager.getAllStoredStocks()) {
    if (std::get<0>(transaction) == stock.stockCode) {
      Investment newInvestment;
      newInvestment.investmentID = -1;
      newInvestment.customerID = editedCustomer.customerID;
      newInvestment.stock = stock;
      newInvestment.numHeld = std::get<1>(transaction);
      newInvestment.initialInvestment = std::get<2>(transaction);
      newInvestment.currentInvestmentWorth = newInvestment.initialInvestment;
      editedCustomer.investments.push_back(newInvestment);
      return dataManager.updateCustomer(customer, editedCustomer);
    }
  }
  return false;
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

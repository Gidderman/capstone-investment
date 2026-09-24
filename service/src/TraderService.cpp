// This class implements TraderService.h, refer to that header file for further
// information

// TODO:
// THIS CLASS IS LARGELY UNIMPLEMENTED AND REQUIRES MORE WORK.

#include "TraderService.h"
#include "Investment.h"
#include <iterator>
#include <ostream>
#include <string>

#include <iostream>

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

  std::cout << "TraderService::executeStockPurchase - Investment size prior to "
               "update: "
            << editedCustomer.investments.size() << std::endl;

  for (Investment investment : editedCustomer.investments) {
    if (std::get<0>(transaction) == investment.stock.stockCode) {

      std::cout << "TraderService::executeStockPurchase (existing investment) "
                   "- running"
                << std::endl;

      investment.numHeld += std::get<1>(transaction);
      investment.initialInvestment += std::get<2>(transaction);
      investment.currentInvestmentWorth =
          (float)investment.numHeld * investment.stock.stockPrice;

      std::cout
          << "TraderService::executeStockPurchase (existing investment) - end"
          << std::endl;
      return dataManager.updateCustomer(customer, editedCustomer);
    }
  }
  for (Stock stock : dataManager.getAllStoredStocks()) {
    if (std::get<0>(transaction) == stock.stockCode) {
      std::cout
          << "TraderService::executeStockPurchase (new investment) - start"
          << std::endl;

      Investment newInvestment;
      newInvestment.investmentID = -1;
      newInvestment.customerID = editedCustomer.customerID;
      newInvestment.stock = stock;
      newInvestment.numHeld = std::get<1>(transaction);
      newInvestment.initialInvestment = std::get<2>(transaction);
      newInvestment.currentInvestmentWorth = newInvestment.initialInvestment;
      editedCustomer.investments.push_back(newInvestment);

      std::cout << "TraderService::executeStockPurchase (new investment) - end"
                << std::endl;
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

// TODO: these
bool TraderService::executeStockSale(
    int customerId, std::tuple<std::string, int, float> transaction) {
  Customer customer = dataManager.getCustomer(customerId);
  Customer editedCustomer = customer;

  for (unsigned int i = 0; i < editedCustomer.investments.size(); i++) {
    if (editedCustomer.investments.at(i).stock.stockCode ==
        std::get<0>(transaction)) {
      // Did the user sell all the held stock?
      if (editedCustomer.investments.at(i).numHeld <=
          std::get<1>(transaction)) {
        editedCustomer.investments.erase(editedCustomer.investments.begin() +
                                         i);
      } else {
        editedCustomer.investments.at(i).numHeld -= std::get<1>(transaction);
      }
      editedCustomer.uninvestedFunds += std::get<2>(transaction);
    }
  }

  return dataManager.updateCustomer(customer, editedCustomer);
}

std::vector<float> TraderService::calculateResultOfSale(int customerId,
                                                        std::string stockCode,
                                                        int num) {
  std::vector<float> transactionInfo;
  Customer customer = dataManager.getCustomer(customerId);

  std::cout
      << "TraderService::calculateResultOfSale - iterating through stock list:"
      << std::endl;
  for (Investment investment : customer.investments) {

    std::cout << investment.stock.stockName << std::endl;
    if (investment.stock.stockCode == stockCode) {
      std::cout << "Stock found: " << investment.stock.stockName << std::endl;
      transactionInfo.push_back(
          investment.initialInvestment); // Initial investment
      transactionInfo.push_back(
          investment.stock.stockPrice *
          (float)investment.numHeld); // Current investment worth
      transactionInfo.push_back(num * investment.stock.stockPrice -
                                transactionInfo.at(0)); // Result of transaction
      std::cout << "Transaction determined" << std::endl;
    }
  }

  return transactionInfo;
}

int TraderService::getNumOfHeldStock(Customer customer, std::string stockCode) {
  for (Investment investment : customer.investments) {
    if (investment.stock.stockCode == stockCode) {
      return investment.numHeld;
    }
  }
}

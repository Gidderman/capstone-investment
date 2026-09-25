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

std::tuple<Employee, int>
TraderService::getEmployeeByAndNumManagedCustomersById(int employeeId) {
  // TODO: DATABASE QUERY OPTIMIZATION

  return {dataManager.getEmployee(employeeId),
          dataManager.getAllCustomersByTraders(employeeId).size()};
}

// Directs the dataManager to provide a list of all customers based on the
// passed in employee id.
std::vector<Customer> TraderService::getListOfManagedCustomers(int employeeId) {
  return dataManager.getAllCustomersByTraders(employeeId);
}

Customer TraderService::getCustomerById(int customerId) {
  return dataManager.getCustomer(customerId);
}

std::vector<Stock> TraderService::getListOfAvailableStocks() {
  return dataManager.getAllStoredStocks();
}

// Recieves the stock code, the number of stocks purchased, and the total cost
// of the transactions as arguments in the tuple
bool TraderService::executeStockPurchase(
    int customerId, std::tuple<std::string, int, float> transaction) {

  Customer customer = dataManager.getCustomer(customerId);
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

  for (Investment investment : customer.investments) {

    if (investment.stock.stockCode == stockCode) {
      transactionInfo.push_back(
          investment.initialInvestment); // Initial investment
      transactionInfo.push_back(
          investment.stock.stockPrice *
          (float)investment.numHeld); // Current investment worth
      transactionInfo.push_back(num * investment.stock.stockPrice -
                                transactionInfo.at(0)); // Result of transaction
      transactionInfo.push_back((float)investment.numHeld);
    }
  }

  return transactionInfo;
}

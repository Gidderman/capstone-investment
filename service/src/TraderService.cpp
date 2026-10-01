// This class implements TraderService.h, refer to that header file for further
// information

#include "TraderService.h"
#include "DataManager.h"
#include "Investment.h"
#include <stdexcept>
#include <string>

#include <iostream>

TraderService::TraderService(DataManager &dataManager)
    : dataManager(dataManager) {}

TraderService::~TraderService() {}

// Directs the dataManager to provide a list of all customers based on the
// passed in employee id.
std::vector<Customer> TraderService::getListOfManagedCustomers(int employeeId) {
  std::vector<Customer> allCustomers =
      dataManager.getAllCustomersByTraders(employeeId);

  if (allCustomers.size() == 0) {
    throw std::logic_error("TraderService::getListOfManagedCustomers -> " +
                           dataManager.getErrorInfo());
  }

  return allCustomers;
}

Customer TraderService::getCustomerById(int customerId) {
  Customer fetchedCustomer = dataManager.getCustomer(customerId);

  if (fetchedCustomer.customerID == -1) {
    throw std::logic_error("TraderService::getCustomerById -> " +
                           dataManager.getErrorInfo());
  }

  return fetchedCustomer;
}

std::vector<Stock> TraderService::getListOfAvailableStocks() {
  std::vector<Stock> fetchedStocks = dataManager.getAllStoredStocks();

  if (fetchedStocks.size() == 0) {
    throw std::logic_error("TraderService::getCustomerById -> " +
                           dataManager.getErrorInfo());
  }

  return fetchedStocks;
}

// Recieves the stock code, the number of stocks purchased, and the total cost
// of the transactions as arguments in the tuple
void TraderService::executeStockPurchase(
    int customerId, std::tuple<std::string, int, float> transaction) {
  Customer fetchedCustomer = dataManager.getCustomer(customerId);

  if (fetchedCustomer.customerID == -1) {
    throw std::logic_error(
        "TraderService::executeStockPurchase -> While fetching customer. \n" +
        dataManager.getErrorInfo());
  }

  // Determine if this investment already exists.
  Investment updatedInvestment;
  for (Investment investment : fetchedCustomer.investments) {
    if (std::get<0>(transaction) == investment.stock.stockCode) {
      updatedInvestment = investment;
    }
  }

  if (updatedInvestment.investmentID == -1) {
    // Investment doesn't exist, so we are creating a new investment
    updatedInvestment.numHeld = std::get<1>(transaction);
    updatedInvestment.stock = dataManager.getStock(std::get<0>(transaction));
    updatedInvestment.initialInvestment = std::get<2>(transaction);
    updatedInvestment.currentInvestmentWorth =
        updatedInvestment.numHeld * updatedInvestment.stock.stockPrice;
    updatedInvestment.customerID = customerId;

    if (!dataManager.createInvestment(updatedInvestment)) {
      throw std::logic_error("TraderService::executeStockPurchase -> While "
                             "creating a new investment.\n" +
                             dataManager.getErrorInfo());
    }

  } else {
    // Investment exists alread, adding to the total
    updatedInvestment.numHeld += std::get<1>(transaction);
    updatedInvestment.currentInvestmentWorth =
        updatedInvestment.numHeld * updatedInvestment.stock.stockPrice;

    if (!dataManager.updateInvestment(updatedInvestment.investmentID,
                                      updatedInvestment)) {
      throw std::logic_error("TraderService::executeStockPurchase -> While "
                             "updating investment.\n" +
                             dataManager.getErrorInfo());
    }
  }

  std::cout << "Subtracting " << std::to_string(std::get<2>(transaction))
            << " from " << std::to_string(fetchedCustomer.uninvestedFunds)
            << std::endl;

  fetchedCustomer.uninvestedFunds -= std::get<2>(transaction);
  if (!dataManager.updateCustomer(fetchedCustomer.customerID,
                                  fetchedCustomer)) {
    throw std::logic_error(
        "TraderService::executeStockPurchase -> while updating customer.\n" +
        dataManager.getErrorInfo());
  }
}

// Calculate the price of a given number of stocks and return a string for
// display
float TraderService::calculatePriceOfStockPurchase(std::string stockCode,
                                                   int num) {
  Stock stock = dataManager.getStock(stockCode);
  return stock.stockPrice * num;
}

void TraderService::executeStockSale(
    int customerId, std::tuple<std::string, int, float> transaction) {
  Customer fetchedCustomer = dataManager.getCustomer(customerId);
  if (fetchedCustomer.customerID == -1) {
    throw std::logic_error(
        "TraderService::executeStockSale -> While fetching customer.\n" +
        dataManager.getErrorInfo());
  }

  Investment investmentToEdit;
  for (Investment investment : fetchedCustomer.investments) {
    if (investment.stock.stockCode == std::get<0>(transaction)) {
      investmentToEdit = investment;
    }
  }

  if (investmentToEdit.numHeld <= std::get<1>(transaction)) {
    // We have sold all the stock.
    if (!dataManager.deleteInvestment(investmentToEdit.investmentID)) {
      throw std::logic_error(
          "TraderService::executeStockSale -> While deleting investment.\n" +
          dataManager.getErrorInfo());
    }
  } else {
    // We have only sold some of the investment.
    investmentToEdit.numHeld -= std::get<1>(transaction);
    investmentToEdit.currentInvestmentWorth =
        investmentToEdit.numHeld * investmentToEdit.stock.stockPrice;

    if (!dataManager.updateInvestment(investmentToEdit.investmentID,
                                      investmentToEdit)) {
      throw std::logic_error(
          "TraderService::executeStockSale -> While updating investment.\n" +
          dataManager.getErrorInfo());
    }
  }

  fetchedCustomer.uninvestedFunds += std::get<2>(transaction);
  if (!dataManager.updateCustomer(fetchedCustomer.customerID,
                                  fetchedCustomer)) {
    throw std::logic_error(
        "TraderService::executeStockSale -> While updating customer.\n" +
        dataManager.getErrorInfo());
  }
}

std::vector<float> TraderService::calculateResultOfSale(int customerId,
                                                        std::string stockCode,
                                                        int num) {
  std::vector<float> transactionInfo;
  Customer customer = dataManager.getCustomer(customerId);
  if (customer.customerID == -1) {
    throw std::logic_error(
        "TraderService::calculateResultOfSale -> While fetching customer.\n" +
        dataManager.getErrorInfo());
  }

  for (Investment investment : customer.investments) {

    if (investment.stock.stockCode == stockCode) {
      transactionInfo.push_back(
          investment.initialInvestment); // Initial investment
      transactionInfo.push_back(
          investment.stock.stockPrice *
          (float)investment.numHeld); // Current investment worth
      // result of the transaction
      transactionInfo.push_back(num * investment.stock.stockPrice);
      transactionInfo.push_back((float)investment.numHeld);
    }
  }

  return transactionInfo;
}

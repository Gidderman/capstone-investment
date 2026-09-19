#include "TraderService.h"
#include <iterator>

TraderService::TraderService() {}

TraderService::~TraderService() {}

std::vector<Customer> TraderService::getListOfManagedCustomers(int employeeId) {
  return dataManager.getAllCustomersByTraders(employeeId);
}

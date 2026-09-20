// This class implements TraderService.h, refer to that header file for further
// information

// TODO:
// THIS CLASS IS LARGELY UNIMPLEMENTED AND REQUIRES MORE WORK.

#include "TraderService.h"

TraderService::TraderService() {}

TraderService::~TraderService() {}

// Directs the dataManager to provide a list of all customers based on the
// passed in employee id.
std::vector<Customer> TraderService::getListOfManagedCustomers(int employeeId) {
  return dataManager.getAllCustomersByTraders(employeeId);
}

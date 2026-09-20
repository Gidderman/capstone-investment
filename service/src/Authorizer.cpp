// This class implements Authorizer.h. See that header file for the purpose of
// the class.

#include "Authorizer.h"

Authorizer::Authorizer(ROLE currentUsersRole) {
  this->currRole = currentUsersRole;
}

Authorizer::~Authorizer() {}

bool Authorizer::authorizeUser(ROLE requiredRole) {
  return currRole == requiredRole;
}

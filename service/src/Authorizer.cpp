#include "Authorizer.h"

Authorizer::Authorizer(ROLE currentUsersRole) {
  this->currRole = currentUsersRole;
}

Authorizer::~Authorizer() {}

bool Authorizer::authorizeUser(ROLE requiredRole) {
  return currRole == requiredRole;
}

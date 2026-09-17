#ifndef AUTHORIZER_H
#define AUTHORIZER_H

#include "Role.h"

class Authorizer {
private:
  ROLE currRole;

public:
  Authorizer(ROLE currentUsersRole);
  ~Authorizer();
  bool authorizeUser(ROLE requiredRole);
};

#endif

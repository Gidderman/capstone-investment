// The Authorizer class is used to authorize users before they access major
// portions of the application. Upon initialialization a role is passed in. This
// current role is the allowed role for the logged in user, and cannot be
// changed once set, and requires the user to log out and log in as a different
// user to change. A pointer to the initialized class is passed to the entry
// points of the various controllers, where a authentication check is
// immediately performed via the authorize user method, passing in the role that
// is required to be authorized in that portion of the application.

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

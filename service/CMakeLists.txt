add_library(Services)

target_sources(Services
  PRIVATE
    src/AdminService.cpp
    src/Authorizer.cpp
    src/DataManager.cpp
    src/InitService.cpp
    src/LogInService.cpp
    src/TraderService.cpp

  PUBLIC
    FILE_SET HEADERS
    FILES
      AdminService.h
      Authorizer.h
      DataManager.h
      InitService.h
      LogInService.h
      TraderService.h
)

#Necessary for password hashing via libsodium
find_package(PkgConfig REQUIRED)
pkg_check_modules(sodium REQUIRED IMPORTED_TARGET libsodium)

target_link_libraries(Services
  PUBLIC
    Utils

  PRIVATE
    Models
    PkgConfig::sodium
)

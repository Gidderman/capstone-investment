add_executable(Investment)

target_sources(Investment
  PRIVATE
    main.cpp
)

target_link_libraries(Investment
  PRIVATE
    Consolidated
)

#LIBRARY CONSOLIDATION
add_library(Consolidated INTERFACE)

target_link_libraries(Consolidated
  INTERFACE
    Controllers
    Models
    Utils
    Services
    Views
    ViewUtils
)

#CONTROLLER LIBRARY
add_library(Controllers)

#Target sources
target_sources(Controllers
  PRIVATE
    src/AdminController.cpp
    src/MasterController.cpp
    src/TraderController.cpp

  PUBLIC
    FILE_SET HEADERS
    FILES 
      AdminController.h
      MasterController.h
      TraderController.h
)

target_link_libraries(Controllers
  PRIVATE
    Services
    Views
    Models
    ViewUtils
)

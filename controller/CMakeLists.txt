#Create target library
add_library(Controllers)

add_executable(Investment)

target_sources(Investment
  PRIVATE
    main.cpp
)

target_link_libraries(Investment
  PRIVATE
    Controllers
    Models
    Utils
    Services
    Views
)

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

add_library(Views)

target_sources(Views
  PRIVATE
    src/AdminMainView.cpp 
    src/CustomerCreationView.cpp
    src/CustomerManagerView.cpp
    src/LogInView.cpp
    src/PurchaseStockWindow.cpp
    src/SellStockWindow.cpp
    src/TraderCreationView.cpp
    src/TraderMainView.cpp
    src/WarningWindow.cpp

  PUBLIC
    FILE_SET HEADERS
    FILES 
      AdminMainView.h 
      CustomerCreationView.h 
      CustomerManagerView.h 
      LogInView.h 
      PurchaseStockWindow.h 
      SellStockWindow.h 
      TraderCreationView.h 
      TraderMainView.h 
      WarningWindow.h 
)

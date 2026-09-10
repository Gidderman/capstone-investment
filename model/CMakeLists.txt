add_library(Models)

target_sources(Models
  PRIVATE
    src/CRUDManager.cpp

  PUBLIC
    FILE_SET HEADERS
    FILES
      CRUDManager.h
)

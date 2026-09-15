cmake_minimum_required(VERSION 4.4)

project(Investment)

#This is necessary to generate the moc .cpp files automatically
#for all files containing the Q_OBJECT macro. In short, it builds
#the signal definitions automatically.
set(CMAKE_AUTOMOC ON)

#Add the subdirectories for project as a whole.
add_subdirectory(controller)
add_subdirectory(model)
add_subdirectory(view)
add_subdirectory(service)
add_subdirectory(util)

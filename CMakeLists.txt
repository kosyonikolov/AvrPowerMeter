cmake_minimum_required(VERSION 3.0.0)
project(AvrPowerMeter VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(AvrPowerMeter src/main.cpp src/twi.cpp src/TwiUtils.cpp src/usart.cpp
               src/Hx711.cpp src/Mpu6050.cpp)
target_include_directories(AvrPowerMeter PRIVATE inc/)
target_compile_options(AvrPowerMeter PRIVATE -mmcu=atmega328p)
target_link_options(AvrPowerMeter PRIVATE -mmcu=atmega328p)
target_compile_definitions(AvrPowerMeter PRIVATE -DF_CPU=8000000)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

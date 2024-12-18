# cmake_minimum_required(VERSION 3.13)

# # Include the Pico SDK using the environment variable
# include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)

# project(mpu6050)

# # Initialize the Raspberry Pi Pico SDK
# pico_sdk_init()

# # Create an executable for the project
# add_executable(mpu6050
#     mpu6050.cpp
# )

# # Link the necessary libraries
# target_link_libraries(mpu6050 pico_stdlib hardware_i2c)

# # Enable USB output (for printf over USB)
# pico_enable_stdio_usb(mpu6050 1)
# # Disable UART output (not needed in this case)
# pico_enable_stdio_uart(mpu6050 0)

# # Create map/bin/hex/uf2 files
# pico_add_extra_outputs(mpu6050)



cmake_minimum_required(VERSION 3.13)

# Include the Pico SDK (must be before project)
include(/home/je6ndar/pico-sdk/pico_sdk_import.cmake)

# Set the project name
project(mpu6050 C CXX ASM)

# Set the C and C++ standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the Pico SDK
pico_sdk_init()

# Add the executable
add_executable(${PROJECT_NAME}
    src/mpu6050.cpp
    src/i2cManager.cpp
)

# Include the header files
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Link the Pico standard library and hardware I2C library
target_link_libraries(${PROJECT_NAME} pico_stdlib hardware_i2c)

# Enable USB output, disable UART output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

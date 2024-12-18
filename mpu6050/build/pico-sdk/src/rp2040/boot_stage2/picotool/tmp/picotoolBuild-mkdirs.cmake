# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/je6ndar/IoT/mpu6050/build/_deps/picotool-src"
  "/home/je6ndar/IoT/mpu6050/build/_deps/picotool-build"
  "/home/je6ndar/IoT/mpu6050/build/_deps"
  "/home/je6ndar/IoT/mpu6050/build/pico-sdk/src/rp2040/boot_stage2/picotool/tmp"
  "/home/je6ndar/IoT/mpu6050/build/pico-sdk/src/rp2040/boot_stage2/picotool/src/picotoolBuild-stamp"
  "/home/je6ndar/IoT/mpu6050/build/pico-sdk/src/rp2040/boot_stage2/picotool/src"
  "/home/je6ndar/IoT/mpu6050/build/pico-sdk/src/rp2040/boot_stage2/picotool/src/picotoolBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/je6ndar/IoT/mpu6050/build/pico-sdk/src/rp2040/boot_stage2/picotool/src/picotoolBuild-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/je6ndar/IoT/mpu6050/build/pico-sdk/src/rp2040/boot_stage2/picotool/src/picotoolBuild-stamp${cfgdir}") # cfgdir has leading slash
endif()

# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/je6ndar/IoT/mpu6050/build/_deps/picotool-src"
  "/home/je6ndar/IoT/mpu6050/build/_deps/picotool-build"
  "/home/je6ndar/IoT/mpu6050/build/_deps/picotool-subbuild/picotool-populate-prefix"
  "/home/je6ndar/IoT/mpu6050/build/_deps/picotool-subbuild/picotool-populate-prefix/tmp"
  "/home/je6ndar/IoT/mpu6050/build/_deps/picotool-subbuild/picotool-populate-prefix/src/picotool-populate-stamp"
  "/home/je6ndar/IoT/mpu6050/build/_deps/picotool-subbuild/picotool-populate-prefix/src"
  "/home/je6ndar/IoT/mpu6050/build/_deps/picotool-subbuild/picotool-populate-prefix/src/picotool-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/je6ndar/IoT/mpu6050/build/_deps/picotool-subbuild/picotool-populate-prefix/src/picotool-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/je6ndar/IoT/mpu6050/build/_deps/picotool-subbuild/picotool-populate-prefix/src/picotool-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()

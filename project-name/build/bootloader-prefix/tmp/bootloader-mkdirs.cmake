# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/maeha/esp/v5.3.1/esp-idf/components/bootloader/subproject"
  "F:/Harrael/Download/GitHub/Mae_Keyboard_Code/project-name/build/bootloader"
  "F:/Harrael/Download/GitHub/Mae_Keyboard_Code/project-name/build/bootloader-prefix"
  "F:/Harrael/Download/GitHub/Mae_Keyboard_Code/project-name/build/bootloader-prefix/tmp"
  "F:/Harrael/Download/GitHub/Mae_Keyboard_Code/project-name/build/bootloader-prefix/src/bootloader-stamp"
  "F:/Harrael/Download/GitHub/Mae_Keyboard_Code/project-name/build/bootloader-prefix/src"
  "F:/Harrael/Download/GitHub/Mae_Keyboard_Code/project-name/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "F:/Harrael/Download/GitHub/Mae_Keyboard_Code/project-name/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "F:/Harrael/Download/GitHub/Mae_Keyboard_Code/project-name/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()

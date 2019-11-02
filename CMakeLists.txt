# ------------------------------------------------------------------------------
#   Project settings
#
cmake_minimum_required(VERSION 3.5)
#~ cmake_minimum_required(VERSION 2.8)
project(bigi C)
set (bigi_VERSION_MAJOR 0)
set (bigi_VERSION_MINOR 9)
set (bigi_VERSION_PATCH 1)


# ------------------------------------------------------------------------------
#   Setup output folders
#
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)
#~ set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)


# ------------------------------------------------------------------------------
#   Compiler flags
#
#~ set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ansi -Wall -pedantic -O3")


# ------------------------------------------------------------------------------
#   C macro definitions
#
OPTION(DEBUG "Debug mode" OFF)
if(DEBUG)
    add_definitions(-DDEBUG)
endif(DEBUG)


# ------------------------------------------------------------------------------
#   Add subdirs
#
add_subdirectory(src)
add_subdirectory(examples)

# mikroC PRO for ARM

# mikroCARM.exe -MSF -DBG -pCEC1302 -BIN -DL -SSA -O11111114 -fo16
# 	-N"D:\veragreg_ecosystem\bigi\ide\bigi_cec1302.mcpar"
#	-SP"D:\install\mikroC\bin\mikroC PRO for ARM\Defs\" -SP"D:\install\mikroC\bin\mikroC PRO for ARM\Uses\MCHP M4\" -SP"D:\veragreg_ecosystem\bigi\ide\" -SP"D:\veragreg_ecosystem\bigi\src\" -SP"D:\veragreg_ecosystem\bigi\examples\"
#	-IP"D:\install\mikroC\bin\mikroC PRO for ARM\Uses\MCHP M4\" -IP"D:\veragreg_ecosystem\bigi\include\" -IP"D:\veragreg_ecosystem\bigi\src\"
#	"bigi.c" "bigi_io.c" "cec1302-example.c" "__Lib_Math.emcl" "__Lib_MathDouble.emcl" "__Lib_Delays.emcl" "__Lib_System_CEC1302.emcl" "__Lib_GPIO_CEC1302.emcl" "__Lib_CType.emcl" "__Lib_Sprintf.emcl" "__Lib_UART.emcl"

#~ -MSF : Short message format.
#~ -DBG : Generate debug info needed for simulator and Hardware Debugger.
#~ -P <devicename> : MCU for which compilation will be done.
#~ -BIN : Generate binary file.
#~ -DL : Build all files as libraries in order to distribute MCLs without sources.
#~ -SSA : Enable SSA optimization.
#~ -O : Miscellaneous output options.
#~ -FO <oscillator> : Set oscillator in MHz.
#~ -N <filename> : Full project file name with path.
#~ -SP <directory> : Add <directoryglt; to the search path list.

#~ -B <directory> : Save compiled binary files (*.emcl) to 'directory'.
#~ -Y : Dynamic link for string literals.
#~ -RA : Rebuild all sources in project.
#~ -L : Check and rebuild libraries if needed.
#~ -LHF : Long hex format.
#~ -PF : Project file name.
#~ -EH <filename> : Full EEPROM HEX file name with path.
#~ -HEAP <size> : Heap size in bytes.
#~ -UICD : Build for Hardware Debugger.

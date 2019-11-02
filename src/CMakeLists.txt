# ------------------------------------------------------------------------------
#   Internal include
#
include_directories(${bigi_SOURCE_DIR}/include ${bigi_SOURCE_DIR}/src)


# ------------------------------------------------------------------------------
#   External include & libs
#
#   ---   empty   ---


# ------------------------------------------------------------------------------
#   Sources
#

# local sources
set(SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/bigi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/bigi_io.c
)

# subdirs
#   ---   empty   ---


# ------------------------------------------------------------------------------
#   Static library & Linking
#
add_library(bigi STATIC ${SRC})

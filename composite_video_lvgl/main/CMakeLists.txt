file(GLOB_RECURSE SOURCES "./*.cpp"
    "*/*.cpp"
    "./*.c"
    "*/*.c"
)
set(INCLUDE_DIRSx "./"
)

set(SRCSx "main.c")

idf_component_register(SRCS ${SRCSx} ${SOURCES} INCLUDE_DIRS ${INCLUDE_DIRSx})
idf_build_set_property(COMPILE_OPTIONS "-DLV_CONF_INCLUDE_SIMPLE=1" APPEND)
idf_build_set_property(COMPILE_OPTIONS "-DLV_CONF_PATH= ${CMAKE_SOURCE_DIR}/main/lv_conf.h" APPEND)

# idf_component_register(SRCS "main.cpp" INCLUDE_DIRS "")

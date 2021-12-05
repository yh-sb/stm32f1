cmake_minimum_required(VERSION 3.13)

set(CMAKE_TOOLCHAIN_FILE cmake/toolchain-arm-gcc-cortex.cmake)

get_filename_component(CURRENT_DIRNAME "${CMAKE_CURRENT_LIST_DIR}" NAME)
project(${CURRENT_DIRNAME} C CXX ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

add_compile_options(
    -mcpu=cortex-m3
    -mthumb
    -ffunction-sections
    -fdata-sections
    $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-threadsafe-statics>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-use-cxa-atexit>
)

add_subdirectory(drivers)
add_subdirectory(periph)

set(DEVICE_NAME STM32F100xB)
add_subdirectory(third_party/CMSIS)
#add_subdirectory(third_party/CMSIS/DSP)

add_subdirectory(third_party/FatFs)

set(FREERTOS_PORT_GCC_ARM_CM3 ON)
set(FREERTOS_CONFIG_FILE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR} CACHE STRING "")
add_subdirectory(third_party/FreeRTOS-Kernel)

add_subdirectory(third_party/printf)

add_subdirectory(utilities/fatfs_diskio)
add_subdirectory(utilities/freertos_wrappers)
add_subdirectory(utilities/syslog)

add_executable(${CMAKE_PROJECT_NAME} main.cpp assert.c fault_handlers.c FreeRTOSHooks.c)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    cmsis
    #cmsis-dsp
    drivers
    periph
    fatfs_diskio
    freertos
    fatfs
    printf
    syslog
)

target_link_options(${CMAKE_PROJECT_NAME} PRIVATE
    -mcpu=cortex-m3
    -Wl,--gc-sections
    -specs=nano.specs
    -specs=nosys.specs
    -Wl,-Map=${CMAKE_PROJECT_NAME}.map,--cref
    -T ${CMAKE_CURRENT_SOURCE_DIR}/third_party/CMSIS/Device/ST/STM32F1xx/Source/linker/STM32F100XB_FLASH.ld
)

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES SUFFIX ".elf")
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_SIZE} ${CMAKE_PROJECT_NAME}.elf
    COMMAND ${CMAKE_OBJCOPY} -O binary ${CMAKE_PROJECT_NAME}.elf "${CMAKE_PROJECT_NAME}.bin"
)

# Add targets for flashing, erasing, resetting and debugging
#set(JLINK_PARAMS -device STM32F100RB -if SWD)
include(cmake/debug-probes/st-link.cmake)

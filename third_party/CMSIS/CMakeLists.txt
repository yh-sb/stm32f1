if(NOT DEFINED DEVICE_NAME)
    message(FATAL_ERROR "DEVICE_NAME is not defined. Example: STM32F100xB")
endif()

add_library(cmsis STATIC
    Device/ST/STM32F1xx/Source/startup_$<LOWER_CASE:${DEVICE_NAME}>.s
    Device/ST/STM32F1xx/Source/system_stm32f1xx.c
)

target_include_directories(cmsis
    PUBLIC
        Core/Include
        Device/ST/STM32F1xx/Include
)

target_compile_definitions(cmsis
    PUBLIC
        ${DEVICE_NAME}
    PRIVATE
        HSE_VALUE=8000000
)

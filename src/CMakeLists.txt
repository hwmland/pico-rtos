add_executable(${ProjectName}
    main.c
)

target_include_directories(${ProjectName} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

# Add pico_stdlib library, add more if used
target_link_libraries(${ProjectName}
    pico_stdlib
    FreeRTOS-Kernel-Heap4
)

# Let's change the default baud rate to 250000 (default is 115200)
#target_compile_definitions(${ProjectName} PRIVATE
#    PICO_DEFAULT_UART_BAUD_RATE=250000
#)
# disable usb output, enable uart output
pico_enable_stdio_usb(${ProjectName} 0)
pico_enable_stdio_uart(${ProjectName} 1)

# Need to generate UF2 file for upload to RP2040
pico_add_extra_outputs(${ProjectName})
 
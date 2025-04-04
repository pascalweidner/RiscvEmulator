cmake_minimum_required(VERSION 3.28)
project(riscvemulator)

set(CMAKE_BUILD_TYPE Debug)

set(SOURCES 
    main.c
    machine.c
    32bit/cpu32.c
    32bit/components/bus.c
    32bit/components/dram.c
    32bit/rv32extensions/rv32f.c
    32bit/rv32extensions/rv32i.c
    32bit/rv32extensions/rv32m.c
)

# Add executable
add_executable(riscvemulator ${SOURCES})

# Include dependencies and subdirectories
target_include_directories(riscvemulator PRIVATE 
    ${CMAKE_SOURCE_DIR}/dependencies
    ${CMAKE_SOURCE_DIR}/32bit
    ${CMAKE_SOURCE_DIR}/32bit/components
    ${CMAKE_SOURCE_DIR}/32bit/rv32extensions
)

# Link the math library if needed
target_link_libraries(riscvemulator m)
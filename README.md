# network_stack

This structure contains the network stack.

CAN/ contains FreeRTOS, CSP, and TI board drivers for the TMS570ls1224 board. This file must be compiled into a .lib static library file using the texas instruments toolchain.

test/ contains the application. The linker must be configured to use the .lib artifact from the CAN/ files. Simple.c is a test application that should send CSP packets to itself via the provided loopback interface.


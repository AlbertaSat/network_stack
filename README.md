# Network Stack
Please post updates to slack:
[![slack](https://img.shields.io/badge/slack-join%20the%20conversation-ff69b4.svg)](https://app.slack.com/client/T1QPU1NHG/C5MJN5YN6)

This repository contains work regarding the Network stack, CSP. It includes code from both libcsp, https://github.com/libcsp/libcsp as well as from FreeRTOS https://github.com/FreeRTOS

The files in this project are meant to be built with the Texas Intruments tools, namely
  * [HalCoGen](http://www.ti.com/tool/HALCOGEN)
  * [Code Composer Studio](http://www.ti.com/tool/CCSTUDIO)

## Structure

There are tree distinct components to this project, the FreeRTOS code, the CSP code and the HalCoGen generated drivers for the development boards.

All source files are in the uhf_base/source/ folder, with subdirectories from the CSP library for the routing, operating system, drivers, interfaes, and the transport layer.

The entry point to the application is found in source/sys_main.c - here you will find the main() funciton which begins the client/server code.

The docs/ folder will contain all important documents.

## Notes
#### This is the place for developers to jot down any important lessons learned to speed up the process for future development/debugging
Test

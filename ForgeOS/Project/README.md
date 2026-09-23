# Linux System Information & Resource Monitoring Tool

## Overview

Linux System Information & Resource Monitoring Tool is a user-space systems programming application developed in C for Linux.

The application collects and displays system information and resource statistics using Linux system interfaces, including the `/proc` pseudo-filesystem and POSIX/Linux APIs.

The tool provides a simple terminal-based monitoring interface for:

- System information
- CPU information and usage
- Memory information and usage
- Disk I/O statistics
- Running processes

The monitor automatically refreshes every 10 seconds.

---

## Objectives

The main objectives of the project are:

1. Develop a Linux-based system monitoring application using C.
2. Access Linux system information through `/proc`.
3. Demonstrate Linux file I/O operations.
4. Demonstrate file descriptors and system calls.
5. Monitor CPU and memory resources.
6. Monitor disk I/O statistics.
7. Monitor running processes.
8. Display system information in a simple command-line interface.
9. Apply Operating Systems and Systems Programming concepts in a practical application.

---

## Features

### 1. System Information

Displays:

- Hostname
- Kernel version
- System architecture
- System uptime

Sources/interfaces used:

- `gethostname()`
- `uname()`
- `/proc/version`
- `/proc/uptime`

### 2. CPU Monitoring

Displays:

- CPU model
- Number of logical processors
- Current CPU utilization

CPU utilization is calculated by comparing two CPU-time snapshots from `/proc/stat`.

### 3. Memory Monitoring

Displays:

- Total memory
- Available memory
- Free memory
- Used memory
- Memory usage percentage

Memory information is obtained from:

```text
/proc/meminfo


# ForgeOS — Linux System Information & Resource Monitoring Tool

A lightweight, modular command-line system monitor and user-space shell written in **C** for Linux. ForgeOS combines system-information and resource-monitoring commands with a simple command interpreter, allowing users to inspect their machine and execute common Linux commands from one interface.

**Project Type:** OSSP (Operating Systems and Systems Programming)  
**Platform:** Linux / Ubuntu (tested on WSL 2)  
**Language:** C  
**Compiler:** GCC  
**Build System:** GNU Make

---

## Table of Contents

- [Overview](#overview)
- [Objectives](#objectives)
- [Features](#features)
- [Architecture](#architecture)
- [Project Structure](#project-structure)
- [Requirements](#requirements)
- [Build and Run](#build-and-run)
- [Commands](#commands)
- [Monitoring Modules](#monitoring-modules)
- [Operating System Concepts](#operating-system-concepts)
- [Implementation Details](#implementation-details)
- [Limitations](#limitations)
- [Troubleshooting](#troubleshooting)
- [Team Contributions](#team-contributions)
- [Future Enhancements](#future-enhancements)
- [References](#references)

---

## Overview

ForgeOS is a Linux system-information and resource-monitoring application implemented in C. It reads operating-system information exposed by Linux interfaces such as the **`/proc` filesystem**, uses POSIX/Linux APIs, and presents results in a terminal.

The project also includes **ShellForge**, a basic command-line shell integrated with the monitoring modules. Users can run built-in ForgeOS commands and launch external Linux programs from the shell.

The application is designed as a user-space program. It does not modify the Linux kernel; instead, it obtains system data through interfaces provided by the operating system.

## Objectives

- Retrieve basic system and kernel information.
- Monitor CPU utilization and processor details.
- Display memory usage statistics.
- Report disk I/O statistics.
- List currently visible processes.
- Integrate monitoring functionality into a simple command-line shell.
- Demonstrate practical use of Linux system interfaces and process-management APIs.
- Organize the code into modules for readability and maintainability.

## Features

### System Information
Displays:
- Hostname
- Kernel version
- System architecture
- System uptime

### CPU Monitor
Displays:
- CPU model information
- Number of online processors
- Approximate CPU utilization calculated from `/proc/stat` readings

### Memory Monitor
Displays:
- Total memory
- Available memory
- Free memory
- Used memory
- Memory usage percentage

### Disk Monitor
Displays disk I/O statistics, including:
- Root filesystem's associated block device
- Read and write operation counts
- Sectors read and written
- Read and written data converted to MB

### Process Monitor
Scans numeric directories under `/proc` and displays process information such as process ID, name, and state.

### ShellForge Command Interpreter
Supports project-specific built-in commands and external foreground commands. The shell includes basic command history and directory navigation.

---

## Architecture

The project follows a modular design.

```text
                     +----------------------+
                     |       main.c        |
                     |  Program / Shell    |
                     +----------+-----------+
                                |
                     +----------v-----------+
                     |     ShellForge      |
                     | Input & Tokenizer   |
                     | Executor & Built-ins |
                     | History & Utilities |
                     +----------+-----------+
                                |
             +------------------+------------------+
             |                  |                  |
   +---------v---------+ +-----v----------+ +------v---------+
   | System Information| | Resource       | | Process        |
   | Module            | | Monitors       | | Monitor        |
   +---------+---------+ +--+----+----+---+ +------+--------+
             |              |    |    |            |
             |            CPU Memory Disk           |
             |              |    |    |            |
   +---------v-------------v----v----v------------v--------+
   |      Linux User-Space Interfaces / APIs                |
   | /proc | sysconf() | POSIX APIs | Process APIs          |
   +-------------------------------------------------------+
```

### Execution Flow

1. The program starts and initializes the shell.
2. ShellForge displays its prompt and reads a command from the user.
3. The input is tokenized into a command and arguments.
4. The executor checks whether the command is a built-in.
5. If it is a monitoring command, the corresponding module is invoked.
6. If it is a shell built-in, the shell performs the requested operation.
7. Otherwise, the shell launches an external command using process-management APIs.
8. The shell waits for a foreground child process to finish and displays the prompt again.

---

## Project Structure

```text
ForgeOS/
├── Makefile
├── main.c
├── shellforge.h
├── system_info.c
├── system_info.h
├── cpu_monitor.c
├── cpu_monitor.h
├── memory_monitor.c
├── memory_monitor.h
├── disk_monitor.c
├── disk_monitor.h
├── process_monitor.c
├── process_monitor.h
├── input.c
├── tokenizer.c
├── history.c
├── executor.c
├── builtins.c
└── utils.c
```

The project is organized into separate modules for system information, resource monitoring, command input, parsing, execution, history, and built-in functionality.

---

## Requirements

### Hardware
- A computer capable of running Linux or a Linux environment such as WSL 2.
- Basic CPU and memory resources sufficient to compile and run a C program.

### Software
- Linux (Ubuntu recommended)
- GCC
- GNU Make
- Standard C library and Linux/POSIX development headers
- Terminal application

---

## Build and Run

### 1. Clone or Download the Project

If the project is hosted on GitHub:

```bash
git clone <REPOSITORY-URL>
cd <REPOSITORY-DIRECTORY>
```

If you received the source code as a ZIP file, extract it and open a terminal in the project directory.

### 2. Install Build Tools on Ubuntu

```bash
sudo apt update
sudo apt install build-essential
```

This installs GCC, GNU Make, and common development tools.

### 3. Compile the Project

```bash
make
```

The Makefile builds the executable:

```text
shellforge
```

To rebuild from scratch, if the Makefile provides a clean target:

```bash
make clean
make
```

### 4. Run ShellForge

```bash
./shellforge
```

You should see the ShellForge prompt. Enter a supported built-in command or a Linux command.

### 5. Exit

```text
exit
```

---

## Commands

The following commands are supported by the current implementation.

| Command | Description | Example |
|---------|-------------|---------|
| `help` | Displays available ShellForge commands | `help` |
| `sysinfo` | Displays general system information | `sysinfo` |
| `cpu` | Displays CPU details and utilization | `cpu` |
| `memory` | Displays memory statistics | `memory` |
| `disk` | Displays disk I/O statistics | `disk` |
| `processes` | Lists process information | `processes` |
| `pwd` | Prints the current working directory | `pwd` |
| `cd` | Changes the current directory | `cd /tmp` |
| `echo` | Prints the provided text | `echo Hello ForgeOS` |
| `history` | Displays command history | `history` |
| `clear` | Clears the terminal display | `clear` |
| `exit` | Exits ShellForge | `exit` |

### External Linux Commands

ShellForge can also execute external commands available in the environment, for example:

```bash
ls
ls -l
date
whoami
uname -a
```

Availability depends on whether the command is installed and accessible through the system's `PATH`.

### Example Session

```text
$ ./shellforge

ForgeOS / ShellForge
Type 'help' to view available commands.

shellforge> help
shellforge> sysinfo
shellforge> cpu
shellforge> memory
shellforge> disk
shellforge> processes
shellforge> pwd
shellforge> cd /tmp
shellforge> pwd
shellforge> ls
shellforge> history
shellforge> exit
```

The exact values and formatting of monitoring output vary by machine and execution time.

---

## Monitoring Modules

### 1. System Information Module

The module gathers general system details using Linux/POSIX interfaces:

- `gethostname()` retrieves the hostname.
- `uname()` provides system and kernel information.
- `/proc/version` provides kernel version details.
- `/proc/uptime` is read to obtain system uptime.

### 2. CPU Monitor

- Reads `/proc/cpuinfo` to obtain CPU model information.
- Uses `sysconf(_SC_NPROCESSORS_ONLN)` to determine the number of online processors.
- Reads CPU counters from `/proc/stat`.
- Takes two readings separated by a short interval.
- Calculates an approximate utilization percentage from the difference between the readings.

Because utilization is sampled over time, the reported percentage is not an instantaneous measurement.

### 3. Memory Monitor

- Reads `/proc/meminfo`.
- Extracts values such as `MemTotal`, `MemAvailable`, and `MemFree`.
- Calculates used memory using the implementation's total-versus-available memory calculation.
- Displays memory values and usage percentage.

### 4. Disk Monitor

- Examines `/proc/mounts` to identify the block device associated with the root filesystem.
- Reads the relevant counters from `/proc/diskstats`.
- Extracts read/write operation counts and sector counts.
- Converts sector totals to MB using the sector-size assumption implemented in the program.

Disk statistics are cumulative kernel counters. They represent activity since the counters were initialized, not just activity during the most recent command.

### 5. Process Monitor

- Iterates through numeric directories in `/proc`.
- Treats numeric directory names as process IDs.
- Reads process metadata from the corresponding `/proc/<PID>/` entries.
- Displays process information such as PID, process name, and state.

The process list can change while it is being scanned because processes may start or terminate at any time.

---

## Operating System Concepts

| OS Concept | How It Appears in ForgeOS |
|------------|----------------------------|
| User space and kernel space | ForgeOS runs as a user-space application and obtains data through kernel-provided interfaces. |
| System calls / OS interfaces | Uses APIs such as `gethostname()`, `uname()`, `sysconf()`, and process-management functions. |
| Proc filesystem | Reads `/proc/cpuinfo`, `/proc/stat`, `/proc/meminfo`, `/proc/diskstats`, `/proc/mounts`, `/proc/uptime`, and process directories. |
| Process creation | `fork()` creates a child process for external commands. |
| Program execution | `execvp()` replaces the child process image with the requested program. |
| Process synchronization | `waitpid()` allows the shell to wait for a foreground child process. |
| Command interpretation | Input parsing, tokenization, command dispatch, and execution are handled by separate shell modules. |
| File I/O | Monitoring modules open and read kernel-exposed pseudo-files under `/proc`. |
| Modular system programming | Separate C modules implement monitoring, input, history, built-ins, and execution. |

---

## Implementation Details

### External Command Execution

For an external command, the shell uses the standard process workflow:

```text
User enters command
        |
        v
Shell parses command and arguments
        |
        v
       fork()
      /      \
 Parent      Child
   |           |
 waitpid()   execvp()
   |           |
   |       Runs external
   |       program
   |           |
   +---- waits -+
        |
        v
  Shell prompt returns
```

### Command Parsing

The current tokenizer splits command input using whitespace and constructs an argument list for command execution. This is sufficient for simple commands and arguments but does not implement a complete Bash-compatible grammar.

---

## Limitations

The current version is intentionally a basic educational shell and monitoring utility. It does **not** implement all features of Bash.

Current limitations include:

- Tokenization is based on whitespace; full quoted-string parsing and escape handling are not implemented.
- Environment-variable expansion is not implemented.
- Pipes (`|`) and I/O redirection (`>`, `<`, `>>`) are not implemented.
- Background execution (`&`), job control, and shell pipelines are not implemented.
- Arrow-key command-history navigation is not implemented; history is accessed through the `history` command.
- `cd -` (switching to the previous directory) is not supported.
- Monitoring data depends on the host system and may vary between Linux distributions and environments.
- Disk-device detection and sector-to-MB conversion depend on the assumptions in the current implementation.

---

## Troubleshooting

### `make: command not found`

Install GNU Make and build tools:

```bash
sudo apt update
sudo apt install build-essential
```

### `gcc: command not found`

Install GCC:

```bash
sudo apt install gcc
```

### `./shellforge: No such file or directory`

Make sure you are in the project directory and compile the project:

```bash
make
ls
./shellforge
```

### Permission Denied

If the executable exists but cannot be launched, check its permissions:

```bash
chmod +x shellforge
./shellforge
```

### An External Command Is Not Found

Check whether the command is installed and available in your system's `PATH`. Try the same command directly in the regular Linux terminal to verify it.

### Monitoring Output Differs Between Computers

This is expected. CPU, memory, process, uptime, and disk I/O values depend on the machine, current workload, kernel, and time of measurement.

---

## Team Contributions

| Team Member | Contribution |
|-------------|-------------|
| CH. Sai Hima Sekhar | System information module and Linux system-interface handling |
| K. Shankar | CPU and memory resource-monitoring modules |
| V. Navadeep | Process-monitoring module, testing, and result analysis |

> Verify the official roll numbers and add them here if required by your faculty.

---

## Future Enhancements

Potential improvements for future versions include:

- More robust tokenizer with support for quoted strings and escape sequences.
- Environment-variable expansion.
- Pipes and input/output redirection.
- Background processes and job control.
- Improved command-history navigation.
- More detailed process information, including CPU and memory per process.
- Refreshing or interactive monitoring views.
- More portable disk-device and sector-size handling.
- Unit tests and automated integration tests.
- Improved error messages and input validation.

These are proposed enhancements and are not claimed as implemented in the current version.

---

## References

1. Linux manual pages: `proc(5)`, `fork(2)`, `exec(3)`, `execvp(3)`, `wait(2)`, `waitpid(2)`, `uname(2)`, `gethostname(2)`, and `sysconf(3)`.
2. Linux kernel documentation — `/proc` filesystem and kernel interfaces.
3. GNU Compiler Collection (GCC) documentation.
4. GNU Make documentation.
5. Linux man-pages project: https://man7.org/linux/man-pages/

---

## License

This project was developed for academic coursework. Add a license here if the project is intended to be distributed or reused publicly.

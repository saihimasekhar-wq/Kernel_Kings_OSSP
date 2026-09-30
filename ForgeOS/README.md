# Linux System Monitor with ShellForge

## Overview

ForgeOS is a Linux-based Operating Systems and Systems Programming project developed in C.

The project combines two major components:

1. **Linux System Information and Resource Monitor**
2. **ShellForge - A basic Ubuntu-terminal-style shell**

The monitoring component collects system information and resource statistics using Linux system interfaces such as the `/proc` pseudo-filesystem and POSIX/Linux APIs.

ShellForge provides an interactive command-line environment through which users can execute monitoring commands as well as normal Linux commands.

---

## Objectives

The main objectives of ForgeOS are:

1. Develop a Linux-based systems programming application using C.
2. Demonstrate Operating Systems concepts through a practical implementation.
3. Access Linux system information through `/proc`.
4. Monitor CPU, memory, disk I/O, and processes.
5. Develop an interactive command-line shell.
6. Demonstrate process creation using `fork()`.
7. Execute external Linux programs using `execvp()`.
8. Synchronize parent and child processes using `waitpid()`.
9. Implement shell built-in commands.
10. Integrate the completed modules into a single application.

---

# ShellForge

ShellForge is the command-line component of ForgeOS.

It provides a prompt:

```text
forgeos$

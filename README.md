[![Build Status](https://travis-ci.org/desyon/EmbeddedSystems.svg?branch=master)](https://travis-ci.org/desyon/EmbeddedSystems)
# Embedded Systems

This repository contains 5 tasks for a university lecture embedded systems. The Makefile contains
confidential information about the lecture and therefore cannot be provided. The helper script
`build.sh` was written to build all projects within TracisCI.

## Task 1 - Command Line Parser

Very basic command line parser. The parser is able to parse the following types of arguments:

1. `-x`
2. `-xvalue`
3. `-x value`
4. `-x=value`

## Task 2 - `printf`-Implementation

A basic implementation of the `printf` function. The implementation can print strings with the following formatting options:

- `%d` for signed int
- `%u` for unsigned int
- `%c` for a single char
- `%s` for a string
- `%x` for a hex number
- `%b` for a binary number
- `%%` for an percent sign

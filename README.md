[![License: CC0-1.0](https://img.shields.io/badge/License-CC0%201.0-lightgrey.svg)](http://creativecommons.org/publicdomain/zero/1.0/)


# IronServer: Minimal, dynamic C++ server


![image](doc/logo.png)


## Table of contents
1. [About the project](#About-the-project)
2. [Getting started](#Getting-started)
	
	- [Usage](#Usage)
3. [Documentation](#documentation)

## About the project
IronServer is a minimal dynamic server for linux written in C++. I created this project for fun, and for having a deeper understanding of C++ and the HTTP	 protocol. The Project has no dependencies, except the `std`-libraries and a modern C++-Compiler such as g++ or clang++.


## Getting started


### Usage
First download the project using git, then compile it and start editing the server.

1. Download it using `git clone https://github.com/ironblockhd/ironServer.git`
2. `cd` into the cloned directory. Compile the server using `make`, then run it using `make run`. Test it in your browser by going on localhost:8080
3.  You can either use the inbuilt way, by editing `src/server/server.cpp`, then running `make clean && make` to recompile it and `make run` to run it, or you can include `src/main/server.h`. Use the documentation.

## Documentation
The documentation can be found in `doc/documentation.md`
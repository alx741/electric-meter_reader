Electric Meter Reader
=====================


What?
-----
This is a hardware and software hack for retrieving a home electric meter
measurements to the PC.

The target meter has no identification, manufacturer name or model on it, so
the only way the identify it is by pictures.


TODO: upload some pics



Hardware
--------

* Electric Meter
	- PIC16F630 micro controller
	- HT1621B LCD controller
	- ADE775 Electrical measurements chip

* Hardware Hack
	- PIC16F628A micro controller
	- FTDI232 for USB/serial communication with the PC



Software
--------
- The firmware software is written in C language for the SDCC compiler.

- The host PC/Hardware hack communication software is written in C for *NIX OS
  with Java wrappers.

        **Note that this project is *NIX OS only compatible**

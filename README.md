# LogicGateSim
A logic gate simulator.
Create a series of basic logic gates and have them compute a binary program.
The compiled executable "HackComputer" should be run with a .hdl (Hardware Description Language) file
as a single parameter.
It should parse this and run a test program on that virtual hardware.

## Compiling
On linux, a "build" directory is to be created and the following commands are to be run;

cmake ..

cmake --build .

## To Do List:
- 	Linking of hdl files through includes i.e multi-chip setups.
 	Preferably multithreaded parsing

-	Loading of binary programs from external files

- 	Finish implementation of virtual output peripherals such as LEDs.

- 	Allow tweaking and proper feedback for chip clock/timing

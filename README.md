# bitlab
Various experiments with bit based operations. 
Contains some third party code to test my methods against other methods. 
Educational use only.

# method

A lookup table based method is used to process most of the bit sequences. 

## abstract

Look up tables are widely used in many applications. 
In a nutshell it allows the reuse of computation to gain performance.
The performance gain can be quite significant because it can reduce counteless instructions into a single memory lookup.
A lookup table operates as a custom function, where for each input the ouput is defined in memory.
(For the best performance, the look up table must stay in cache memory.)
Because of this, the range of application is severely limited. 
The input must have a limited number of bits and cannot be applied to any type of input.
In this project, look up tables are used to efficiently process bit sequences.

## look up tables

A look up table is treated as a function which maps short sequences of bits to another short sequence of bits.
It is used to efficiently implement some functions inside a program.
The efficiency is achieved by replacing multiple instructions with a single memory lookup.
Unfortunately it can not be generalised for everything, but it is widely used for optimising at least a part of a function.
For example the x86 CISC instruction `fsin` uses a look up table inside the CPU to compute the sine of a floating point value. [should reference]

The look up table is located in the computer memory. 
The size of occupied by a look up table depends on the number of bits used as input and the number of bits used as output.
More precisely the space used by a look up table is two to the number of input bits, multiplied by the number of output bits.

The number of input bits is limited to the word size that can be used to address a memory region.
But beyond that, the biggest limiting factor for the input is the amount of memory we can use for the look up table.
For example, a 32-bit computer can used 32-bit input. 
But that would require 4GB of RAM, wasting the whole address space on a single lookup table.
A classic input size is 8-bit, using only 256 bytes of RAM for the lookup table.
A reasonable input size is 16-bit, which uses 64KB of RAM, which has no impact on modern applications.
However there is no need to align the input to these classical values. 
By using a bitmask, any input from 18-bit downto 1-bit can be used.
In practice 17-bit input works well with no performance overhead compared to 16-bit. [Experimental results]

The nubmer of output bits is limited by the values that can be adressed.
If the CPU can only load and store 32-bit values from memory, then we're limited to multiples of 32-bits. 
It is up to the programmer how he wishes to use these values.
If the programmer only decides to use 19 bits out of 32 bits, that is a bit wasteful.
The x86 family of processors has the advantage of being able to load and store 32-bit, 16-bit or 8-bit values.






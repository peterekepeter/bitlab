# bitlab 

Various experiments with bit based operations. 
Contains some third party code to test my methods against other methods. 
Educational use only.

# method

A look up table based method is used to process most of the bit sequences. 

## abstract

Look up tables are widely used in many applications. 
In a nutshell it allows the reuse of computation to gain performance.
The performance gain can be quite significant because it can reduce countless instructions into a single memory look up.
A look up table operates as a custom function, where for each input the output is defined in memory.
(For the best performance, the look up table must stay in cache memory.)
Because of this, the range of application is severely limited. 
The input must have a limited number of bits and cannot be applied to any type of input.
In this project, look up tables are used to efficiently process bit sequences.

## classic look up tables

A look up table is treated as a function which maps short sequences of bits to another short sequence of bits.
It is used to efficiently implement some functions inside a program.
The efficiency is achieved by replacing multiple instructions with a single memory lookup.
Unfortunately it can not be generalized for everything, but it is widely used for optimising at least a part of a function.
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
By using a bit-mask, any input from 18-bit down to 1-bit can be used.
In practice 17-bit input works well with no performance overhead compared to 16-bit. [Experimental results]

The number of output bits is limited by the values that can be addressed.
If the CPU can only load and store 32-bit values from memory, then we're limited to multiples of 32-bits. 
It is up to the programmer how he wishes to use these values.
If the programmer only decides to use 19 bits out of 32 bits, that is a bit wasteful.
The x86 family of processors has the advantage of being able to load and store 32-bit, 16-bit or 8-bit values.
If the number of output bits is greater than the largest value that can be loaded, the input can be shifted left by one bit. 
Shifting the input one bit to the left causes the least significant bit to be 0, effectively skipping every second value in memory.
The skipped value can be loaded together with the addressed value to produce a value twice as big as the largest loadable value.

Another important aspect of the look up table is that the values must be computed in the memory region occupied by it.
Depending on the application and the size of the look up table, the time of computation can be long. 
If the programmer wants to store the results of a very expensive recursive search inside a look up table then the time to compute it might take a lot.
The computation time might also be a limiting factor in the chosen size of a look up table.
In these extreme cases, the look up table should be stored on a persistent medium, and only recompute it if necessary.

Some situations require huge look up tables with sizes up to a gigabyte, this is perfectly acceptable.
If the memory reads have a uniform distribution, then most of the reads from the look up tables will result in cache miss.
Depending on what is stored inside the look up table, the performance might be worse than bypassing the look up table and computing the outputs directly.

It should be clear by now the advantages that look up tables can bring to application development. 
However it should also be clear that this method cannot be applied in every case.
For the maximum efficiency the programmer must choose appropriate sizes for both the input and output.
The size occupied in memory affects both performance and memory efficiency.
For optimal performance the look up table must fit inside the cache memory next to the program code and possibly other data.


## look up and carry

In this project, the focus is on implementing algorithms which process sequences of bits. 
These sequences can range from a handful of bits up to streams of several gigabytes. 
It is obvious that these sequences won't fit into the input of a look up table.
The way look up tables are applied is that a word is read from the bit stream, 
the look up table function is applied and then it is written to the output stream.

Some algorithms work seamlessly with these look up tables.
For example counting the number of cleared bits can be easily decomposed to chunks of 16-bits and processed serially.
Any algorithm where the neighboring bits don't affect each other beyond map well to the classic look up tables.
However that's a very limited set of functions which can be implemented.

The method used in this project improves the classic look up table approach to optimization by including another set of bits, called the carry bits.
The algorithm is decomposed into N steps, each processing M bits. 
But between the each step another set of bits can be carried over.
Passing these carry bits from a previous step to the next one has has zero cost, because when compiling to machine code these carry bits remain in processor registers. [claim should be backed by experiment]

Using these carry bits the transition count algorithm has been implemented. 
The transition count algorithm counts the number of times we have a transition from 0 to 1 or from 1 to 0 inside a bit sequence.
This requires at each bit level knowledge about the previous bit. 
This algorithm was implemented using a 17-bit look up table.
On each step, 16 bits are loaded into the processor. 
The 17th bit is the carry bit which is passed from one step to another. 
At the first step the carry needs to be set to an initial value which doesn't affect the final count.

This method can be generalized. 
The biggest challenge is to choose a good data size to process during each step.
A throughput of 16-bits per step seems to be a good design choice in general.
It forces reads and writes of aligned 16-bits. 
The downside of this is that the extra bits required by the carry can make the memory requirements grow very fast.
At 16-bit input and 4-bit carry, 1 megabyte is used up by the look up table.
If many carry bits are required then the throughput must be lowered.

Since the look up and carry method is still just a look up table, it is still subject to the memory and addressing limitations of the system.
The problem it solves is that it allows a more general usage of the look up tables. 
Look up and carry essentially creates a moving window over the input stream where these windows can overlap each other.
Using this method an efficient and simple implementation can be made of the transition count problem.

Look up and carry can be implemented on almost any hardware. 
The requirement are to have a computer with random access memory and basic bit-wise operations, more specifically bit-wise and, bit-wise or and shifting.


## experimental results

The transition count problem has been implemented in multiple ways to test the performance of various methods.
The input is a randomly generates sequence of 1600000000 bits, that is 200 megabytes of random data.
The test was run on a 1.3Ghz Intel Celeron processor.
The following are the running time of the tests:

0: Naive implementation:            2985 ms
1: Look up and carry:                 62 ms
2: Look up and carry, unrolled:       63 ms
3: NIST implementation:             1984 ms
4: NIST implementation, optimised:  1812 ms
5: optimised NIST 1:                1406 ms
6: optimised NIST 2:                 125 ms
7: optimised NIST 3:                 109 ms

Test 0 is the naive implementation. 
The naive implementation operates on a bit by bit level by shifting bits of a word, and testing the previous bit against the next one.
This implementation takes up the most time, but it is not useless. 
It was used to generate the look up tables for look up and carry and to validate the results of the look up and carry methods.

Test 1 is the basic implementation of look up and carry.
It works on 16-bit words on each step and 1 bit is carried over between each step. 
It is 48 times faster than a naive implementation.

Test 2 is a different implementation of look up and carry which unrolls the loop to process 128 bits during each step. 
In some cases it offers a better run time, but the code is much harder to maintain than the basic look up and carry implementation.

Test 3 is the official NIST implementation. 
Here each bit is represented in a byte, wasting 7 bits of memory for every 1 bit from the sequence.
While loading a byte instead is easier than shifting and testing a word, it has a better performance than the naive implementation.
However it should be noted that 5968 ms were taken to allocate extra memory for this method to work.

Test 4 is similar to the test 3, it is the same NIST implementation but with some optimization.


## future work

Currently to make use of the look up and carry method a programmer who has experience with optimization is required. 
The programmer needs to make a good decision on the size of the look up table, and to follow the implementation guidelines presented in this paper.
These steps should be automated. 
Given a 'higher level' description of the function, an automated system can be create which will generate the look up table. 
This will speed up the development and testing of such algorithms and can also eliminate potential implementation bugs.

This look up and carry method defines a computational model. 
As for any computation model, a compiler can be created which translates a high level description into a look up table.
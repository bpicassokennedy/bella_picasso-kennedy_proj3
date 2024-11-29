# Machine Learning Simulator - Project 3
## Program Description 
This project is a **assemly language instruction simulator** that reads and executes a sequence of instructions from an input file, while updating the registers and flags. It emulates basic arithmetic, logical, and shift operations on a set of registers. Again, managing the condition flags based on the results of these operations (and whether or not the suffix -s is present in the opcode instruction).
## How to Compile the Program 
In order to compile the program, first ensure that the makefile and source file (driver.cpp) are all in the same directory. Then run the make command in your terminal. This command will compile and create an executable named proj3. After successful compilation you can run the program using **./proj3**. 
## Code's Working Process 
**readData:** Once an array for the ARM instructions has been established in main, the readData() function is called (passing through the instruction array, and the arrays maximum capacity as parameters). In readData() the *programming-project-3.txt* file is read and stored into the passed array line by line, while simultaneously keeping track of the number of instructions. Once the file is empty, the file is closed and the number of instructions is returned to be used as a looping condition in later functions. 

**display:** The second function called in the program is the display function in main. display() (based on the number returned from readData) loops through the instruction array. While looping each ARM instruction line is printed and another function parseInstruction() is called. Once parseInstruction() is called, the register values and flags are updated and printed in the display function as well. 

**parseInstruction:** In parseInstruction() an entire line of instruction is passed through. First the line is parsed for the opcode. Depending on the opcode, then the instruction is further parsed for Rd, Rn, Rm, or an immediate value. These values of which are stored into a register array. For example for MOV R1 #0x72DF9901, the value #0x72DF9901 is stored into index 1 of the register array. In this function we also determine whether or not the instruction will update the flag conditions checking if the opcode has a suffix -s. Lastly, the register string is converted to an integer for easier utilization in the executeInstruction() function, which is called depending on the opcode instruction (since not all instructions utilize immediate value, Rn, etc). 

**executeInstruction:** In executeInstruction() if the opcode requires use of an immediate value the function parseImmediate() is called. Converting the string to a uint32_t variable, returning it back to the executeInstruction() function to be used in performing the instruction. Then all the opcode instructions are performed (ADD, SUB, XOR, etc). If the update flag condition is true (passed through by the parseInstruction() function) then the flag is updated depending on the results of the instruction. The flags are stored in a Flags structure, which encapsulates all 4 flags into a single object. In using a struct, the flags can be easily passed between functions and kept consistent throughout the entirety of the program. Where if the instruction does not have the suffix - s the flag conditions remain the same. 

**parseImmediate:** parseImmediate() is called when an immediate value is used. First, a substring is created from the imeediate value string passed through (starting at index 3 where the #0x is ignored). Then the newly created string is converted to a uint32_t variable and returned to the executeInstruction() function for use. 

## Result 
MOV R1, #0x72DF9901
R0: 0x0 R1: 0x72DF9901 R2: 0x0 R3: 0x0
R4: 0x0 R5: 0x0 R6: 0x0 R7: 0x0
N: 0 Z: 0 C: 0 V: 0

MOV R2, #0x2E0B484A
R0: 0x0 R1: 0x72DF9901 R2: 0x2E0B484A R3: 0x0
R4: 0x0 R5: 0x0 R6: 0x0 R7: 0x0
N: 0 Z: 0 C: 0 V: 0

ADDS R3, R1, R2
R0: 0x0 R1: 0x72DF9901 R2: 0x2E0B484A R3: 0xA0EAE14B
R4: 0x0 R5: 0x0 R6: 0x0 R7: 0x0
N: 1 Z: 0 C: 0 V: 1 

SUBS R4, R3, R2
R0: 0x0 R1: 0x72DF9901 R2: 0x2E0B484A R3: 0xA0EAE14B
R4: 0x72DF9901 R5: 0x0 R6: 0x0 R7: 0x0
N: 0 Z: 0 C: 1 V: 1

mov r0, #0xAAA5555
R0: 0xAAA5555 R1: 0x72DF9901 R2: 0x2E0B484A R3: 0xA0EAE14B
R4: 0x72DF9901 R5: 0x0 R6: 0x0 R7: 0x0
N: 0 Z: 0 C: 1 V: 1

ands r0, r0, r4
R0: 0x28A1101 R1: 0x72DF9901 R2: 0x2E0B484A R3: 0xA0EAE14B
R4: 0x72DF9901 R5: 0x0 R6: 0x0 R7: 0x0
N: 0 Z: 0 C: 1 V: 1

orr r0, r2, r3
R0: 0xAEEBE94B R1: 0x72DF9901 R2: 0x2E0B484A R3: 0xA0EAE14B
R4: 0x72DF9901 R5: 0x0 R6: 0x0 R7: 0x0
N: 0 Z: 0 C: 1 V: 1

xor r5, r3, r4
R0: 0xAEEBE94B R1: 0x72DF9901 R2: 0x2E0B484A R3: 0xA0EAE14B
R4: 0x72DF9901 R5: 0xD235784A R6: 0x0 R7: 0x0
N: 0 Z: 0 C: 1 V: 1

MOV R6, #0xFFFFFFFF
R0: 0xAEEBE94B R1: 0x72DF9901 R2: 0x2E0B484A R3: 0xA0EAE14B
R4: 0x72DF9901 R5: 0xD235784A R6: 0xFFFFFFFF R7: 0x0
N: 0 Z: 0 C: 1 V: 1

lsrs R7, R6, #2
R0: 0xAEEBE94B R1: 0x72DF9901 R2: 0x2E0B484A R3: 0xA0EAE14B
R4: 0x72DF9901 R5: 0xD235784A R6: 0xFFFFFFFF R7: 0x3FFFFFFF
N: 0 Z: 0 C: 1 V: 1

LSLS R6, R7, #1
R0: 0xAEEBE94B R1: 0x72DF9901 R2: 0x2E0B484A R3: 0xA0EAE14B
R4: 0x72DF9901 R5: 0xD235784A R6: 0x7FFFFFFE R7: 0x3FFFFFFF
N: 0 Z: 0 C: 0 V: 1
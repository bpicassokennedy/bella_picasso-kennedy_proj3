# Machine Learning Simulator - Project 3
## Program Description 
This project is a **machine learning simualtor** 
## How to Compile the Program 
In order to compile the program, first ensure that the makefile and source file (driver.cpp) are all in the same directory. Then run the make command in your terminal. This command will compile and create an executable named proj3. After successful compilation you can run the program using **./proj3**. 
## Code's Working Process 
**readData:** Once an array for the ARM instructions has been established in main, the readData() function is called (passing through the instruction array, and the arrays maximum capacity as parameters). In readData() the *programming-project-3.txt* file is read and stored into the passed array line by line, while simultaneously keeping track of the number of instructions. Once the file is empty, the file is closed and the number of instructions is returned to be used as a looping condition in later functions. 
**display:** The second function called in the program is the display function in main. display() (based on the number returned from readData) loops through the instruction array. While looping each ARM instruction line is printed and another function parseInstruction() is called. Once parseInstruction() is called, the register values and flags are updated and printed in the display function as well. 
**parseInstruction:** 
## Result 
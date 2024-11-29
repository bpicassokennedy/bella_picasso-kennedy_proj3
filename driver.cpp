// 
//  Programmer: Bella Picasso-Kennedy 
//  Date Completed: November 29, 2024
//  Version: 1.0 (CS219 Project 3)
//  Brief Program Description: This project is an assemly language instruction simulator that reads and executes a sequence of instructions from an input file (programming-project-3.txt, coded in readData function), while 
//                             updating the registers and flags. It emulates basic arithmetic, logical, and shift operations on a set of registers. Again, managing the condition flags based on the results of these operations (and whether 
//                             or not the suffix -s is present in the opcode instruction).
//

#include <iostream> //input + output stream library 
#include <string> // defintion for the string class 
#include <sstream> // provides string classes like stringstream
#include <fstream> // reading input from the txt file
#include <cstdint> // provides fixed width integer types (ex. 32-bit unsigned integer)
#include <iomanip> // input + output manipulator, control hexademical formatting 

struct Flags{ // struct for the condition flags used in function prototypes, needs to be defined prior 
    bool N, Z, C, V; // negative, zero, carry, overflow 
};

//function declarations (prototypes)
int readData(std::string arr[], int numInstructions);
void display(std::string instructions[], uint32_t registers[], int numInstructions, Flags& flags);
void parseInstruction(std::string instruction, uint32_t registers[], std::string instructions[], Flags& flags);
void executeInstruction(std::string opcode, int rd, int rn, int rm, std::string immediateStr, uint32_t registers[], std::string instructions[], bool updateFlags, Flags& flags);
uint32_t parseImmediate(std::string line); // NOTE: c++ compiler processes the code from top to bottom, so when a function is called in main it needs to know the functions signature (return type, parameters, + name) 

int main(){
    static const int maxCount = 50;
    std::string instructions[maxCount]; // stores each line of instruction 
    
    int numInstructions = readData(instructions, maxCount); 
  
    static const int numRegisters = 8;
    uint32_t registers[numRegisters] = {0}; // initialize all registers to 0

    Flags flags = {0, 0, 0, 0}; // initialize all flags to 0 

    display(instructions, registers, numInstructions, flags); // display runs all other functions required for the program to successfully run
    
    return 0; // code executed successfully! 
}

//function defintions 
int readData(std::string arr[], int max){ // want to modify the array so not passing through const
    std::ifstream inFile("programming-project-3.txt");
    if(!inFile){
        std::cout << "error opening file!" << std::endl;
    }
    int count = 0; // initialize instruction count to 0  
    std::string instruction;
    while(inFile.good() && count < max){ 
        getline(inFile, instruction);
        arr[count] = instruction;
        count++; 
    }
    inFile.close();
    return count;
}

void display(std::string instructions[], uint32_t registers[], int numInstructions, Flags& flags){
    for(int i = 0; i < numInstructions; i++){
        std::cout << instructions[i] << std::endl;
        parseInstruction(instructions[i], registers, instructions, flags); // hex and uppercase for correct formatting 
        std::cout << "R0: 0x" << std::hex << std::uppercase << registers[0] << " R1: 0x" << std::hex << std::uppercase << registers[1] << " R2: 0x" << std::hex << std::uppercase << registers[2] << " R3: 0x" << std::hex << std::uppercase << registers[3] << std::endl;
        std::cout << "R4: 0x" << std::hex << std::uppercase << registers[4] << " R5: 0x" << std::hex << std::uppercase << registers[5] << " R6: 0x" << std::hex << std::uppercase << registers[6] << " R7: 0x" << std::hex << std::uppercase << registers[7] << std::endl;
        std::cout << "N: " << flags.N << " Z: " << flags.Z << " C: " << flags.C << " V: " << flags.V << std::endl << std::endl; 
    }
}

void parseInstruction(std::string instruction, uint32_t registers[], std::string instructions[], Flags& flags){
    std::string opcode, rd, rn, rm, immediate;
    std::stringstream ss(instruction);
    bool updateFlags = false; // are we updating instructions or preserving from previous instruction?
    ss >> opcode; // get the opcode (instruction)

    for(int i = 0; i < opcode.length(); i++){ // making opcode instruction capital so we don't have to worry about case sensitivty (could have used transform?) 
        if(opcode[i] >= 'a' && opcode[i] <= 'z'){
            opcode[i] = opcode[i] - ('a' - 'A'); 
        }
    }

    if(opcode == "MOV"){
        ss >> rd >> immediate; 
        char num = rd[1]; // casting an array at index [1] to a char, then to an int to pass through executeInstruction...
        int rdNum = num - '0'; // this will be used to determine what register we are reading values from and writing values to in the executeInstructions function call(s)
        executeInstruction(opcode, rdNum, 0, 0, immediate, registers, instructions, updateFlags, flags);
    }
    else if(opcode == "LSLS" || opcode == "LSRS"){
        ss >> rd >> rn >> immediate;
        if(opcode.back() == 'S' || opcode.back() == 's'){ // checking whether or not the opcode instruction will update the flags or not.. either way this pointer isp assed through to executeInstruction 
            updateFlags = true;
        }
        char num1 = rd[1];
        int rdNum = num1 - '0';
        char num2 = rn[1];
        int rnNum = num2 - '0';
        executeInstruction(opcode, rdNum, rnNum, 0, immediate, registers, instructions, updateFlags, flags);
    }
    else if(opcode == "ADDS" || opcode == "SUBS" || opcode == "ANDS" || opcode == "ORR" || opcode == "XOR"){
        ss >> rd >> rn >> rm;
        if(opcode.back() == 'S' || opcode.back() == 's'){
            updateFlags = true; 
        }
        char num1 = rd[1];
        int rdNum = num1 - '0'; // converts char -> int 
        char num2 = rn[1];
        int rnNum = num2 - '0';
        char num3 = rm[1];
        int rmNum = num3 - '0';
        executeInstruction(opcode, rdNum, rnNum, rmNum, "", registers, instructions, updateFlags, flags);
    }
    else{
        std::cout << "operation not supported!" << std::endl;
    }
}

void executeInstruction(std::string opcode, int rd, int rn, int rm, std::string immediateStr, uint32_t registers[], std::string instructions[], bool updateFlags, Flags& flags){
    int immediateValue; 
    bool carry = 0;
    bool overflow = 0;
    if(opcode == "MOV"){
        uint32_t movValue = parseImmediate(immediateStr); // casting a string to a 32-bit unsigned integer 
        registers[rd] = movValue; 
    }
    else if(opcode == "LSLS" || opcode == "LSRS"){
        if(opcode == "LSLS"){
            std::string temp = immediateStr.substr(1); // won't result in an overflow but can cause a carry determined by how many numbers shift out 
            int immediateValue = stoi(temp);
            registers[rd] = registers[rn] << immediateValue;
            carry = (registers[rn] & (1 << (32 - immediateValue))) != 0;
        }
        else if(opcode == "LSRS"){
            std::string temp = immediateStr.substr(1);
            int immediateValue = stoi(temp);
            registers[rd] = registers[rn] >> immediateValue;
            carry = (registers[rn] & (1 << (32 - immediateValue))) != 0;
        }
    }
    else if(opcode == "ADDS" || opcode == "SUBS" || opcode == "ANDS" || opcode == "ORR" || opcode == "XOR"){ // arithmetic operations affect all four flags..
        if(opcode == "ADDS"){
            registers[rd] = registers[rn] + registers[rm];
            carry = (registers[rn] > registers[rd]) || (registers[rm] > registers[rd]);
            overflow = ((~(registers[rn] ^ registers[rm]) & (registers[rn] ^ registers[rd])) & 0x80000000) != 0;
        }
        else if(opcode == "SUBS"){
            registers[rd] = registers[rn] - registers[rm];
            carry = registers[rn] >= registers[rm];
            overflow = (((registers[rn] ^ registers[rm]) & (registers[rn] ^ registers[rm])) & 0x80000000) != 0;
        }
        else if(opcode == "ANDS"){ // whereas logical oeprations only affect the negative and zero flags, carry and overflow remain unchanged
            registers[rd] = registers[rn] & registers[rm];
        }
        else if(opcode == "ORR"){
            registers[rd] = registers[rn] | registers[rm];
        }
        else if(opcode == "XOR"){
            registers[rd] = registers[rn] ^ registers[rm];
        }
        else{
            std::cout << "operation not supported!" << std::endl;
        }
    }
    else{
        std::cout << "operation not supported!" << std::endl;
    }
    if(updateFlags){ // actually updating the flags! these are passed by reference and will make their way all the way  back to the display function we originally called in our main! 
        flags.N = (registers[rd] & 0x80000000) != 0;
        flags.Z = (registers[rd] == 0); 
        if(opcode != "ANDS"){
            flags.C = carry;
            if(opcode != "LSLS" && opcode != "LSRS"){
                flags.V = overflow;
            }
        }
    }
}

uint32_t parseImmediate(std::string line){
    std::string temp = line.substr(3); // ensuring we aren't casting #0x into the hex number 
    std::stringstream ss;
    uint32_t hexValue;
    ss << std::hex << temp;
    ss >> hexValue;
    return hexValue;
}
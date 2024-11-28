#include <iostream>
#include <string> // defintion for the string class 
#include <sstream>
#include <fstream> // reading input from the txt file
#include <cstdint> // provides fixed width integer types (ex. 32-bit unsigned integer)
#include <iomanip> // input + output manipulator, control hexademical formatting 

int readData(std::string arr[], int numInstructions);
void display(std::string instructions[], uint32_t registers[], bool update, int max);
void parseInstruction(std::string instruction, uint32_t registers[], std::string instructions[], bool& update);
void executeInstruction(std::string opcode, int rd, int rn, int rm, std::string immediateStr, uint32_t registers[], bool update, std::string instructions[]);
uint32_t parseImmediate(std::string line);

int main(){
    static const int maxCount = 50;
    std::string instructions[maxCount]; // stores each line of instruction 
    
    int numInstructions = readData(instructions, maxCount); 
  
    static const int numRegisters = 8;
    uint32_t registers[numRegisters] = {0}; // initialize all registers to 0

    bool update = false;
    display(instructions, registers, update, numInstructions); // numInstructions used as looping condition in following functions

    return 0; // code executed successfully! 
}

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

void display(std::string instructions[], uint32_t registers[], bool update, int numInstructions){
    for(int i = 0; i < numInstructions; i++){
        std::cout << instructions[i] << std::endl;
        parseInstruction(instructions[i], registers, instructions, update);  
        std::cout << "R0: 0x" << std::hex << std::uppercase << registers[0] << " R1: 0x" << std::hex << std::uppercase << registers[1] << " R2: 0x" << std::hex << std::uppercase << registers[2] << " R3: 0x" << std::hex << std::uppercase << registers[3] << std::endl;
        std::cout << "R4: 0x" << std::hex << std::uppercase << registers[4] << " R5: 0x" << std::hex << std::uppercase << registers[5] << " R6: 0x" << std::hex << std::uppercase << registers[6] << " R7: 0x" << std::hex << std::uppercase << registers[7] << std::endl << std::endl;
    }
}

void parseInstruction(std::string instruction, uint32_t registers[], std::string instructions[], bool& update){
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
        ss >> rd >> immediate; // moving immediate into rd 
        char num = rd[1];
        int rdNum = num - '0';
        executeInstruction(opcode, rdNum, 0, 0, immediate, registers, updateFlags, instructions);
    }
    else if(opcode == "LSLS" || opcode == "LSRS"){
        ss >> rd >> rn >> immediate;
        if(opcode.back() == 'S'){
            updateFlags = true;
        }
        char num1 = rd[1];
        int rdNum = num1 - '0';
        char num2 = rn[1];
        int rnNum = num2 - '0';
        executeInstruction(opcode, rdNum, rnNum, 0, immediate, registers, updateFlags, instructions);
    }
    else if(opcode == "ADDS" || opcode == "SUBS" || opcode == "ANDS" || opcode == "ORR" || opcode == "XOR"){
        ss >> rd >> rn >> rm;
        if(opcode.back() == 'S'){
            updateFlags = true; 
        }
        char num1 = rd[1];
        int rdNum = num1 - '0';
        char num2 = rn[1];
        int rnNum = num2 - '0';
        char num3 = rm[1];
        int rmNum = num3 - '0';
        executeInstruction(opcode, rdNum, rnNum, rmNum, "", registers, updateFlags, instructions);
    }
    else{
        std::cout << "operation not supported!" << std::endl;
    }
}

void executeInstruction(std::string opcode, int rd, int rn, int rm, std::string immediateStr, uint32_t registers[], bool update, std::string instructions[]){
    int immediateValue; 
    if(opcode == "MOV"){
        uint32_t movValue = parseImmediate(immediateStr);
        registers[rd] = movValue; 
    }
    else if(opcode == "LSLS" || opcode == "LSRS"){
        if(opcode == "LSLS"){
            std::string temp = immediateStr.substr(1);
            int immediateValue = stoi(temp);
            registers[rd] = registers[rn] << immediateValue;
        }
        else if(opcode == "LSRS"){
            std::string temp = immediateStr.substr(1);
            int immediateValue = stoi(temp);
            registers[rd] = registers[rn] >> immediateValue;
        }
    }
    else if(opcode == "ADDS" || opcode == "SUBS" || opcode == "ANDS" || opcode == "ORR" || opcode == "XOR"){
        if(opcode == "ADDS"){
            registers[rd] = registers[rn] + registers[rm];
        }
        else if(opcode == "SUBS"){
            registers[rd] = registers[rn] - registers[rm];
        }
        else if(opcode == "ANDS"){
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
}

uint32_t parseImmediate(std::string line){
    std::string temp = line.substr(3);
    std::stringstream ss;
    uint32_t hexValue;
    ss << std::hex << temp;
    ss >> hexValue;
    return hexValue;
}

void updateFlags(std::string instructions[], bool update){

}

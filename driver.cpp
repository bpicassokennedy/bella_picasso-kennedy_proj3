#include <iostream>
#include <string> // defintion for the string class 
#include <fstream> // reading input from the txt file
#include <cstdint> // provides fixed width integer types (ex. 32-bit unsigned integer)
#include <iomanip> // input + output manipulator, control hexademical formatting 

int readData(std::string arr[], int max);
void parseInstruction(std::string instruction, uint32_t registers[]);

int main(){
    static const int maxCount = 50;
    std::string instructions[maxCount]; // stores each line of instruction 
    
    int numInstructions = readData(instructions, maxCount);
  
    static const int numRegisters = 8;
    uint32_t registers[numRegisters] = {0}; // initialize all registers to 0

    parseInstruction(instructions[0], registers);

    return 0; // code executed successfully! 
}

int readData(std::string arr[], int max){ // want to modify the array so not passing through const
    std::ifstream inFile("programming-project-3.txt");
    if(!inFile){
        std::cout << "error opening file!" << std::endl;
        return 1; // indicating error + abnormal terminiation
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

void parseInstruction(std::string instruction, uint32_t registers[]){
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
        uint32_t immediateNum = stoi(immediate);

        executeInstruction(opcode, rdNum, "", "", immediateNum, registers, updateFlags);
    }
    else if(opcode == "LSLS" || opcode == "LSRS"){
        ss >> rd >> rn >> immediate;
        if(opcode.back() == 'S'){
            updateFlags = true;
        }

       // executeInstruction(opcode, rd, rn, "", immediate, registers, updateFlags);
    }
    else if(opcode == "ADDS" || opcode == "SUBS" || opcode == "ANDS" || opcode == "ORR" || opcode == "XOR"){
        ss >> rd >> rn >> rm;
        if(opcode.back() == 'S'){
            updateFlags = true; 
        }
       // executeInstruction(opcode, rd, rn, rm, "", registers, updateFlags);
    }
    else{
        std::cout << "operation not supported!" << std::endl;
    }
}

void executeInstruction(std::string opcode, int rd, int rn, int rm, uint32_t immediate, uint32_t registers[], bool update){

}

void updateFlags(){
    
}

void display(){

}
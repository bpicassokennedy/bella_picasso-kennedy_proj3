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

        executeInstruction(opcode, rdNum, 0, 0, immediate, registers, updateFlags);
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

        executeInstruction(opcode, rdNum, rnNum, 0, immediate, registers, updateFlags);
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

        executeInstruction(opcode, rdNum, rnNum, rmNum, "", registers, updateFlags);
    }
    else{
        std::cout << "operation not supported!" << std::endl;
    }
}

void executeInstruction(std::string opcode, int rd, int rn, int rm, std::string immediateStr, uint32_t registers[], bool update){
    uint32_t immediateValue; 
    if(update){
        if(opcode == "MOV"){
            immediateValue = parseImmediate(immediateStr);
            registers[rd] = immediateValue; 
        }
        else if(opcode == "LSLS" || opcode == "LSRS"){
            if(opcode == "LSLS"){
                immediateValue = parseImmediate(immediateStr);
                registers[rd] = registers[rn] << immediateValue;
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
        }
        else{
            std::cout << "not supported!" << std::endl;
        }
        updateFlags();
    }
    else{

    }
}

uint32_t parseImmediate(std::string line){
    std::string value = line.substr(1);

}

void updateFlags(){

}

void display(){

}
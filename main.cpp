#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <ctime>
#include <vector>
#include <array>
#include <sstream>
#include <algorithm>


using namespace std;

struct reservation_station
{
    string name;
    bool busy=false;
    string operation="";
    int vj=-1, vk=-1;
    string qj="", qk="";
    int address=-1;
    string label="";
    int instruction=-1;
    int writeflag=0;
    int to_write=-1;
};

struct nums{
    int issued=-1;
    int exec_start=-1;
    int exec_end=-1;
    int write=-1;
};

reservation_station rs[15];
vector<string> reg_status(8,"");
array<int, 8> registers;
int progcount = 0;
int const load_exec=6,store_exec=6,beq_exec=1,call_ret_exec=1,add_sub_exec=2,nor_exec=1,mul_exec=10;
int clock_cycle=0;
vector<nums> instructions;
vector<pair<string, int>> branch;
vector<pair<int, int>> memory_access;



int get_content(int address)
{
    for (int i = 0; i < memory_access.size(); i++)
    {
        if (memory_access[i].first == address)
            return memory_access[i].second;
    }

    cerr << "Error: Memory address " << address << " not found. Returning default value 0." << endl;
    return 0;
}

void store_content(int address, int content)
{
    for (int i = 0; i < memory_access.size(); i++)
    {
        if (memory_access[i].first == address)
        {
            memory_access[i].second = content;
            return;
        }
    }

    cerr << "Error: Memory address " << address << " not found." << endl;
}

void LOAD(int rd, int rs1, int offset, vector<string>& Output)
{
        int address = registers[rs1];
        address += offset;
        int content = get_content(address);
        registers[rd] = registers[rd] + content;
}

void STORE(int rs1, int rs2, int offset, vector<string>& Output)
{

        int address = registers[rs2];
        address += offset;
        int content = (registers[rs1]) & 0xFFFF;
        store_content(address, content);
        progcount++;
        // cout<<"SW: Content of R"<<rs1<<" is stored in the memory with an offset of "<<offset<<endl;
        cout << "SW: Content of word R" << rs1 << " is stored in the memory starting from address " << registers[rs2] << " with an offset of " << offset << endl;
        cout << "Updated Program Count: " << progcount << endl;
}

void BEQ(int rs1, int rs2, int offset, vector<string>& Output)
{
        if (registers[rs1] == registers[rs2])
        {
            progcount += 1 + offset;
        }
        else
        {
            progcount++;
        }
}

void CALL(int rd, int immediate, vector<string>& Output)
{

        registers[rd] = 1 + progcount;
        progcount = (progcount)+(immediate);
}

void RET()
{
    progcount = registers[1];
}

int ADD(int rs1, int rs2)
{
    return (rs1 + rs2);
}

int SUB(int rs1, int rs2)
{
    return (rs1 - rs2);
}


int NOR(int rs1, int rs2)
{
    return (!(rs1 | rs2));
}

int MUL(int rs1,int rs2)
{
    return ((rs1 * rs2) & 0xFFFF);
}

bool valid_load()
{
    if(rs[0].busy==false || rs[1].busy==false)
    {
        return true;
    }
    return false;
}
bool valid_store()
{
    if(rs[2].busy==false || rs[3].busy==false)
    {
        return true;
    }
    return false;
}
bool valid_beq()
{
    if(rs[4].busy==false || rs[5].busy==false)
    {
        return true;
    }
    return false;
}
bool valid_call_ret()
{
    if(rs[6].busy==false)
    {
        return true;
    }
    return false;
}
bool valid_add_sub()
{
    if(rs[7].busy==false || rs[8].busy==false || rs[9].busy==false || rs[10].busy==false)
    {
        return true;
    }
    return false;
}
bool valid_nor()
{
    if(rs[11].busy==false || rs[12].busy==false)
    {
        return true;
    }
    return false;
}
bool valid_mul()
{
    if(rs[13].busy==false || rs[14].busy==false)
    {
        return true;
    }
    return false;
}

int GetReg(string regname) {
    stringstream ss(regname);
    char prefix;
    int regNumber;

    ss >> prefix >> regNumber;
    if (prefix != 'R' || ss.fail()) {
        throw invalid_argument("Invalid register format");
    }
    return regNumber;

}


void issue(vector<string> lines,string& opcode, string& rd, string& rs1, string& rs2, string& offset, string& name)
{
    int commandCount = lines.size();

    string outputString;

        if (progcount < commandCount) {
            string command = lines[progcount];

            replace(command.begin(), command.end(), ',', ' ');


            istringstream ss(command);
            int imm = 0, offset = 0;
            char openParen, closeParen;
            ss >> opcode;
            transform(opcode.begin(), opcode.end(), opcode.begin(), ::toupper);

            if (opcode == "ADD" || opcode == "SUB" || opcode == "NOR" || opcode == "MUL") {

                ss >> rd >> rs1 >> rs2;
            }
            else if (opcode == "LOAD") {

                ss >> rd >> offset >> openParen >> rs1 >> closeParen;
            }
            else if (opcode == "STORE") {
                ss >> rs1 >> offset >> openParen >> rs2 >> closeParen;
            }
            else if (opcode == "BEQ") {
                ss >> rs1 >> rs2 >> offset;
            }
            else if (opcode == "CALL") {
                ss >> name;
            }
            else {
                //branch.push_back(make_pair(opcode, i));
                //cout<<opcode<<" "<<i << endl;
                string opcode1;
                int imm = 0;
                ss >> opcode1;
                transform(opcode1.begin(), opcode1.end(), opcode1.begin(), ::toupper);
                if (!opcode1.empty() && opcode1[0] == ':') {
                    opcode1.erase(0, 1);
                }

                if (opcode == "ADD" || opcode == "SUB" || opcode == "NOR" || opcode == "MUL") {

                    ss >> rd >> rs1 >> rs2;
                }
                else if (opcode == "LOAD") {

                    ss >> rd >> offset >> openParen >> rs1 >> closeParen;
                }
                else if (opcode == "STORE") {
                    ss >> rs1 >> offset >> openParen >> rs2 >> closeParen;
                }
                else if (opcode == "BEQ") {
                    ss >> rs1 >> rs2 >> name;
                }
                else if (opcode == "CALL") {
                    ss >> name;
                }
                else if (opcode == "RET") {
                }
                else {
                    cerr << "Unknown instruction format or opcode: " << opcode << "\n";
                }
                opcode = opcode1;

            }

        }
    nums inststat;
    inststat.issued=clock_cycle;
    if (opcode == "ADD") {
        if(valid_add_sub()){
            if(rs[7].busy==false)
            {
                progcount++;
                instructions.push_back(inststat);
                rs[7].instruction=instructions.size()-1;
                rs[7].busy=true;
                rs[7].operation="ADD";
                if(reg_status.at(GetReg(rs1))==""){
                    rs[7].vj=registers[GetReg(rs1)];
                }
                else{
                    rs[7].qj=reg_status.at(GetReg(rs1));
                }
                if(reg_status.at(GetReg(rs2))==""){
                    rs[7].vk=registers[GetReg(rs2)];
                }
                else{
                    rs[7].qk=reg_status.at(GetReg(rs2));
                }
                reg_status.at(GetReg(rd))=rs[7].name;
                
            }
            else if(rs[8].busy==false)
            {
                progcount++;
                instructions.push_back(inststat);
                rs[8].instruction=instructions.size()-1;
                rs[8].busy=true;
                rs[8].operation="ADD";
                if(reg_status.at(GetReg(rs1))==""){
                    rs[8].vj=registers[GetReg(rs1)];
                }
                else{
                    rs[8].qj=reg_status.at(GetReg(rs1));
                }
                if(reg_status.at(GetReg(rs2))==""){
                    rs[8].vk=registers[GetReg(rs2)];
                }
                else{
                    rs[8].qk=reg_status.at(GetReg(rs2));
                }
                reg_status.at(GetReg(rd))=rs[8].name;
                
            }
            else if(rs[9].busy==false)
            {
                progcount++;
                instructions.push_back(inststat);
                rs[9].instruction=instructions.size()-1;
                rs[9].busy=true;
                rs[9].operation="ADD";
                if(reg_status.at(GetReg(rs1))==""){
                    rs[9].vj=registers[GetReg(rs1)];
                }
                else{
                    rs[9].qj=reg_status.at(GetReg(rs1));
                }
                if(reg_status.at(GetReg(rs2))==""){
                    rs[9].vk=registers[GetReg(rs2)];
                }
                else{
                    rs[9].qk=reg_status.at(GetReg(rs2));
                }
                reg_status.at(GetReg(rd))=rs[9].name;
                
            }
            else if(rs[10].busy==false)
            {
                progcount++;
                instructions.push_back(inststat);
                rs[10].instruction=instructions.size()-1;
                rs[10].busy=true;
                rs[10].operation="ADD";
                if(reg_status.at(GetReg(rs1))==""){
                    rs[10].vj=registers[GetReg(rs1)];
                }
                else{
                    rs[10].qj=reg_status.at(GetReg(rs1));
                }
                if(reg_status.at(GetReg(rs2))==""){
                    rs[10].vk=registers[GetReg(rs2)];
                }
                else{
                    rs[10].qk=reg_status.at(GetReg(rs2));
                }
                reg_status.at(GetReg(rd))=rs[10].name;
                
            }

        }
    }
    else if (opcode == "SUB") {
        if(valid_add_sub()){
            if(rs[7].busy==false)
            {
                progcount++;
                instructions.push_back(inststat);
                rs[7].instruction=instructions.size()-1;
                rs[7].busy=true;
                rs[7].operation="SUB";
                if(reg_status.at(GetReg(rs1))==""){
                    rs[7].vj=registers[GetReg(rs1)];
                }
                else{
                    rs[7].qj=reg_status.at(GetReg(rs1));
                }
                if(reg_status.at(GetReg(rs2))==""){
                    rs[7].vk=registers[GetReg(rs2)];
                }
                else{
                    rs[7].qk=reg_status.at(GetReg(rs2));
                }
                reg_status.at(GetReg(rd))=rs[7].name;
                
            }
            else if(rs[8].busy==false)
            {
                progcount++;
                instructions.push_back(inststat);
                rs[8].instruction=instructions.size()-1;
                rs[8].busy=true;
                rs[8].operation="SUB";
                if(reg_status.at(GetReg(rs1))==""){
                    rs[8].vj=registers[GetReg(rs1)];
                }
                else{
                    rs[8].qj=reg_status.at(GetReg(rs1));
                }
                if(reg_status.at(GetReg(rs2))==""){
                    rs[8].vk=registers[GetReg(rs2)];
                }
                else{
                    rs[8].qk=reg_status.at(GetReg(rs2));
                }
                reg_status.at(GetReg(rd))=rs[8].name;
                
            }
            else if(rs[9].busy==false)
            {
                progcount++;
                instructions.push_back(inststat);
                rs[9].instruction=instructions.size()-1;
                rs[9].busy=true;
                rs[9].operation="SUB";
                if(reg_status.at(GetReg(rs1))==""){
                    rs[9].vj=registers[GetReg(rs1)];
                }
                else{
                    rs[9].qj=reg_status.at(GetReg(rs1));
                }
                if(reg_status.at(GetReg(rs2))==""){
                    rs[9].vk=registers[GetReg(rs2)];
                }
                else{
                    rs[9].qk=reg_status.at(GetReg(rs2));
                }
                reg_status.at(GetReg(rd))=rs[9].name;
                
            }
            else if(rs[10].busy==false)
            {
                progcount++;
                instructions.push_back(inststat);
                rs[10].instruction=instructions.size()-1;
                rs[10].busy=true;
                rs[10].operation="SUB";
                if(reg_status.at(GetReg(rs1))==""){
                    rs[10].vj=registers[GetReg(rs1)];
                }
                else{
                    rs[10].qj=reg_status.at(GetReg(rs1));
                }
                if(reg_status.at(GetReg(rs2))==""){
                    rs[10].vk=registers[GetReg(rs2)];
                }
                else{
                    rs[10].qk=reg_status.at(GetReg(rs2));
                }
                reg_status.at(GetReg(rd))=rs[10].name;
                
            }

        }
    }
    else if (opcode == "NOR") {
        if(valid_nor())
        {
            if(rs[11].busy==false)
            {
                progcount++;
                instructions.push_back(inststat);
                rs[11].instruction=instructions.size()-1;
                rs[11].busy=true;
                rs[11].operation="NOR";
                if(reg_status.at(GetReg(rs1))==""){
                    rs[11].vj=registers[GetReg(rs1)];
                }
                else{
                    rs[11].qj=reg_status.at(GetReg(rs1));
                }
                if(reg_status.at(GetReg(rs2))==""){
                    rs[11].vk=registers[GetReg(rs2)];
                }
                else{
                    rs[11].qk=reg_status.at(GetReg(rs2));
                }
                reg_status.at(GetReg(rd))=rs[11].name;
                
            }
            else if(rs[12].busy==false)
            {
                progcount++;
                instructions.push_back(inststat);
                rs[12].instruction=instructions.size()-1;
                rs[12].busy=true;
                rs[12].operation="NOR";
                if(reg_status.at(GetReg(rs1))==""){
                    rs[12].vj=registers[GetReg(rs1)];
                }
                else{
                    rs[12].qj=reg_status.at(GetReg(rs1));
                }
                if(reg_status.at(GetReg(rs2))==""){
                    rs[12].vk=registers[GetReg(rs2)];
                }
                else{
                    rs[12].qk=reg_status.at(GetReg(rs2));
                }
                reg_status.at(GetReg(rd))=rs[12].name;
                
            }
        }
    }
    else if (opcode == "MUL") {
        if(valid_mul())
        {
            if(rs[13].busy==false)
            {
                progcount++;
                instructions.push_back(inststat);
                rs[13].instruction=instructions.size()-1;
                rs[13].busy=true;
                rs[13].operation="MUL";
                if(reg_status.at(GetReg(rs1))==""){
                    rs[13].vj=registers[GetReg(rs1)];
                }
                else{
                    rs[13].qj=reg_status.at(GetReg(rs1));
                }
                if(reg_status.at(GetReg(rs2))==""){
                    rs[13].vk=registers[GetReg(rs2)];
                }
                else{
                    rs[13].qk=reg_status.at(GetReg(rs2));
                }
                reg_status.at(GetReg(rd))=rs[13].name;
                
            }
            else if(rs[14].busy==false)
            {
                progcount++;
                instructions.push_back(inststat);
                rs[14].instruction=instructions.size()-1;
                rs[14].busy=true;
                rs[14].operation="MUL";
                if(reg_status.at(GetReg(rs1))==""){
                    rs[14].vj=registers[GetReg(rs1)];
                }
                else{
                    rs[14].qj=reg_status.at(GetReg(rs1));
                }
                if(reg_status.at(GetReg(rs2))==""){
                    rs[14].vk=registers[GetReg(rs2)];
                }
                else{
                    rs[14].qk=reg_status.at(GetReg(rs2));
                }
                reg_status.at(GetReg(rd))=rs[14].name;
                
            }
        }
    }
    else if (opcode == "LOAD") {
        if(valid_load())
        {
            if(rs[0].busy==false)
            {
                progcount++;
                instructions.push_back(inststat);
                rs[0].instruction=instructions.size()-1;
                rs[0].busy=true;
                rs[0].operation="LOAD";
                if(reg_status.at(GetReg(rs1))==""){
                    rs[0].vj=registers[GetReg(rs1)];
                }
                else{
                    rs[0].qj=reg_status.at(GetReg(rs1));
                }
                rs[0].address=stoi(offset);
                reg_status.at(GetReg(rd))=rs[0].name;
                
            }
            else if(rs[1].busy==false)
            {
                progcount++;
                instructions.push_back(inststat);
                rs[1].instruction=instructions.size()-1;
                rs[1].busy=true;
                rs[1].operation="LOAD";
                if(reg_status.at(GetReg(rs1))==""){
                    rs[1].vj=registers[GetReg(rs1)];
                }
                else{
                    rs[1].qj=reg_status.at(GetReg(rs1));
                }
                rs[1].address=stoi(offset);
                reg_status.at(GetReg(rd))=rs[1].name;
                
            }
        }
    }
    else if (opcode == "STORE") {
        if(valid_store())
        {
            if(rs[2].busy==false)
            {
                progcount++;
                instructions.push_back(inststat);
                rs[2].instruction=instructions.size()-1;
                rs[2].busy=true;
                rs[2].operation="STORE";
                if(reg_status.at(GetReg(rs1))==""){
                    rs[2].vj=registers[GetReg(rs1)];
                }
                else{
                    rs[2].qj=reg_status.at(GetReg(rs1));
                }
                if(reg_status.at(GetReg(rs2))==""){
                    rs[2].vk=registers[GetReg(rs2)];
                }
                else{
                    rs[2].qk=reg_status.at(GetReg(rs2));
                }
                rs[2].address=stoi(offset);
                
            }
            else if(rs[3].busy==false)
            {
                progcount++;
                instructions.push_back(inststat);
                rs[3].instruction=instructions.size()-1;
                rs[3].busy=true;
                rs[3].operation="STORE";
                if(reg_status.at(GetReg(rs1))==""){
                    rs[3].vj=registers[GetReg(rs1)];
                }
                else{
                    rs[3].qj=reg_status.at(GetReg(rs1));
                }
                if(reg_status.at(GetReg(rs2))==""){
                    rs[3].vk=registers[GetReg(rs2)];
                }
                else{
                    rs[3].qk=reg_status.at(GetReg(rs2));
                }
                rs[3].address=stoi(offset);
                
            }
        }
    }
    else if (opcode == "BEQ") {
        if(valid_beq())
        {
            if(rs[4].busy==false)
            {
                progcount++;
                instructions.push_back(inststat);
                rs[4].instruction=instructions.size()-1;
                rs[4].busy=true;
                rs[4].operation="BEQ";
                if(reg_status.at(GetReg(rs1))==""){
                    rs[4].vj=registers[GetReg(rs1)];
                }
                else{
                    rs[4].qj=reg_status.at(GetReg(rs1));
                }
                if(reg_status.at(GetReg(rs2))==""){
                    rs[4].vk=registers[GetReg(rs2)];
                }
                else{
                    rs[4].qk=reg_status.at(GetReg(rs2));
                }
                rs[4].address=stoi(offset);
                
            }
            else if(rs[5].busy==false)
            {
                progcount++;
                instructions.push_back(inststat);
                rs[5].instruction=instructions.size()-1;
                rs[5].busy=true;
                rs[5].operation="BEQ";
                if(reg_status.at(GetReg(rs1))==""){
                    rs[5].vj=registers[GetReg(rs1)];
                }
                else{
                    rs[5].qj=reg_status.at(GetReg(rs1));
                }
                if(reg_status.at(GetReg(rs2))==""){
                    rs[5].vk=registers[GetReg(rs2)];
                }
                else{
                    rs[5].qk=reg_status.at(GetReg(rs2));
                }
                rs[5].address=stoi(offset);
                
            }
        }
    }
    else if (opcode == "CALL") {
        if(valid_call_ret())
        {
            if(rs[6].busy==false)
            {
                progcount++;
                instructions.push_back(inststat);
                rs[6].instruction=instructions.size()-1;
                rs[6].busy=true;
                rs[6].operation="CALL";
                rs[6].label=name;
            }
           
        }
    }
    else if (opcode == "RET") {
        if(valid_call_ret())
        {
            if(rs[6].busy==false)
            {
                progcount++;
                instructions.push_back(inststat);
                rs[6].instruction=instructions.size()-1;
                rs[6].busy=true;
                rs[6].operation="RET";
                if(reg_status.at(GetReg(rs1))=="")
                {
                    rs[6].vj=registers[GetReg(rs1)];
                }
                else
                {
                    rs[6].qj=reg_status.at(GetReg(rs1));
                }
            }
           
        }
    }
}

void execute(string opcode, string rd, string rs1, string rs2, string offset, string name)
{
    for(int i=0;i<15;i++)
    {
        if(rs[i].busy==false)
        {
            continue;
        }
        else
        {
            if(i==0||i==1)
            {
                if(instructions[rs[i].instruction].exec_start==-1)
                {
                    if((rs[i].vj!=-1)&&(rs[i].vk!=-1))
                    {
                        instructions[rs[i].instruction].exec_start=clock_cycle;
                        instructions[rs[i].instruction].exec_end=instructions[rs[i].instruction].exec_start+load_exec-1;
                    }
                }
                else if(instructions[rs[i].instruction].exec_end==clock_cycle)
                {
                    //rs[i].to_write=LOAD(rd,rs1,offset);
                    rs[i].writeflag=1;
                }
                    
            }
            if(i==2||i==3)
            {
                if(instructions[rs[i].instruction].exec_start==-1)
                {
                    if((rs[i].vj!=-1)&&(rs[i].vk!=-1))
                    {
                        instructions[rs[i].instruction].exec_start=clock_cycle;
                        instructions[rs[i].instruction].exec_end=instructions[rs[i].instruction].exec_start+store_exec-1;
                    }
                }
                else if(instructions[rs[i].instruction].exec_end==clock_cycle)
                {
                    //STORE(rd,rs1,offset);
                    rs[i].writeflag=1;
                }
            }
            if(i==4||i==5)
            {
                if(instructions[rs[i].instruction].exec_start==-1)
                {
                    if((rs[i].vj!=-1)&&(rs[i].vk!=-1))
                    {
                        instructions[rs[i].instruction].exec_start=clock_cycle;
                        instructions[rs[i].instruction].exec_end=instructions[rs[i].instruction].exec_start+beq_exec-1;
                    }
                }
                else if(instructions[rs[i].instruction].exec_end==clock_cycle)
                {
                    //BEQ(rd,rs1,offset);
                    rs[i].writeflag=1;
                }
            }
            if(i==6)
            {
                if(instructions[rs[i].instruction].exec_start==-1)
                {
                    if((rs[i].vj!=-1)&&(rs[i].vk!=-1))
                    {
                        instructions[rs[i].instruction].exec_start=clock_cycle;
                        instructions[rs[i].instruction].exec_end=instructions[rs[i].instruction].exec_start+call_ret_exec-1;
                    }
                }
                else if(instructions[rs[i].instruction].exec_end==clock_cycle)
                {
                    if(rs[i].operation=="CALL")
                    {
                        //CALL(rd,rs1,offset);
                    }
                    if(rs[i].operation=="RET")
                    {
                        //RET(rs1);
                    }
                    rs[i].writeflag=1;
                }

            }
            if(i==7||i==8||i==9||i==10)
            {
                if(instructions[rs[i].instruction].exec_start==-1)
                {
                    if((rs[i].vj!=-1)&&(rs[i].vk!=-1))
                    {
                        instructions[rs[i].instruction].exec_start=clock_cycle;
                        instructions[rs[i].instruction].exec_end=instructions[rs[i].instruction].exec_start+add_sub_exec-1;
                    }
                }
                else if(instructions[rs[i].instruction].exec_end==clock_cycle)
                {
                    if(rs[i].operation=="ADD")
                    {
                        rs[i].to_write=ADD(rs[i].vj,rs[i].vk);
                    }
                    if(rs[i].operation=="SUB")
                    {
                        rs[i].to_write=SUB(rs[i].vj,rs[i].vk);
                    }
                    rs[i].writeflag=1;
                }

            }
            if(i==11||i==12)
            {
                if(instructions[rs[i].instruction].exec_start==-1)
                {
                    if((rs[i].vj!=-1)&&(rs[i].vk!=-1))
                    {
                        instructions[rs[i].instruction].exec_start=clock_cycle;
                        instructions[rs[i].instruction].exec_end=instructions[rs[i].instruction].exec_start+add_sub_exec-1;
                    }
                }
                else if(instructions[rs[i].instruction].exec_end==clock_cycle)
                {
                    rs[i].to_write=NOR(rs[i].vj,rs[i].vk);
                    rs[i].writeflag=1;
                }
            }
            if(i==13||i==14)
            {
                if(instructions[rs[i].instruction].exec_start==-1)
                {
                    if((rs[i].vj!=-1)&&(rs[i].vk!=-1))
                    {
                        instructions[rs[i].instruction].exec_start=clock_cycle;
                        instructions[rs[i].instruction].exec_end=instructions[rs[i].instruction].exec_start+add_sub_exec-1;
                    }
                }
                else if(instructions[rs[i].instruction].exec_end==clock_cycle)
                {
                    rs[i].to_write=MUL(rs[i].vj,rs[i].vk);
                    rs[i].writeflag=1;
                }
            }
        }
    }
}

void write()
{
    for(int i=0;i<15;i++)
    {
        if(rs[i].busy==false)
        {
            continue;
        }
        else
        {
            if(rs[i].writeflag==1)
            {
                if(rs[i].to_write!=-1)
                {
                    for(int j=0;j<8;j++)
                    {
                        if(reg_status[j]==rs[i].name)
                        {
                            registers[j]=rs[i].to_write;
                            reg_status[j]="";
                        }
                    }
                }
                instructions[rs[i].instruction].write=clock_cycle;
                rs[i].busy=false;
                rs[i].operation="";
                rs[i].vj=-1;
                rs[i].vk=-1;
                rs[i].qj="";
                rs[i].qk="";
                rs[i].address=-1;
                rs[i].label="";
                rs[i].instruction=-1;
                rs[i].writeflag=0;
                rs[i].to_write=-1;
            }
        }
    }
}


void tomasulo(vector<string> lines)
{
    rs[0].name = "load1";
    rs[1].name = "load2";
    rs[2].name = "store1";
    rs[3].name = "store2";
    rs[4].name = "beq1";
    rs[5].name = "beq2";
    rs[6].name = "call/ret";
    rs[7].name = "add/sub1";
    rs[8].name = "add/sub2";
    rs[9].name = "add/sub3";
    rs[10].name = "add/sub4";
    rs[11].name = "nor1";
    rs[12].name = "nor2";
    rs[13].name = "mul1";
    rs[14].name = "mul2";
    
    string opcode, rd, rs1, rs2, offset, name;
    
    
    while(true)
    {
        issue(lines, opcode, rd, rs1, rs2, offset, name);
        clock_cycle++;
        execute(opcode, rd, rs1, rs2, offset, name);
        clock_cycle++;
        write();
        clock_cycle++;
        for(int i=0;i<instructions.size();i++)
        {
            cout << i << "  " << instructions[i].issued << "  " << instructions[i].exec_start << "  " << instructions[i].exec_end << "  " << instructions[i].write << endl;
        }
        
        bool all_done = (progcount >= lines.size());
            for (int i = 0; i < 15; i++) {
                if (rs[i].busy) {
                    all_done = false;
                    break;
                }
            }

            if (all_done) break;
        
    }
}


vector<string> readFileToVector(const string& filePath) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error: Could not open the file: " << filePath << endl;
        return {};
    }

    vector<string> fileContents;
    string line;

    while (getline(file, line)) {
        if (!line.empty() && !all_of(line.begin(), line.end(), [](unsigned char c) { return isspace(c); })) {
            fileContents.push_back(line);
        }
    }

    file.close();
    return fileContents;
}


void createBranch(vector<string>& lines)
{
    int commandCount = lines.size();
    for (int i = 0; i < commandCount; ++i)
    {
        string command = lines[i];
        replace(command.begin(), command.end(), ',', ' ');

            
        istringstream ss(command);
        string opcode;
        ss >> opcode;
        transform(opcode.begin(), opcode.end(), opcode.begin(), ::toupper);

        if (!(opcode == "LOAD" || opcode == "STORE" || opcode == "BEQ" || opcode == "CALL" || opcode == "RET" || opcode == "ADD" || opcode == "SUB" || opcode == "NOR" || opcode == "MUL"))
        {
            string opcode1 = opcode;
            opcode1.erase(opcode1.length() - 1, opcode1.length());
            branch.push_back(make_pair(opcode1, i));
            command.erase(0, opcode.length() + 1);
            lines[i] = command;
        }

    }
}

int main()
{
    
    vector<string> result;
    cout << "enter initial program count" << endl;
    cin >> progcount;
    for (int i = 0; i < 10; i++)
    {
        int k = rand() % 256;
        memory_access.emplace_back(i, k);
    }
    registers[0]=0;
    registers[1]=1;
    registers[2]=2;
    registers[3]=3;
    registers[4]=4;
    registers[5]=5;
    cout << memory_access[3].first << "," << memory_access[3].second << endl;
    cout << memory_access[4].first << "," << memory_access[4].second << endl;
    cout << memory_access[5].first << "," << memory_access[5].second << endl;
    cout << memory_access[6].first << "," << memory_access[6].second << endl;
    string inputfilepath = "/Users/youssefibrahim/Desktop/test.txt";
    vector<string> commands = readFileToVector(inputfilepath);
    createBranch(commands);
    tomasulo(commands);
    //string outputfilepath = "test_out.txt";
    //writeVectorToFile(outputfilepath, result);

    return 0;
}

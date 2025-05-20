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
    string operation=NULL;
    int vj=NULL, vk=NULL;
    string qj=NULL, qk=NULL;
    int address=NULL;
    string label="";
    int instruction=NULL;
    int writeflag=0;
    int to_write=0;
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


void issue(string opcode,string rs1, string rs2, string rd, int offset)
{
    if (opcode == "ADD") {
        if(valid_add_sub()){
            if(rs[7].busy==false)
            {
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
                rs[0].busy=true;
                rs[0].operation="LOAD";
                if(reg_status.at(GetReg(rs1))==""){
                    rs[0].vj=registers[GetReg(rs1)];
                }
                else{
                    rs[0].qj=reg_status.at(GetReg(rs1));
                }
                rs[0].address=offset;
                reg_status.at(GetReg(rd))=rs[0].name;
                
            }
            else if(rs[1].busy==false)
            {
                rs[1].busy=true;
                rs[1].operation="LOAD";
                if(reg_status.at(GetReg(rs1))==""){
                    rs[1].vj=registers[GetReg(rs1)];
                }
                else{
                    rs[1].qj=reg_status.at(GetReg(rs1));
                }
                rs[1].address=offset;
                reg_status.at(GetReg(rd))=rs[1].name;
                
            }
        }
    }
    else if (opcode == "STORE") {
        if(valid_store())
        {
            if(rs[2].busy==false)
            {
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
                rs[2].address=offset;
                
            }
            else if(rs[3].busy==false)
            {
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
                rs[3].address=offset;
                
            }
        }
    }
    else if (opcode == "BEQ") {
        if(valid_beq())
        {
            if(rs[4].busy==false)
            {
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
                rs[4].address=offset;
                
            }
            else if(rs[5].busy==false)
            {
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
                rs[5].address=offset;
                
            }
        }
    }
    else if (opcode == "CALL") {
        if(valid_call_ret())
        {
            if(rs[6].busy==false)
            {
                rs[6].busy=true;
                rs[6].operation="CALL";
                rs[6].label=label;
            }
           
        }
    }
    else if (opcode == "RET") {
        if(valid_call_ret())
        {
            if(rs[6].busy==false)
            {
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

void execute()
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
                    if((rs[i].vj!=NULL)&&(rs[i].vk!=NULL))
                    {
                        instructions[rs[i].instruction].exec_start=clock_cycle;
                        instructions[rs[i].instruction].exec_end=instructions[rs[i].instruction].exec_start+load_exec-1;
                    }
                }
                else if(instructions[rs[i].instruction].exec_end==clock_cycle)
                {
                    rs[i].to_write=load(rd,rs1,offset);
                    rs[i].writeflag=1;
                }
                    
            }
            if(i==2||i==3)
            {
                if(instructions[rs[i].instruction].exec_start==-1)
                {
                    if((rs[i].vj!=NULL)&&(rs[i].vk!=NULL))
                    {
                        instructions[rs[i].instruction].exec_start=clock_cycle;
                        instructions[rs[i].instruction].exec_end=instructions[rs[i].instruction].exec_start+store_exec-1;
                    }
                }
                else if(instructions[rs[i].instruction].exec_end==clock_cycle)
                {
                    store(rd,rs1,offset);
                    rs[i].writeflag=1;
                }
            }
            if(i==4||i==5)
            {
                if(instructions[rs[i].instruction].exec_start==-1)
                {
                    if((rs[i].vj!=NULL)&&(rs[i].vk!=NULL))
                    {
                        instructions[rs[i].instruction].exec_start=clock_cycle;
                        instructions[rs[i].instruction].exec_end=instructions[rs[i].instruction].exec_start+beq_exec-1;
                    }
                }
                else if(instructions[rs[i].instruction].exec_end==clock_cycle)
                {
                    beq(rd,rs1,offset);
                    rs[i].writeflag=1;
                }
            }
            if(i==6)
            {
                if(instructions[rs[i].instruction].exec_start==-1)
                {
                    if((rs[i].vj!=NULL)&&(rs[i].vk!=NULL))
                    {
                        instructions[rs[i].instruction].exec_start=clock_cycle;
                        instructions[rs[i].instruction].exec_end=instructions[rs[i].instruction].exec_start+call_ret_exec-1;
                    }
                }
                else if(instructions[rs[i].instruction].exec_end==clock_cycle)
                {
                    if(rs[i].operation=="CALL")
                    {
                        call(rd,rs1,offset);
                    }
                    if(rs[i].operation=="RET")
                    {
                        ret(rs1);
                    }
                    rs[i].writeflag=1;
                }

            }
            if(i==7||i==8||i==9||i==10)
            {
                if(instructions[rs[i].instruction].exec_start==-1)
                {
                    if((rs[i].vj!=NULL)&&(rs[i].vk!=NULL))
                    {
                        instructions[rs[i].instruction].exec_start=clock_cycle;
                        instructions[rs[i].instruction].exec_end=instructions[rs[i].instruction].exec_start+add_sub_exec-1;
                    }
                }
                else if(instructions[rs[i].instruction].exec_end==clock_cycle)
                {
                    if(rs[i].operation=="ADD")
                    {
                        rs[i].to_write=add(rd,rs1,offset);
                    }
                    if(rs[i].operation=="SUB")
                    {
                        rs[i].to_write=sub(rs1);
                    }
                    rs[i].writeflag=1;
                }

            }
            if(i==11||i==12)
            {
                if(instructions[rs[i].instruction].exec_start==-1)
                {
                    if((rs[i].vj!=NULL)&&(rs[i].vk!=NULL))
                    {
                        instructions[rs[i].instruction].exec_start=clock_cycle;
                        instructions[rs[i].instruction].exec_end=instructions[rs[i].instruction].exec_start+add_sub_exec-1;
                    }
                }
                else if(instructions[rs[i].instruction].exec_end==clock_cycle)
                {
                    rs[i].to_write=nor(rd,rs1,offset);
                    rs[i].writeflag=1;
                }
            }
            if(i==13||i==14)
            {
                if(instructions[rs[i].instruction].exec_start==-1)
                {
                    if((rs[i].vj!=NULL)&&(rs[i].vk!=NULL))
                    {
                        instructions[rs[i].instruction].exec_start=clock_cycle;
                        instructions[rs[i].instruction].exec_end=instructions[rs[i].instruction].exec_start+add_sub_exec-1;
                    }
                }
                else if(instructions[rs[i].instruction].exec_end==clock_cycle)
                {
                    rs[i].to_write=mul(rd,rs1,offset);
                    rs[i].writeflag=1;
                }
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
                if(rs[i].to_write!=NULL)
                {
                    for(int j=0;j<8;j++)
                    {
                        if(reg_status[j]==rs[i].name)
                        {
                            registers[j]=rs[i].to_write;
                        }
                    }
                }
                instruction[rs[i].instruction].write=clock_cycle;
                rs[i].busy=false;
                rs[i].operation=NULL;
                rs[i].vj=NULL;
                rs[i].vk=NULL;
                rs[i].qj=NULL;
                rs[i].qk=NULL;
                rs[i].address=NULL;
                rs[i].label="";
                rs[i].instruction=NULL;
                rs[i].writeflag=0;
                rs[i].to_write=0;
            }
        }
    }
}


void tomasulo()
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
    
    while(true)
    {
        issue();
        execute();
        write();
        clock_cycle++;
        
    }
}

int main()
{

    return 0;
}

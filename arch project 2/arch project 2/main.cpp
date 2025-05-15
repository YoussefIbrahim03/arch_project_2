#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <ctime>
#include <vector>
#include <array>
#include <sstream>

using namespace std;


array<int, 8> registers;
int progcount=0;
vector<pair<string,int>> branch;
vector<pair<int, int>> memory_access;

bool Valid_d(int r){
    if (r >= 8) {
        cerr << "Invalid register index!" << endl;
        return false;
    }
    if(r==0 ) {
        cerr << "Register Zero Cannot be overwritten" << endl;
        progcount++;
        return false;
    }
    return true;
}

void writeVectorToFile(const string& filePath,const vector<string>& output) {


    ofstream file(filePath);
    if (!file) {
        throw std::ios_base::failure("Failed to open file: " + filePath);
    }

    
    for (const auto& line : output) {
        file << line << '\n';
    }
    
    file.close();
}

bool Valid_s(int r){
    if (r >= 8) {
        cerr << "Invalid register index!" << endl;
        return false;
    }
    return true;
}


int get_content(int address)
{
    for(int i=0;i<memory_access.size();i++)
    {
        if(memory_access[i].first==address)
            return memory_access[i].second;
    }
    
    cerr << "Error: Memory address " << address << " not found. Returning default value 0." << endl;
    return 0;
}

void store_content(int address, int content)
{
    for(int i=0;i<memory_access.size();i++)
    {
        if(memory_access[i].first==address)
        {
            memory_access[i].second=content;
            return;
        }
    }
    
    cerr << "Error: Memory address " << address << " not found." << endl;
}

void LOAD(int rd, int rs1, int offset, vector<string>& Output)
{
    if(Valid_d(rd) && Valid_s(rs1))
    {
    int address=registers[rs1];
    address+=offset;
    int content=get_content(address);
    registers[rd]=registers[rd]+content;
    progcount++;
    string output = "LW: R" + to_string(rd) + ", (" + to_string(offset) + ")" + "R" + to_string(rs1) + " ( R" + to_string(rd) + "=" + to_string(registers[rd]) + ")\n" +
    "Updated Program Count: " + to_string(progcount) + "\n";
    cout << output;
    Output.push_back(output);
    }
}

void STORE(int rs1, int rs2, int offset, vector<string>& Output)
{
    if(Valid_s(rs1) && Valid_s(rs2))
    {
    int address=registers[rs2];
    address+=offset;
    int content=(registers[rs1])&0xFFFF;
    store_content(address,content);
    progcount++;
   // cout<<"SW: Content of R"<<rs1<<" is stored in the memory with an offset of "<<offset<<endl;
    cout<<"SW: Content of word R"<<rs1<<" is stored in the memory starting from address " << registers[rs2] << " with an offset of "<<offset<<endl;
    cout << "Updated Program Count: " << progcount  << endl;
    }
}

void BEQ(int rs1, int rs2, int offset,vector<string>& Output)
{
    if(Valid_s(rs1) && Valid_s(rs2))
    {
    if(registers[rs1]==registers[rs2])
    {
        progcount+=1+offset;
    }
    else
    {
        progcount++;
    }
    string output = "BEQ: Branch to " + to_string(offset) + " if R" + to_string(rs1) + " is equal to R" + to_string(rs2) + "\n" +
                "Updated Program Count: " + to_string(progcount) + "\n";

cout << output;
Output.push_back(output);

    }
}

void CALL(int rd, int immediate,vector<string>& Output)
{
    if(Valid_d(rd))
    {

    registers[rd]=1+progcount;
    progcount=(progcount)+(immediate);
        
    string output = "CALL: R" + to_string(rd) + "," + to_string(immediate) + " ( R" + to_string(rd) + "=" + to_string(registers[rd]) + ")\n" +
                "Updated Program Count: " + to_string(progcount) + "\n";

cout << output;
Output.push_back(output);

    }
}

void RET()
{
    progcount=registers[1];
}

void ADD(int rd, int rs1, int rs2,vector<string>& Output) {
    if(Valid_d(rd) && Valid_s(rs1) && Valid_s(rs2)){
    progcount+=1;
    int value_rs1 = registers[rs1];
    int value_rs2 = registers[rs2];

    int result = value_rs1 + value_rs2;

    registers[rd] = result;

    string output = "ADD: R" + to_string(rd) + " = R" + to_string(rs1) + " + R" + to_string(rs2) +
                " (" + to_string(value_rs1) + " + " + to_string(value_rs2) +
                " = " + to_string(result) + ")\n" +
                "Updated Program Count: " + to_string(progcount) + "\n";

cout << output;
Output.push_back(output);
    
    }

}

void SUB(int rd, int rs1, int rs2,vector<string>& Output){
    if(Valid_d(rd) && Valid_s(rs1) && Valid_s(rs2)){
    progcount+=1;
    int value_rs1 = registers[rs1];
    int value_rs2 = registers[rs2];

    int result = value_rs1 - value_rs2;

    registers[rd] = result;

    string output = "SUB: R" + to_string(rd) + " = R" + to_string(rs1) + " - R" + to_string(rs2) +
                " (" + to_string(value_rs1) + " - " + to_string(value_rs2) +
                " = " + to_string(result) + ")\n" +
                "Updated Program Count: " + to_string(progcount) + "\n";

cout << output;
Output.push_back(output);

    }
}


void NOR(int rd, int rs1, int rs2,vector<string>& Output){
    if(Valid_d(rd) && Valid_s(rs1) && Valid_s(rs2)){
    progcount+=1;
    int value_rs1 = registers[rs1];
    int value_rs2 = registers[rs2];

    int result = !(value_rs1 | value_rs2);

    registers[rd] = result;

    string output = "NOR: R" + to_string(rd) + " = R" + to_string(rs1) + " | R" + to_string(rs2) +
                " (" + to_string(value_rs1) + " | " + to_string(value_rs2) +
                " = " + to_string(result) + ")\n" +
                "Updated Program Count: " + to_string(progcount) + "\n";

    cout << output;
    Output.push_back(output);

    }
}

void MUL(int rd, int rs1, int rs2,vector<string>& Output) {
    if(Valid_d(rd) && Valid_s(rs1) && Valid_s(rs2)){
    progcount+=1;
    int value_rs1 = registers[rs1];
    int value_rs2 = registers[rs2];

    int result = (value_rs1 * value_rs2)&0xFFFF;

    registers[rd] = result;

    string output = "MUL: R" + to_string(rd) + " = R" + to_string(rs1) + " + R" + to_string(rs2) +
                " (" + to_string(value_rs1) + " + " + to_string(value_rs2) +
                " = " + to_string(result) + ")\n" +
                "Updated Program Count: " + to_string(progcount) + "\n";

cout << output;
Output.push_back(output);
    
    }

}


int GetReg(string regname){
    stringstream ss(regname);
    char prefix;
    int regNumber;

    ss >> prefix >> regNumber;
    if (prefix != 'R' || ss.fail()) {
        throw invalid_argument("Invalid register format");
    }
    return regNumber;

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
 
     if (!(opcode == "LOAD" || opcode == "STORE" || opcode == "BEQ" || opcode == "CALL" || opcode == "RET" || opcode == "ADD"|| opcode == "SUB" || opcode == "NOR" || opcode == "MUL"))
     {
         string opcode1=opcode;
         opcode1.erase(opcode1.length()-1,opcode1.length());
         branch.push_back(make_pair(opcode1, i));
         command.erase(0, opcode.length()+1);
         lines[i]=command;
     }
        
    }
}

void Sim(vector<string> lines,vector<string>& Output){
    int commandCount = lines.size();
    
    string outputString;


    while(progcount<commandCount) {
            string command = lines[progcount];

        replace(command.begin(), command.end(), ',', ' ');


    istringstream ss(command);
    string opcode, rd, rs1, rs2,name;
    int imm=0,offset=0;
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
        int imm=0;;
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
          ss >> rs1 >> rs2 >> offset;
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

    if (opcode== "ADD") {
                ADD(GetReg(rd),GetReg(rs1),GetReg(rs2),Output);
            }
    else if (opcode== "SUB") {
                SUB(GetReg(rd),GetReg(rs1),GetReg(rs2),Output);
            }
    else if (opcode== "NOR") {
                NOR(GetReg(rd),GetReg(rs1),GetReg(rs2),Output);
            }
    else if (opcode== "MUL") {
                MUL(GetReg(rd),GetReg(rs1),GetReg(rs2),Output);
            }
    else if (opcode== "LOAD") {
                LOAD(GetReg(rd),GetReg(rs1),imm,Output);
            }
    else if (opcode== "STORE") {
                STORE(GetReg(rs1),GetReg(rs2),offset,Output);
            }
    else if (opcode== "BEQ") {
                BEQ(GetReg(rs1),GetReg(rs2),offset,Output);
            }
    else if (opcode== "CALL") {
                CALL(GetReg(rd),imm,Output);
            }
    else if (opcode== "RET") {
                RET();
            }

    }
}



int main()
{
    vector<string> result;
    cout << "enter initial program count" << endl;
    cin >> progcount;
    for(int i=0;i<10;i++)
    {
        int k=rand()%256;
        memory_access.emplace_back(i, k);
    }
    cout << memory_access[3].first << "," << memory_access[3].second << endl;
    cout << memory_access[4].first << "," << memory_access[4].second << endl;
    cout << memory_access[5].first << "," << memory_access[5].second << endl;
    cout << memory_access[6].first << "," << memory_access[6].second << endl;
    string inputfilepath="/Users/youssefibrahim/Desktop/test.txt";
    vector<string> commands=readFileToVector(inputfilepath);
    createBranch(commands);
    Sim(commands,result);
    string outputfilepath="/Users/youssefibrahim/Desktop/test_out.txt";
    writeVectorToFile(outputfilepath,result);
    
    
    return 0;
}

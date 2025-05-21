Students:
- Seifeldin Elwan | ID: 900222286
- Youssef Abdelkarim | ID: 900212889

Description:
A C++ implementation of the Tomasulo algorithm for a 16-bit RISC processor. The simulator supports dynamic scheduling, register renaming, and handles data hazards using reservation stations.

What Works:
- Instructions: ADD, SUB, MUL, NOR, LOAD, STORE, BEQ, CALL, RET
- Out-of-order execution and data hazard handling
- Branch resolution with flushing
- Instruction timing tracking (issue, execute, write-back)
- CALL/RET with return address logic

Assumptions:
- Input is provided as a well-formatted text file
- Registers are named R0 to R7
- Memory is randomly initialized
- CALL stores return address in R1

Known Issues:
- None

How to Run:
1. Compile: g++ tomasulo working.cpp -o tomasulo
2. Run: ./tomasulo
3. Set the correct path for the input file in main()

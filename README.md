This is a von Neumann 8-bit breadboard computer with:
* 16-bytes of RAM
* 1 memory address register
* 1 instruction register
* 1 program counter
* 2 general-purpose registers
* 1 arithmetic logic unit
* 1 flags register with 2 flags (carry and zero)
* 1 output register with 7-segment displays
* 16 control signals managed by 2 EEPROMs
* 11 machine language instructions (including conditional and unconditional jumps)

| Control Pin | Function |
| --- | --- |
| HLT | Halt Clock |
| MI | Memory Address Register (IN) |
| RI | RAM (IN) |
| RO | RAM (OUT) |
| IO | Instruction Register (OUT) |
| II | Instruction Register (IN) |
| AI | Register A (IN) |
| AO | Register A (OUT) |
| SU | Subtract |
| EO | Arithmetic Logic Unit (OUT) |
| BI | Register B (IN) |
| OUT | Output Register (IN) |
| CE | Program Counter (ENABLE) |
| CJ | Program Counter (JUMP) |
| CO | Progarm Counter (Out) |
| FI | Flags Register (IN) |

| Instruction | Function | Control Sequence |
| NOP (0000 0000) | No operation | (CO MI) (RO II CE) |
| LDA (0001 xxxx) | Load register A with contents in xxxx | (CO MI) (RO II CE) (IO MI) (RO AI) |
| ADD (0010 xxxx) | Add contents in register A and xxxx | (CO MI) (RO II CE) (IO MI) (RO BI) (EO AI FI) |
| SUB (0011 xxxx) | Subtract contents in register A and xxxx | (CO MI) (RO II CE) (IO MI) (RO BI) (EO AI SU FI) |
| STA (0100 xxxx) | Store contents in register A at xxxx | (CO MI) (RO II CE) (IO MI) (AO RI) |
| LDI (0101 xxxx) | Immediately load xxxx to register A | (CO MI) (RO II CE) (IO MI) (IO AI) |
| JMP (0110 xxxx) | Jump to program stored in xxxx | (CO MI) (RO II CE) (IO MI) (IO CJ) |
| JC  (0111 xxxx) | Jump to program stored in xxxx if carry flag is set | (CO MI) (RO II CE) (IO MI) (IO CJ)* |
| JZ  (1000 xxxx) | Jump to program stored in xxxx if zero flag is set | (CO MI) (RO II CE) (IO MI) (IO CJ)* |
| OUT (1110 0000) | Dump register A contents to output register | (CO MI) (RO II CE) (IO MI) (AO OUT) |
| HLT (1111 0000) | Halt clock signal | (CO MI) (RO II CE) (IO MI) (HLT) |

| Program | Date | YouTube |
| --- | --- | --- |
| Fibonacci Sequence | 07/24/24 | https://youtu.be/Rl5dPWCrEBQ |
| Counting w/ Jump Carry & Zero Flags | 06/20/24 | https://youtu.be/w5fmc0aoqj8 |
| Counting w/ EEPROM Control Logic | 06/02/24 | https://youtu.be/Gm3HwKCBr_c |
| Blinky Program | 06/02/24 | https://youtu.be/H7Rrt8iaJPc |
| Testing LDI & STA Instructions | 06/01/24 | https://youtu.be/u_UtnhLg71E |
| Addition & Subtraction | 06/01/24 | https://youtu.be/H69gBVl0mco |
| Counting w/ Manual Control Logic | 05/25/24 | https://youtu.be/cGufHGSTiuM |
| Testing the ALU | 04/10/24 | https://youtu.be/OblWvOqAwRo |

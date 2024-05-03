#include <cstdlib>
#include <iostream>
#include <string>
#include <map>

int main(int argc, char *argv[])
{
    unsigned int sys0 = std::atoi(argv[1]);
    unsigned int sys1 = std::atoi(argv[2]);

    // decimal to binary
    if(sys0 == 10 && sys1 == 2) {
        int decimal = std::atoi(argv[3]);
        int binary[32], i = 32;
        while(decimal) {
            binary[--i] = decimal & 1;
            decimal >>= 1;
        }
        for(; i < 32; i++)
            std::cout << binary[i];
        std::cout << "\n";
    }

    // binary to decimal
    else if(sys0 == 2 && sys1 == 10) {
        int binary = std::atoi(argv[3]);
        int decimal = 0, power = 0;
        while(binary) {
            decimal += (binary % 10 ? 1 << power : 0);
            binary /= 10;
            power++;
        }
        std::cout << decimal << "\n";
    }

    // binary to hex
    else if(sys0 == 2 && sys1 == 16) {
        std::map<std::string, char> hex_dict;
        hex_dict["0000"] = '0'; hex_dict["0001"] = '1'; hex_dict["0010"] = '2'; hex_dict["0011"] = '3';
        hex_dict["0100"] = '4'; hex_dict["0101"] = '5'; hex_dict["0110"] = '6'; hex_dict["0111"] = '7';
        hex_dict["1000"] = '8'; hex_dict["1001"] = '9'; hex_dict["1010"] = 'A'; hex_dict["1011"] = 'B';
        hex_dict["1100"] = 'C'; hex_dict["1101"] = 'D'; hex_dict["1110"] = 'E'; hex_dict["1111"] = 'F';

        std::string hex = "";
        std::string binary = argv[3];
        for(int i = 0; i < binary.length(); i += 4) {
            std::string group = binary.substr(i, 4);
            hex += hex_dict[group];
        }
        std::cout << hex << "\n";
    }

    else {}

    return 0;
}
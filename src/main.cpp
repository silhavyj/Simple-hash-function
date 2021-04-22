#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>

#include "cxxopts.hpp"

#define F1(B,C) ((B & C) ^ (~B & C))
#define F2(B,C) ((B | C) & (B ^ C))

#define A 0
#define B 1
#define C 2
#define D 3
#define INIT_VECTOR {0x2EC0, 0xE3BC, 0xFD17, 0xA13F}

#define DEBUG(msg) (arg["verbose"].as<bool>() && std::cout << msg << std::flush)

cxxopts::ParseResult arg;
cxxopts::Options options("./hash_BIT <input>", "KIV/BIT task 5 - implementation of a simple hash function");

std::vector<uint16_t> inputData;

void printState(int iteration, uint16_t *prev, uint16_t *next) {
    std::cout << std::dec << "iteration (" << iteration << "):\n";
    std::cout << "prev: ";

    for (int i = 0; i < 4; i++)
        std::cout << std::setfill('0') << std::setw(4) << std::right << std::hex << prev[i] << " ";

    std::cout << "\nnext: ";
    for (int i = 0; i < 4; i++)
        std::cout << std::setfill('0') << std::setw(4) << std::right << std::hex << next[i] << " ";
    std::cout << "\n\n";
}

void calculateHash() {
    DEBUG("starting calculating the hash value...\n");
    uint16_t prev[4] = INIT_VECTOR;
    uint16_t next[4];
    uint16_t f;

    for (int i = 0; i < (int)inputData.size(); i++) {
        f = i & 1 ? F2(prev[B], prev[C]) : F1(prev[B], prev[C]);

        next[A] = f ^ prev[D];
        next[B] = prev[D];
        next[C] = (prev[A] ^ prev[B]) ^ inputData[i];
        next[D] = prev[C];

        if (arg["verbose"].as<bool>())
            printState(i, prev, next);

        memcpy(prev, next, sizeof(next));
    }
    for (int i = 0; i < 4; i++)
        std::cout << std::setfill('0') << std::setw(4) << std::right << std::hex << next[i];
    std::cout << "\n";
}

int readInputFile(std::string fileName) {
    DEBUG("loading the input file...");
    std::ifstream input(fileName, std::ios::binary);
        if (input.fail()) {
        return 1;
    }
    std::vector<uint8_t> buffer(std::istreambuf_iterator<char>(input), {});
    input.close();
    
    for (int i = 0; i < (int)buffer.size(); i++) {
        if (i & 1)
            *inputData.rbegin() |= buffer[i];
        else
            inputData.push_back(buffer[i] << 8);
    }
    DEBUG("OK\n");
    return 0;
}

int main(int argc, char **argv) {
      options.add_options()
        ("v,verbose", "print out info as the program proceeds", cxxopts::value<bool>()->default_value("false"))
        ("h,help", "print help")
    ;
    arg = options.parse(argc, argv);
    if (arg.count("help")) {
        std::cout << options.help() << std::endl;
        return 0;
    }
    if (argc < 2) {
        std::cout << "ERR: The input file is not specified!\n";
        std::cout << "     Run './hash_BIT --help'\n";
        return 1;
    }
    if (readInputFile(argv[1]) != 0) {
        std::cout << "ERR: Input file does not exists!\n";
        return 1;
    }
    calculateHash();
    return 0;
}
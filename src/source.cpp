#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include "assem_instr.h"

#define ubyte uint8_t
#define sbyte int8_t

/*
 * Minimum number of arguments required:
 */
static const uint32_t REQ_ARGS = 2;

void usage(const std::string& pr_name) {
    std::string sub = "PR_NAME";
    std::string usageString = R"(Usage: PR_NAME [rom_path])";
    usageString.replace(usageString.find(sub), sub.length(), pr_name);
    std::cout << usageString << std::endl;
}

std::string toHex(int i) {
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << std::hex << i;
    return ss.str();
}

void throwBytes(uint32_t u_Bytes) {
}

int main(int argc, char** argv) {
    /*
     * Input Validation:
     */
    if (argc < REQ_ARGS) {
        usage(argv[0]);
    }
    /*
     * File System:
     */
    std::ifstream g_Code(argv[1], std::ios_base::binary); // open rom in binary mode
    g_Code >> std::noskipws; // don't skip whitespace
    /*
     * Read header bytes:
     */
    ubyte s_Str;
    for (int i = 0; i < 3; i++) {
        // read header:
        g_Code >> s_Str;
    }
    g_Code.ignore(16 - 3);
    /*
     * Main loop:
     */
    uint32_t u_Instr = 1;
    // main 6502 registers (8-bit):
    Regs regs; // PC, SP, ACC, X, Y
    while (u_Instr && !g_Code.eof()) {
        u_Instr++;
        ubyte b_Op;
        std::string s_Instr;
        g_Code.read((char*)&b_Op, 1);
        auto ign = [&](const uint32_t& s) { g_Code.ignore(s); };
        /*
         * Instructions Opcodes:
         */
        switch (b_Op) {
            case 0x78: s_Instr = "sei"; ign(0); break; // check
            case 0xD8: s_Instr = "cld"; ign(0); break; // check
            case 0xA9: s_Instr = "lda"; ign(1); break; // check
            case 0x8D: s_Instr = "sta"; ign(2); break; // check
            case 0xA2: s_Instr = "ldx"; ign(1); break; // check
            case 0x9A: s_Instr = "txs"; ign(0); break; // check
            case 0xAD: s_Instr = "lda"; ign(2); break; // check
            case 0x10: s_Instr = "bpl"; ign(1); break; // check
            case 0xA0: s_Instr = "ldy"; ign(1); break; // check
            case 0xBD: s_Instr = "lda"; ign(2); break; // check
            case 0xC9: s_Instr = "cmp"; ign(1); break; // check
            case 0xB0: s_Instr = "bcs"; ign(1); break; // check
            case 0xCA: s_Instr = "dex"; ign(0); break; // check
            case 0xD0: s_Instr = "bne"; ign(1); break; // check
            case 0x20: s_Instr = "jsr"; ign(2); break; // check
            case 0xEE: s_Instr = "inc"; ign(2); break; // check
            case 0x09: s_Instr = "ora"; ign(1); break; // check
            case 0x4C: s_Instr = "jmp"; ign(2); break; // check
            case 0x01: s_Instr = "ora"; ign(1); break; // check
            case 0xC8: s_Instr = "iny"; ign(0); break; // check
            case 0xEC: s_Instr = "cpx"; ign(2); break; // check
            case 0x41: s_Instr = "eor"; ign(1); break; // check
            case 0x44: s_Instr = "nop"; ign(1); break; // check
            case 0x68: s_Instr = "pla"; ign(0); break; // check
            case 0x7C: s_Instr = "nop"; ign(2); break; // check
            case 0xDE: s_Instr = "dec"; ign(2); break; // check
            case 0x8C: s_Instr = "sty"; ign(2); break; // check
            case 0x03: s_Instr = "slo"; ign(1); break; // check
            case 0x29: s_Instr = "and"; ign(1); break; // check
            case 0xAC: s_Instr = "ldy"; ign(2); break; // check
            case 0xAE: s_Instr = "ldx"; ign(2); break; // check
            case 0x85: s_Instr = "sta"; ign(1); break; // check
            case 0xE0: s_Instr = "cpx"; ign(1); break;
            case 0xBE: s_Instr = "ldx"; ign(2); break;
            case 0x9D: s_Instr = "sta"; ign(2); break;
            case 0x4A: s_Instr = "lsr"; ign(0); break;
            default: s_Instr = "Not Defined"; u_Instr = 0;
        }
        std::cout << "I" << u_Instr - 1 << ": " << s_Instr << " (" << std::hex << (int)b_Op << ")" << std::endl;
    }
    g_Code.close();
}

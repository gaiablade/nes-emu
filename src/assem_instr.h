#pragma once

#define ubyte uint8_t
#define sbyte int8_t

struct Regs {
    sbyte reg6502[5]; // PC, SP, ACC, X, Y
    ubyte flags[7];   // Carry, Zero, Interrupt, Decimal, Break, Overflow, Negative
};

/*
 * Set interrupt Disable Status
 */
void sei(Regs& regs) {
    regs.flags[2] = 1;
}

/*
 * Clear Decimal Mode
 */
void cld(Regs& regs) {
    regs.flags[3] = 0;
}

/*
 * Load Accumulator
 * $A9: Immediate
 * Load argument value into the ACC register
 */
void lda(Regs& regs, const sbyte& arg) {
    regs.reg6502[2] = arg;
}

/*
 * Store Accumulator
 * $8D: Store Accumulator
 */
void sta(Regs& regs, int16_t addr) {
    // store content in acc into memory at address
    // regs.memory[addr] = regs.regs6502[2];
}

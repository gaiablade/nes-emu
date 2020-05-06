#pragma once

#define ubyte uint8_t
#define sbyte int8_t

const uint32_t MEM_SIZE = 2048; // 2048 bytes of memory (2 kB)

enum R { PC = 0, SP, ACC, X, Y };
enum F { CF = 0, ZF, IF, DF, BF, OF, NF };

struct Regs {
    sbyte reg6502[5]; // PC, SP, ACC, X, Y
    ubyte flags[7];   // Carry, Zero, Interrupt, Decimal, Break, Overflow, Negative
    sbyte* mem = new sbyte[MEM_SIZE];
    ~Regs() { delete[] this->mem; }
};

/*
 * Prototypes:
 */
// TODO: Decide on way to separate instructions with modes that share same
// parameters.
void sei(Regs& regs);                      // imp
void cld(Regs& regs);                      // imp
void dex(Regs& regs);                      // imp
void lda(Regs& regs, const sbyte& arg);    // imm
void lda(Regs& regs, const int16_t& data); // abs
void lda(Regs& regs, const int16_t& addr); // abs,X
void ldx(Regs& regs, const sbyte& val);    // imm
void ldy(Regs& regs, const sbyte& val);    // imm
void sta(Regs& regs, const int16_t& addr); // abs
void txs(Regs& regs);                      // impl
void bpl(Regs& regs, const sbyte& offset); // rel
void bcs(Regs& regs, const sbyte& offset); // rel
void bne(Regs& regs, const sbyte& offset); // rel
void cmp(Regs& regs, const sbyte& val);    // imm
void cpx(Regs& regs, const int16_t& addr); // abs
void jsr(Regs& regs, const int16_t& addr); // abs
void jmp(Regs& regs, const int16_t& addr); // abs
void inc(Regs& regs, const int16_t& addr); // abs
void iny(Regs& regs);                      // imp
void ora(Regs& regs, const sbyte& val);    // imm
void ora(Regs& regs, const sbyte& val);    // (ind,X)
void eor(Regs& regs, const sbyte& val);    // (ind,X)
void nop(Regs& regs);                      // imp

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
 * Decrement X Register
 * $CA: Implied
 */
void dex(Regs& regs) {
    regs.reg6502[R::X] += 1;
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
 * $AD: Absolute
 */
void lda(Regs& regs, const int16_t& data) {
    regs.reg6502[R::ACC] = data;
}
/*
 * $BD: Absolute, X
 */
void lda(Regs& regs, const int16_t& addr) {
    regs.reg6502[R::ACC] = regs.mem[addr + regs.reg6502[R::X]];
}

/*
 * Load X Register
 * $A2: Immediate
 */
void ldx(Regs& regs, const sbyte& val) {
    regs.reg6502[3] = val;
    if (!val)         regs.flags[F::ZF] = 1;
    else if (val < 0) regs.flags[F::NF] = 1;
}

/*
 * Load Y Register
 * $A0: Immediate
 */
void ldy(Regs& regs, const sbyte& val) {
    regs.reg6502[R::Y] = val;
    if (!val)         regs.flags[F::ZF] = 1;
    else if (val < 0) regs.flags[F::NF] = 1;
}

/*
 * Store Accumulator
 * $8D: Absolute
 */
void sta(Regs& regs, const int16_t& addr) {
    // store content in acc into memory at address
    //regs.memory[addr] = regs.reg6502[2];
    regs.mem[addr] = regs.reg6502[2];
}

/*
 * Transfer X to Stack Pointer
 * $9A: Implied
 */
void txs(Regs& regs) {
    regs.reg6502[R::SP] = regs.reg6502[R::X];
}

/*
 * Branch if Positive
 * $10: Relative
 * If the negative flag is clear then add the relative displacement to the program counter to cause a branch to a new location.
 */
void bpl(Regs& regs, const sbyte& offset) {
    if (!regs.flags[F::NF])
        regs.reg6502[R::PC] += offset;
}

/*
 * Branch if Carry Set
 * $B0: Relative
 */
void bcs(Regs& regs, const sbyte& offset) {
    if (regs.flags[F::CF])
        regs.reg6502[R::PC] += offset;
}

/*
 * Branch if Not Equal
 * $D0: Relative
 */
void bne(Regs& regs, const sbyte& offset) {
    if (!regs.flags[F::ZF]) {
        regs.reg6502[R::PC] += offset;
    }
}

/*
 * Compare
 * $C9: Immediate
 */
void cmp(Regs& regs, const sbyte& val) {
    sbyte diff = regs.reg6502[R::ACC] - val;
    if (!diff) {
        regs.flags[F::ZF] = 1;
    }
    else if (diff < 0) {
        regs.flags[F::NF] = 1;
    }
    if (regs.reg6502[R::ACC] >= val) {
        regs.flags[F::CF] = 1;
    }
    else {
        regs.flags[F::CF] = 0;
    }
}

/*
 * Compare X Register
 * $EC: Absolute
 * Updates: Z, C, N
 */
void cpx(Regs& regs, const int16_t& addr) {
    sbyte diff = regs.reg6502[R::X] - regs.mem[addr];
    if (!diff) {
        regs.flags[F::ZF] = 1;
    }
    else if (diff < 0) {
        regs.flags[F::NF] = 1;
    }
    if (regs.reg6502[R::ACC] >= regs.mem[addr]) {
        regs.flags[F::CF] = 1;
    }
    else {
        regs.flags[F::CF] = 0;
    }
}

/*
 * Jump to Subroutine
 * $20: Absolute
 */
void jsr(Regs& regs, const int16_t& addr) {
    regs.reg6502[R::PC] = addr;
}

/*
 * Jump
 * $4C: Absolute
 */
void jmp(Regs& regs, const int16_t& addr) {
    regs.reg6502[R::PC]
}

/*
 * Increment Memory
 * $EE: Absolute
 */
void inc(Regs& regs, const int16_t& addr) {
    regs.mem[addr] += 1;
}

/*
 * Increment Y
 * $C8: Implied
 */
void iny(Regs& regs) {
    regs.reg6502[R::Y] += 1;
}

/*
 * Logical Inclusive OR
 * $09: Immediate
 */
void ora(Regs& regs, const sbyte& val) {
    regs.reg6502[R::ACC] |= val;
}

/*
 * Logical Inclusive OR
 * $01: (Indirect, X)
 */
void ora(Regs& regs, const sbyte& val) {
    const int16_t i16_TargetAddr = regs.mem[regs.reg6502[R::X] + val];
    const sbyte b_LoadVal = regs.mem[i16_TargetAddr];
    regs.reg6502[R::ACC] |= b_LoadVal;
}

/*
 * Exclusive-OR
 * $41: (Indirect, X)
 * Updates: Z, N
 */
void eor(Regs& regs, const sbyte& val) {
    const int16_t i16_TargetAddr = regs.mem[regs.reg6502[R::X] + val];
    const sbyte b_LoadVal = regs.mem[i16_TargetAddr];
    regs.reg6502[R::ACC] ^= b_LoadVal;
    if (!regs.reg6502[R::AC])
        regs.flags[F::ZF] = 1;
    else if (regs.reg6502[R::ACC] >> 7)
        regs.flags[F::NF] = 1;
}

/*
 * No Operation
 * $44: ??? (NES-exclusive)
 */
void nop(Regs& regs) {
    // Waste 3 cycles
}

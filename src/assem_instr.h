#pragma once

#define ubyte uint8_t
#define sbyte int8_t

const uint32_t MEM_SIZE = 2048; // 2048 bytes of memory (2 kB)

enum R { SP = 0, ACC, X, Y };
enum F { CF = 0, ZF, IF, DF, BF, OF, NF };

struct Regs {
    int16_t PC;
    sbyte reg6502[4]; // SP, ACC, X, Y
    ubyte flags[7];   // Carry, Zero, Interrupt, Decimal, Break, Overflow, Negative
    sbyte* mem = new sbyte[MEM_SIZE];
    Regs() {
        this->reg6502[R::SP] = 0xFF;
    }
    ~Regs() { delete[] this->mem; }
};

/*
 * Prototypes:
 */
// TODO: Decide on way to separate instructions with modes that share same
// parameters.
void sei_imp(Regs& regs);                       // imp
void cld_imp(Regs& regs);                       // imp
void dec_absX(Regs& regs, const int16_t& addr); // abs,X
void dex_imp(Regs& regs);                       // imp
void lda_imm(Regs& regs, const sbyte& arg);     // imm
void lda_abs(Regs& regs, const int16_t& data);  // abs
void lda_absX(Regs& regs, const int16_t& addr); // abs,X
void ldx_imm(Regs& regs, const sbyte& val);     // imm
void ldx_abs(Regs& regs, const int16_t& addr);  // abs
void ldy_imm(Regs& regs, const sbyte& val);     // imm
void ldy_abs(Regs& regs, const uint16_t& addr); // abs
void sta_abs(Regs& regs, const int16_t& addr);  // abs
void sta_zp(Regs& regs, const sbyte& val);      // zp
void sty_abs(Regs& regs, const int16_t& addr);  // abs
void txs_imp(Regs& regs);                       // impl
void bpl_rel(Regs& regs, const sbyte& offset);  // rel
void bcs_rel(Regs& regs, const sbyte& offset);  // rel
void bne_rel(Regs& regs, const sbyte& offset);  // rel
void cmp_imm(Regs& regs, const sbyte& val);     // imm
void cpx_abs(Regs& regs, const int16_t& addr);  // abs
void jsr_abs(Regs& regs, const int16_t& addr);  // abs
void jmp_abs(Regs& regs, const int16_t& addr);  // abs
void inc_abs(Regs& regs, const int16_t& addr);  // abs
void iny_imp(Regs& regs);                       // imp
void and_imm(Regs& regs, const sbyte& val):     // imm
void ora_imm(Regs& regs, const sbyte& val);     // imm
void ora_indX(Regs& regs, const sbyte& val);    // (ind,X)
void eor_indX(Regs& regs, const sbyte& val);    // (ind,X)
void pla_imp(Regs& regs);                       // imp
void slo_indX(Regs& regs, const sbyte& val);    // (ind,X)
void nop_3(Regs& regs);
void nop_45(Regs& regs);

/*
 * Set interrupt Disable Status
 */
void sei_imp(Regs& regs) {
    regs.flags[2] = 1;
}

/*
 * Clear Decimal Mode
 */
void cld_imp(Regs& regs) {
    regs.flags[3] = 0;
}

/*
 * Decrement Memory
 * $DE: Absolute,X
 */
void dec_absX(Regs& regs, const int16_t& addr) {
    regs.mem[reg6502[R::X] + addr] += 1;
}

/*
 * Decrement X Register
 * $CA: Implied
 */
void dex_imp(Regs& regs) {
    regs.reg6502[R::X] += 1;
}

/*
 * Load Accumulator
 * $A9: Immediate
 * Load argument value into the ACC register
 */
void lda_imm(Regs& regs, const sbyte& arg) {
    regs.reg6502[R::ACC] = arg;
}
/*
 * $AD: Absolute
 */
void lda_abs(Regs& regs, const int16_t& data) {
    regs.reg6502[R::ACC] = data;
}
/*
 * $BD: Absolute, X
 */
void lda_absX(Regs& regs, const int16_t& addr) {
    regs.reg6502[R::ACC] = regs.mem[addr + regs.reg6502[R::X]];
}

/*
 * Load X Register
 * $A2: Immediate
 */
void ldx_imm(Regs& regs, const sbyte& val) {
    regs.reg6502[R::X] = val;
    if (!val)         regs.flags[F::ZF] = 1;
    else if (val < 0) regs.flags[F::NF] = 1;
}
/*
 * $AE: Absolute
 */
void ldx_abs(Regs& regs, const int16_t& addr) {
    regs.reg6502[R::Y] = regs.mem[addr];
}

/*
 * Load Y Register
 * $A0: Immediate
 */
void ldy_imm(Regs& regs, const sbyte& val) {
    regs.reg6502[R::Y] = val;
    if (!val)         regs.flags[F::ZF] = 1;
    else if (val < 0) regs.flags[F::NF] = 1;
}
/*
 * $AC: Absolute
 */
void ldy_abs(Regs& regs, const uint16_t& addr) {
    regs.reg6502[R::Y] = regs.mem[addr];
}

/*
 * Store Accumulator
 * $8D: Absolute
 */
void sta_abs(Regs& regs, const int16_t& addr) {
    // store content in acc into memory at address
    regs.mem[addr] = regs.reg6502[R::ACC];
}
/*
 * $85: Zero Page
 */
void sta_zp(Regs& regs, const sbyte& val) {
    regs.mem[val] = regs.reg6502[R::ACC];
}

/*
 * Store Y Register
 * $8C: Absolute
 */
void sty_abs(Regs& regs, const int16_t& addr) {
    regs.mem[addr] = regs.reg6502[R::Y];
}

/*
 * Transfer X to Stack Pointer
 * $9A: Implied
 */
void txs_imp(Regs& regs) {
    regs.reg6502[R::SP] = regs.reg6502[R::X];
}

/*
 * Branch if Positive
 * $10: Relative
 * If the negative flag is clear then add the relative displacement to the program counter to cause a branch to a new location.
 */
void bpl_rel(Regs& regs, const sbyte& offset) {
    if (!regs.flags[F::NF])
        regs.PC += offset;
}

/*
 * Branch if Carry Set
 * $B0: Relative
 */
void bcs_rel(Regs& regs, const sbyte& offset) {
    if (regs.flags[F::CF])
        regs.PC += offset;
}

/*
 * Branch if Not Equal
 * $D0: Relative
 */
void bne_rel(Regs& regs, const sbyte& offset) {
    if (!regs.flags[F::ZF]) {
        regs.PC += offset;
    }
}

/*
 * Compare
 * $C9: Immediate
 */
void cmp_imm(Regs& regs, const sbyte& val) {
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
void cpx_abs(Regs& regs, const int16_t& addr) {
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
void jsr_abs(Regs& regs, const int16_t& addr) {
    regs.PC = addr;
}

/*
 * Jump
 * $4C: Absolute
 */
void jmp_abs(Regs& regs, const int16_t& addr) {
    regs.PC = addr;
}

/*
 * Increment Memory
 * $EE: Absolute
 */
void inc_abs(Regs& regs, const int16_t& addr) {
    regs.mem[addr] += 1;
}

/*
 * Increment Y
 * $C8: Implied
 */
void iny_imp(Regs& regs) {
    regs.reg6502[R::Y] += 1;
}

/*
 * Logical AND
 * $29: Immediate
 */
void and_imm(Regs& regs, const sbyte& val) {
    regs.reg6502[R::ACC] &= val;
}

/*
 * Logical Inclusive OR
 * $09: Immediate
 */
void ora_imm(Regs& regs, const sbyte& val) {
    regs.reg6502[R::ACC] |= val;
}

/*
 * Logical Inclusive OR
 * $01: (Indirect, X)
 */
void ora_indX(Regs& regs, const sbyte& val) {
    const int16_t i16_TargetAddr = regs.mem[regs.reg6502[R::X] + val];
    const sbyte b_LoadVal = regs.mem[i16_TargetAddr];
    regs.reg6502[R::ACC] |= b_LoadVal;
}

/*
 * Exclusive-OR
 * $41: (Indirect, X)
 * Updates: Z, N
 */
void eor_indX(Regs& regs, const sbyte& val) {
    const int16_t i16_TargetAddr = regs.mem[regs.reg6502[R::X] + val];
    const sbyte b_LoadVal = regs.mem[i16_TargetAddr];
    regs.reg6502[R::ACC] ^= b_LoadVal;
    if (!regs.reg6502[R::ACC])
        regs.flags[F::ZF] = 1;
    else if (regs.reg6502[R::ACC] >> 7)
        regs.flags[F::NF] = 1;
}

/*
 * Pull Accumulator From Stack (Pop)
 * $68: Implied
 */
void pla_imp(Regs& regs) {
    regs.reg6502[R::ACC] = regs.mem[0x0100 & regs.reg6502[R::SP]];
    regs.reg6502[R::SP] -= 1;
}

/*
 * Equivalent to ASL value then ORA value.
 * $03: Indexed Indirect (Ind, X)
 */
void slo_indX(Regs& regs, const sbyte& val) {
    const ubyte addr = val + regs.reg6502[R::X];
    regs.mem[addr] *= 2;
    regs.mem[addr] |= regs.reg6502[R::ACC];
}

/*
 * No Operation
 * $44: ??? (NES-exclusive)
 * $7C: ??? (NES-exclusive)
 */
void nop_3(Regs& regs) {
    // Waste 3 cycles
}
void nop_45(Regs& regs) {
    // Waste 4 or 5 cycles
}

/*
 * Appendix
 * ----------------------------------------
 *  I. Addressing Modes
 *      1. Implicit/Implied (Imp)
 *          - SEI
 *      2. Accumulator (Acc)
 *          - LSR A
 *      3. Immediate (Imm)
 *          - LDA #10
 *      4. Zero Page (ZP)
 *          - LDA #00
 *      5. Zero Page, X (ZP,X)
 *          - STY $10, X
 *      6. Zero Page, Y (ZP,Y)
 *          - LDX $10, Y
 *      7. Relative (Rel)
 *          - BEQ LABEL
 *      8. Absolute (Abs)
 *          - JMP #1234
 *      9. Absolute, X (Abs,X)
 *          - STA #3000,X
 *      10. Absolute, Y (Abs,Y)
 *          - AND #4000, Y
 *      11. Indirect (Ind)
 *          - JMP (#FFFC)
 *      12. Indexed Indirect (Ind,X)
 *          - LDA (#40,X)
 *      13. Indirect Indexed (Ind,Y)
 *          - LDA ($40),Y
 *
 *      Note: Indexed Indirect and Indirect Indexed instructions will
 *          access addresses in the zeropage range since they take an
 *          8-bit address instead of a 16-bit address.
 */

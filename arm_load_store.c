/*
Armator - simulateur de jeu d'instruction ARMv5T   but p dagogique
Copyright (C) 2011 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique G n rale GNU publi e par la Free Software
Foundation (version 2 ou bien toute autre version ult rieure choisie par vous).

Ce programme est distribu  car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but sp cifique. Reportez-vous   la
Licence Publique G n rale GNU pour plus de d tails.

Vous devez avoir re u une copie de la Licence Publique G n rale GNU en m me
temps que ce programme ; si ce n'est pas le cas,  crivez   la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
 tats-Unis.

Contact: Guillaume.Huard@imag.fr
	 B timent IMAG
	 700 avenue centrale, domaine universitaire
	 38401 Saint Martin d'H res
*/
#include "arm_load_store.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "util.h"
#include "debug.h"
#include <assert.h>

int arm_get_index(arm_core p, uint32_t ins, uint32_t *index) {
    uint8_t shift, rm, shift_imm;

    rm = get_bits (ins, 3, 0);
    shift = get_bits (ins, 6, 5);
    shift_imm = get_bits (ins, 11, 7);

    switch (shift) {
        case LSL:
            *index = arm_read_register (p, rm) << shift_imm;
            break;
        case LSR:
            if (shift_imm == 0) { // LSR #32 {
                *index = 0;
            } else {
                *index = arm_read_register (p, rm) >> shift_imm;
            }
            break;
        case ASR:
            if (shift_imm == 0) { // ASR #32
                if (((rm >> 31) & 1) == 1) {
                    *index = 0xFFFFFFFF;
                } else {
                    *index = 0;
                }
            } else {
                *index = asr(arm_read_register (p, rm), shift_imm);
            }
            break;
        case ROR:
            *index = ror (arm_read_register (p, rm), shift_imm);
            break;
    }
    return 0;
}

int arm_get_address_word_byte (arm_core p, uint32_t ins, uint32_t *address) {
    uint8_t i, pp, u, w, rn, rm;
    uint16_t offset;
    uint32_t index;

    i = get_bit (ins, 25);
    pp = get_bit (ins, 24);
    u = get_bit (ins, 23);
    w = get_bit (ins, 21);
    rn = get_bits (ins, 19, 16);
    rm = get_bits (ins, 3, 0);
    offset = get_bits (ins, 11, 0);

    if (i == 0) { // Immediate offset/index
        if (pp == 0) { // Immediate post-indexed
            *address = arm_read_register (p, rn);
            if (u == 1) {
                arm_write_register (p, rn, arm_read_register (p, rn) + offset);
            } else {
                arm_write_register (p, rn, arm_read_register (p, rn) - offset);
            }
        } else { // Immediate offset or Immediate pre-indexed
            if (w == 0) { // Immediate offset
                if (u == 1) {
                    *address = arm_read_register (p, rn) + offset;
                } else {
                    *address = arm_read_register (p, rn) - offset;
                }
            } else { // Immediate pre-indexed
                if (u == 1) {
                    *address = arm_read_register (p, rn) + offset;
                } else {
                    *address = arm_read_register (p, rn) - offset;
                }
                arm_write_register (p, rn, *address);
            }
        }
    } else { // Register offset/index OU Scaled register offset/index
        if (get_bits (ins, 11, 5) == 0) { // Register offset/index
            if (pp == 0) { // Register post-indexed
                *address = arm_read_register (p, rn);
                if (u == 1) {
                    arm_write_register (p, rn, arm_read_register (p, rn) + arm_read_register (p, rm));
                } else {
                    arm_write_register (p, rn, arm_read_register (p, rn) - arm_read_register (p, rm));
                }
            } else { // Register offset OU Register pre-indexed
                if (w == 0) { // Register offset
                    if (u == 1) {
                        *address = arm_read_register (p, rn) + arm_read_register (p, rm);
                    } else {
                        *address = arm_read_register (p, rn) - arm_read_register (p, rm);
                    }
                } else { // Register pre-indexed
                    if (u == 1) {
                        *address = arm_read_register (p, rn) + arm_read_register (p, rm);
                    } else {
                        *address = arm_read_register (p, rn) - arm_read_register (p, rm);
                    }
                    arm_write_register (p, rn, *address);
                }
            }
        } else { // Scaled register offset/index
            if (pp == 0) { // Scaled register post-indexed
                arm_get_index (p, ins, &index);
                if (u == 1) {
                    arm_write_register (p, rn, arm_read_register (p, rn) + index);
                } else {
                    arm_write_register (p, rn, arm_read_register (p, rn) - index);
                }
            } else { // Scaled register offset OU Scaled register pre-indexed
                if (w == 0) { // Scaled register offset
                    arm_get_index (p, ins, &index);
                    if (u == 1) {
                        *address = arm_read_register (p, rn) + index;
                    } else {
                        *address = arm_read_register (p, rn) - index;
                    }
                } else { // Scaled register pre-indexed
                    arm_get_index (p, ins, &index);
                    if (u == 1) {
                        *address = arm_read_register (p, rn) + index;
                    } else {
                        *address = arm_read_register (p, rn) - index;
                    }
                    arm_write_register (p, rn, *address);
                }
            }
        }
    }
    return 0;
}

int arm_get_address_half (arm_core p, uint32_t ins, uint32_t *address) {
    uint8_t pp, w, offset_8, immedH, immedL, u, rn, rm;

    pp = get_bit (ins, 24);
    w = get_bit (ins, 21);
    immedH = get_bits (ins, 11, 8);
    immedL = get_bits (ins, 3, 0);
    u = get_bit (ins, 23);
    rn = get_bits (ins, 19, 16);
    rm = get_bits (ins, 3, 0);

    if (get_bit(ins, 22) == 1) { //Immediate offset/index
        if (pp == 0) {  // Immediate post-indexed
            *address = arm_read_register (p, rn);
            offset_8 = (immedH << 4) | immedL;
            if (u == 1) {
                arm_write_register (p, rn, arm_read_register (p, rn) + offset_8);
            } else {
                arm_write_register (p, rn, arm_read_register (p, rn) - offset_8);
            }
        } else { // Immediate offset OU Immediate pre-indexed
            if (w == 0) { // Immediate offset
                offset_8 = (immedH << 4) | immedL;
                if (u == 1) {
                    *address = arm_read_register (p, rn) + offset_8;
                } else {
                    *address = arm_read_register (p, rn) - offset_8;
                }
            } else { // Immediate pre-indexed
                offset_8 = (immedH << 4) | immedL;
                if (u == 1) {
                    *address = arm_read_register (p, rn) + offset_8;
                } else {
                    *address = arm_read_register (p, rn) - offset_8;
                }
                arm_write_register (p, rn, *address);
            }
        }

    } else { //Register offset/index
        if (pp == 0) {  // Register post-indexed
            *address = arm_read_register (p, rn);
            if (u == 1) {
                arm_write_register (p, rn, arm_read_register (p, rn) + arm_read_register (p, rm));
            } else {
                arm_write_register (p, rn, arm_read_register (p, rn) - arm_read_register (p, rm));
            }
        } else { // Register offset OU Register pre-indexed
            if (w == 0) { // Register offset
                if (u == 1) {
                    *address = arm_read_register (p, rn) + arm_read_register (p, rm);
                } else {
                    *address = arm_read_register (p, rn) - arm_read_register (p, rm);
                }
            } else { // Register pre-indexed
                if (u == 1) {
                    *address = arm_read_register (p, rn) + arm_read_register (p, rm);
                } else {
                    *address = arm_read_register (p, rn) - arm_read_register (p, rm);
                }
                arm_write_register (p, rn, *address);
            }
        }
    }
    return 0;
}

int arm_ldr (arm_core p, uint32_t ins, uint32_t address) {
    uint32_t data;

    arm_read_word(p, address, &data);
    if (get_bits (ins, 15, 12) == 0x0F) {
        arm_write_register (p, 15, data & 0xFFFFFFFE);
        arm_write_cpsr (p, (arm_read_cpsr(p) & ~(1 << 5)) | ((data & 1) << 5));
    } else {
        arm_write_register (p, get_bits (ins, 15, 12), data);
    }
    return 0;
}

int arm_str(arm_core p, uint32_t ins, uint32_t address) {
    arm_write_word(p, address, arm_read_register(p, get_bits(ins, 15, 12)));
    return 0;
}

int arm_ldrb(arm_core p, uint32_t ins, uint32_t address) {
    uint8_t data;

    arm_read_byte(p, address, &data);
    arm_write_register (p, get_bits (ins, 15, 12), (uint32_t)data);
    return 0;
}

int arm_strb(arm_core p, uint32_t ins, uint32_t address) {
    arm_write_byte(p, address, arm_read_register(p, get_bits(ins, 15, 12)));
    return 0;
}

int arm_ldrh(arm_core p, uint32_t ins, uint32_t address) {
    uint16_t data;

    if ((address & 1) == 0) {
        arm_read_half (p, address, &data);
    } else {
        return DATA_ABORT;
    }
    arm_write_register (p, get_bits (ins, 15, 12), (uint32_t)data);
    return 0;
}

int arm_strh(arm_core p, uint32_t ins, uint32_t address) {
    if ((address & 1) == 0) {
        arm_write_half (p, address, arm_read_register(p, get_bits(ins, 15, 12)));
    } else {
        return DATA_ABORT;
    }
    return 0;
}

int arm_load_store(arm_core p, uint32_t ins) {

    uint8_t opcode, l, b;
    uint32_t address;

    opcode = get_bit(ins, 26); //bit qui indique si c'est un mot de 32/8 bits ou 16 bits
    l = get_bit(ins, 20); //L
    b = get_bit(ins, 22); //B

    switch (opcode) {
        case 0x01: //32 ou 8 bits
            arm_get_address_word_byte(p, ins, &address);
            if (b == 0x00) { //32 bits
                if (l == 0x01) { //load
                    return arm_ldr(p, ins, address);
                } else { //store
                    return arm_str(p, ins, address); //TEST
                }
            } else { //8 bits
                if (l == 0x01) { //load
                    return arm_ldrb(p, ins, address);
                } else { //store
                    return arm_strb(p, ins, address); //TEST
                }
            }
            break;

        case 0x00: //16 bits
            arm_get_address_half(p, ins, &address);
            if (l == 0x01) { //load
                return arm_ldrh(p, ins, address);
            } else { //store
                return arm_strh(p, ins, address); //TEST
            }
            break;

        default:
            return UNDEFINED_INSTRUCTION;
    }

    return 0;
}

int count_nb_set(uint16_t register_list) { // Compter le nombre de 1 dans un nombre en binaire
    int count = 0;

    while (register_list != 0) {
        count =  count + (register_list & 1);
        register_list = register_list >> 1;
    }

    return count;
}

int arm_get_start_end_address(arm_core p, uint32_t ins, uint32_t *start_address, uint32_t *end_address) {
    uint8_t opcode, rn, w;
    uint16_t register_list;

    opcode = get_bits (ins, 24, 23);
    register_list = get_bits (ins, 15, 0);
    w = get_bit (ins, 21);
    rn = get_bits (ins, 19, 16);

    switch (opcode) {
        case 0x00: // Decrement after
            *start_address = arm_read_register(p, rn) - (count_nb_set(register_list) * 4) + 4;
            *end_address = arm_read_register(p, rn);
            if (w == 1) {
                arm_write_register (p, rn, arm_read_register(p, rn) - (count_nb_set(register_list) * 4) );
            }
            break;
        case 0x01: // Increment after
            *start_address = arm_read_register(p, rn);
            *end_address = arm_read_register(p, rn) + (count_nb_set(register_list) * 4) - 4;
            if (w == 1) {
                arm_write_register (p, rn, arm_read_register(p, rn) + (count_nb_set(register_list) * 4) );
            }
            break;
        case 0x02: // Decrement before
            *start_address = arm_read_register(p, rn) - (count_nb_set(register_list) * 4);
            *end_address = arm_read_register(p, rn) - 4;
            if (w == 1) {
                arm_write_register (p, rn, arm_read_register(p, rn) - (count_nb_set(register_list) * 4) );
            }
            break;
        case 0x03: // Increment before
            *start_address = arm_read_register(p, rn) + 4;
            *end_address = arm_read_register(p, rn) + (count_nb_set(register_list) * 4);
            if (w == 1) {
                arm_write_register (p, rn, arm_read_register(p, rn) + (count_nb_set(register_list) * 4) );
            }
            break;
    }
    return 0;
}

int arm_ldm(arm_core p, uint32_t ins, uint32_t start_address, uint32_t end_address) {
    uint32_t address, ri, value;
    uint16_t register_list;

    register_list = get_bits (ins, 15, 0);
    address = start_address;

    for (int i = 0; i <= 14; i++) {
        if ( ((register_list >> i) & 1) == 1 ) {
            arm_read_word(p, address, &ri);
            arm_write_register (p, i, ri);
            address = address + 4;
        }
    }
    if ( ((register_list >> 15) & 1) == 1 ) {
        arm_read_word(p, address, &value);
        arm_write_register (p, 15, value & 0xFFFFFFFE);
        arm_write_cpsr (p, (arm_read_cpsr(p) & ~(1 << 5)) | ((value & 1) << 5));
        address = address + 4;
    }
    assert (end_address == address - 4);
    return 0;
}

int arm_stm(arm_core p, uint32_t ins, uint32_t start_address, uint32_t end_address) {
    uint32_t address;
    uint16_t register_list;

    register_list = get_bits (ins, 15, 0);
    address = start_address;

    for (int i = 0; i <= 15; i++) {
        if ( ((register_list >> i) & 1) == 1 ) {
            arm_write_word(p, address, arm_read_register(p, i));
            address = address + 4;
        }
    }
    assert (end_address == address - 4);
    return 0;
}

int arm_load_store_multiple(arm_core p, uint32_t ins) {
    uint8_t l;
    uint32_t start_address, end_address;

    l = get_bit (ins, 20); //L
    arm_get_start_end_address (p, ins, &start_address, &end_address);

    switch (l) {
        case 0x01: //load multiple
            return arm_ldm (p, ins, start_address, end_address); //TEST
        case 0x00: //store multiple
            return arm_stm (p, ins, start_address, end_address); //TEST
        default:
            return UNDEFINED_INSTRUCTION;
    }
}

int arm_coprocessor_load_store(arm_core p, uint32_t ins) {
    /* Not implemented */
    return UNDEFINED_INSTRUCTION;
}

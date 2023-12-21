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

int arm_load_store(arm_core p, uint32_t ins) {

    uint8_t l, b, u, opcode, data8;
    uint16_t data16;
    int rotation_bits;
    uint32_t offset, rn, add_rn, data32;

    l = get_bit(ins, 20); //L
    opcode = get_bit(ins, 26); //bit qui indique si c'est un mot de 32/8 bits ou 16 bits
    
    offset = get_bits(ins, 11, 0); //addressing_mode_specific
    rn = get_bits(ins, 19, 16);
    add_rn = arm_read_register(p, rn);

    switch (opcode) { 
        case 0x01: //32 ou 8 bits
            b = get_bit(ins, 22); //B
            if (b == 0x00) { //32 bits
                if (l == 0x01) { //load
                    u = get_bit(ins, 23); //U
                    if (u == 0) { //CP15_reg1_Ubit == 0
                        arm_read_word(p, offset+add_rn, &data32);
                        rotation_bits = 8 * ((offset+add_rn) % 4);
                        data32 = (data32 >> rotation_bits) | (data32 << (32 - rotation_bits));
                    } else {
                        arm_read_word(p, offset+add_rn, &data32);
                    }
                    if (get_bits(ins, 15, 12) == 0x0F) {
                        arm_write_register(p, 15, data32 & 0xFFFFFFFE);
                        arm_write_cpsr(p, data32 & 0x1);
                    } else {
                        arm_write_register(p, get_bits(ins, 15, 12), data32);
                    }
                } else { //store
                    return 0; //TEST
                }
            } else { //8 bits
                if (l == 0x01) { //load
                    arm_read_byte(p, offset+add_rn, &data8);
                    arm_write_register(p, get_bits(ins, 15, 12), data8);
                } else { //store
                    return 0; //TEST
                }

            }
            break;
        case 0X00: //16 bits
            if (l == 0x01) { //load
                u = get_bit(ins, 23); //U
                if (u == 0) { //CP15_reg1_Ubit == 0
                    if ((offset & 0x01) == 0) {
                        arm_read_half(p, offset+add_rn, &data16);
                    } else {
                        return -1;
                    }
                } else {
                    arm_read_half(p, offset, &data16);
                }
                arm_write_register(p, get_bits(ins, 15, 12), data16); 

                } else { //store
                    return 0; //TEST
                }

            break;

        default:
            return UNDEFINED_INSTRUCTION;
    }

    return 0;
}

int arm_load_store_multiple(arm_core p, uint32_t ins) {
    return UNDEFINED_INSTRUCTION;
}

int arm_coprocessor_load_store(arm_core p, uint32_t ins) {
    /* Not implemented */
    return UNDEFINED_INSTRUCTION;
}

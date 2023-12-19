/*
Armator - simulateur de jeu d'instruction ARMv5T � but p�dagogique
Copyright (C) 2011 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique G�n�rale GNU publi�e par la Free Software
Foundation (version 2 ou bien toute autre version ult�rieure choisie par vous).

Ce programme est distribu� car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but sp�cifique. Reportez-vous � la
Licence Publique G�n�rale GNU pour plus de d�tails.

Vous devez avoir re�u une copie de la Licence Publique G�n�rale GNU en m�me
temps que ce programme ; si ce n'est pas le cas, �crivez � la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
�tats-Unis.

Contact: Guillaume.Huard@imag.fr
	 B�timent IMAG
	 700 avenue centrale, domaine universitaire
	 38401 Saint Martin d'H�res
*/
#include "arm_data_processing.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "arm_branch_other.h"
#include "util.h"
#include "debug.h"

/* Decoding functions for different classes of instructions */
int arm_data_processing_shift(arm_core p, uint32_t ins) {
    uint8_t opcode,r1,r2,r3;
    //int result;

    opcode = get_bits(ins, 24, 21);

    r1 = arm_read_register(p, get_bits(ins, 19, 16));
    r2 = arm_read_register(p, get_bits(ins, 15, 12));
    r3 = arm_read_register(p, get_bits(ins, 4, 0));


    switch(opcode){
        case AND:
            r2 = r1 & r3;
			arm_write_register(p, get_bits(ins, 15, 12), (uint32_t)r2);
            break;
        case EOR:
            r2 = r1 ^ r3;
            arm_write_register(p, get_bits(ins, 15, 12), (uint32_t)r2);
            break;
        case SUB:
            r2 = r1 - r3;
            arm_write_register(p, get_bits(ins, 15, 12), (uint32_t)r2);
            break;
        case RSB:
            r2 = r3 - r1;
            arm_write_register(p, get_bits(ins, 15, 12), (uint32_t)r2);
            break;
        case ADD:
            r2 = r1 + r3;
            arm_write_register(p, get_bits(ins, 15, 12), (uint32_t)r2);
            break;
        case ADC:
            r2 = r1 + r3 /*+ FLAG C*/;
            arm_write_register(p, get_bits(ins, 15, 12), (uint32_t)r2);
            break;
        case SBC:
            r2 = r1 - r3 /*- NOT(FLAG C)*/;
            arm_write_register(p, get_bits(ins, 15, 12), (uint32_t)r2);
            break;
        case RSC:
            r2 = r3 - r1 /*- NOT(FLAG C)*/;
            arm_write_register(p, get_bits(ins, 15, 12), (uint32_t)r2);
            break;
        case TST: 
            r2 = r1 & r3;
            //update_flags(r2);
            break;
        case TEQ:
            r2 = r1 ^ r3;
            //update_flags(r2);
            break;
        case CMP:
            r2 = r1 - r3;
            //update_flags(r2);
            break;
        case CMN:
            r2 = r1 + r3;
            //update_flags(r2);
            break;
        case ORR:
            r2 = r1 | r3;
            arm_write_register(p, get_bits(ins, 15, 12), (uint32_t)r2);
            break;
        case MOV:
            r2 = r3;
            arm_write_register(p, get_bits(ins, 15, 12), (uint32_t)r2);
            break;
        case BIC:
            r2 = r1 & ~(r3);
            arm_write_register(p, get_bits(ins, 15, 12), (uint32_t)r2);
            break;
        case MVN:
            r2 = ~(r3);
            arm_write_register(p, get_bits(ins, 15, 12), (uint32_t)r2);
            break;
        default:
            return -1;
    }
    return 0;
}

int arm_data_processing_immediate_msr(arm_core p, uint32_t ins) {
uint8_t opcode,r1,r2,v1;
    //int result;

    opcode = get_bits(ins, 24, 21);

    r1 = arm_read_register(p, get_bits(ins, 19, 16));
    r2 = arm_read_register(p, get_bits(ins, 15, 12));
    v1 = get_bits(ins, 7, 0);


    switch(opcode){
        case AND:
            r2 = r1 & v1;
			arm_write_register(p, get_bits(ins, 15, 12), (uint32_t)r2);
            break;
        case EOR:
            r2 = r1 ^ v1;
            arm_write_register(p, get_bits(ins, 15, 12), (uint32_t)r2);
            break;
        case SUB:
            r2 = r1 - v1;
            arm_write_register(p, get_bits(ins, 15, 12), (uint32_t)r2);
            break;
        case RSB:
            r2 = v1 - r1;
            arm_write_register(p, get_bits(ins, 15, 12), (uint32_t)r2);
            break;
        case ADD:
            r2 = r1 + v1;
            arm_write_register(p, get_bits(ins, 15, 12), (uint32_t)r2);
            break;
        case ADC:
            r2 = r1 + v1 /*+ FLAG C*/;
            arm_write_register(p, get_bits(ins, 15, 12), (uint32_t)r2);
            break;
        case SBC:
            r2 = r1 - v1 /*- NOT(FLAG C)*/;
            arm_write_register(p, get_bits(ins, 15, 12), (uint32_t)r2);
            break;
        case RSC:
            r2 = v1 - r1 /*- NOT(FLAG C)*/;
            arm_write_register(p, get_bits(ins, 15, 12), (uint32_t)r2);
            break;
        case TST: 
            r2 = r1 & v1;
            //update_flags(r2);
            break;
        case TEQ:
            r2 = r1 ^ v1;
            //update_flags(r2);
            break;
        case CMP:
            r2 = r1 - v1;
            //update_flags(r2);
            break;
        case CMN:
            r2 = r1 + v1;
            //update_flags(r2);
            break;
        case ORR:
            r2 = r1 | v1;
            arm_write_register(p, get_bits(ins, 15, 12), (uint32_t)r2);
            break;
        case MOV:
            r2 = v1;
            arm_write_register(p, get_bits(ins, 15, 12), (uint32_t)r2);
            break;
        case BIC:
            r2 = r1 & ~(v1);
            arm_write_register(p, get_bits(ins, 15, 12), (uint32_t)r2);
            break;
        case MVN:
            r2 = ~(v1);
            arm_write_register(p, get_bits(ins, 15, 12), (uint32_t)r2);
            break;
        default:
            return -1;
    }
    return 0;
}

int add(uint32_t ins){
    int s = get_bit(ins, 20);


    if (s){
        //update_flags();
    }
	return -1;
}

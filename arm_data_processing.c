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
    uint8_t opcode;
    uint32_t r1,r2;

    opcode = get_bits(ins, 24, 21);

    r1 = arm_read_register(p, get_bits(ins, 19, 16));
    r2 = arm_read_register(p, get_bits(ins, 3, 0));

    return arm_data_processing_operation(1, p, ins, opcode, r1, r2);
}

int arm_data_processing_immediate_msr(arm_core p, uint32_t ins) {
    uint8_t opcode,v1;
    uint32_t r1;

    opcode = get_bits(ins, 24, 21);

    r1 = arm_read_register(p, get_bits(ins, 19, 16));
    v1 = get_bits(ins, 7, 0);

    return arm_data_processing_operation(0, p, ins, opcode, r1, v1);
}

int arm_data_processing_operation(int shift, arm_core p, uint32_t ins, uint8_t opcode, uint32_t p1, uint8_t p2) {
    long res;
    uint8_t bit_id, bit_r;

    if(shift){
        bit_id = get_bit(ins, 20);
        bit_r = get_bit(ins, 22);
    }

    switch(opcode){
        case AND:
            res = p1 & p2;
            break;
        case EOR:
            res = p1 ^ p2;
            break;
        case SUB:
            res = p1 - p2;
            break;
        case RSB:
            res = p2 - p1;
            break;
        case ADD:
            res = p1 + p2;
            break;
        case ADC:
            res = p1 + p2 + get_bit(arm_read_cpsr(p), C);
            break;
        case SBC:
            res = p1 - p2 - ~get_bit(arm_read_cpsr(p), C);
            break;
        case RSC:
            res = p2 - p1 - ~get_bit(arm_read_cpsr(p), C);
            break;
        case TST_MRS:
            if(shift && bit_id == 0x01){ // MRS
                res = mrs_instruction(p, bit_r);
            } else {
                res = p1 & p2;
            }
            break;
        case TEQ:
            res = p1 ^ p2;
            break;
        case CMP_MRS:
            if(shift && bit_id == 0x01){ // MRS
                res = mrs_instruction(p, bit_r);
            } else {
                res = p1 - p2;
            }
            break;
        case CMN_MISC:
            if(shift && bit_id == 0x01){ // miscellaneous instruction
                return arm_miscellaneous(p, ins);
            } else {
                res = p1 + p2;
            }
            break;
        case ORR:
            res = p1 | p2;
            break;
        case MOV:
            res = p2;
            break;
        case BIC:
            res = p1 & ~(p2);
            break;
        case MVN:
            res = ~(p2);
            break;
        default:
            return -1;
    }
    arm_write_register(p, get_bits(ins, 15, 12), (uint32_t)res);
    if (get_bit(ins, 20) == 1){
        update_flags(p, res);
    }
    return 0;
}

void update_flags(arm_core p, long res){

    uint32_t cpsr = arm_read_cpsr(p);

    if((uint32_t)res == 0){
        cpsr = set_bit(cpsr, Z);
    }
    if(get_bit(res, 31) == 1){
        cpsr = set_bit(cpsr, N);
    }
    if(get_bit(res, 32) == 1){
        cpsr = set_bit(cpsr, C);
    }
    if((res < 0 && get_bit(res, 31) == 0) || (res > 0 && get_bit(res,31) == 1)){
        cpsr = set_bit(cpsr, V);
    }
    arm_write_cpsr(p, cpsr);
}

long mrs_instruction(arm_core p, uint8_t bit_r){
    long res;

    if(bit_r == 0x01){
        res = arm_read_spsr(p);
    } else {
        res = arm_read_cpsr(p);
    }
    return res;
}
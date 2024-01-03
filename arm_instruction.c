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
#include "arm_instruction.h"
#include "arm_exception.h"
#include "arm_data_processing.h"
#include "arm_load_store.h"
#include "arm_branch_other.h"
#include "arm_constants.h"
#include "util.h"

static int arm_execute_instruction(arm_core p) {

    uint32_t ins;
    int result;
    int exec;

    result = arm_fetch(p, &ins);
    if(result != 0){
        return result;
    }
    exec = check_flags(p, ins);
    if(exec){
        result = switch_type(p, ins);
    } else {
        result = 0;
    }

    return result;
}

int arm_step(arm_core p) {
    int result;

    result = arm_execute_instruction(p);
    if (result) {
        return arm_exception(p, result);
    }
    return result;
}

int switch_type(arm_core p, uint32_t ins){
    int result;
    uint8_t type = get_bits(ins, 27, 25);

    switch (type){
        case 0x00: //INSTR de type add r4, r3
            result = arm_data_processing_shift(p, ins);
            break;
        case 0x01: //INSTR de type add r4, #5
            result = arm_data_processing_immediate_msr(p, ins);
            break;
        case 0x02: //INSTR de type ldr r4, #5
        case 0x03: //INSTR de type ldr r4, [r5]
            result = arm_load_store(p, ins);
            break;
        case 0x04: //INSTR de type ldr r4, [r2,r3]
            result = arm_load_store_multiple(p, ins);
            break;
        case 0x05: //INSTR de type BL etiq
            result = arm_branch(p, ins);
            break;
        case 0x06:
        case 0x07:  //INSTR de type swi
            result = arm_coprocessor_others_swi(p, ins);
            break;
        default: 
            result = -1; 
            break; 
    }
    return result;
}

int check_flags(arm_core p, uint32_t ins){
    uint8_t cond = get_bits(ins, 31, 28);
    uint32_t cpsr = arm_read_cpsr(p);
    uint8_t bit_z = get_bit(cpsr, Z);
    uint8_t bit_c = get_bit(cpsr, C);
    uint8_t bit_n = get_bit(cpsr, N);
    uint8_t bit_v = get_bit(cpsr, V);
    int result;

    switch (cond) {
        case EQ:
            result = bit_z == 0x01;
            break;
        case NE:
            result = bit_z == 0x00;
            break;
        case CS_HS:
            result = bit_c == 0x01;
            break;
        case CC_LO:
            result = bit_c == 0x00;
            break;
        case MI:
            result = bit_n == 0x01;
            break;
        case PL:
            result = bit_n == 0x00;
            break;
        case VS:
            result = bit_v == 0x01;
            break;
        case VC:
            result = bit_v == 0x00;
            break;
        case HI:
            result = bit_c == 0x01 && bit_z == 0x00;
            break;
        case LS:
            result = bit_c == 0x00 || bit_z == 0x01;
            break;
        case GE:
            result = bit_n == bit_v;
            break;
        case LT:
            result = bit_n != bit_v;
            break;
        case GT:
            result = bit_z == 0x00 && (bit_n == bit_v);
            break;
        case LE:
            result = bit_z == 0x01 || (bit_n != bit_v);
            break;
        case AL:
            result = 1;
            break;
        default:
            result = -1;
            break;
    }
    return result;
}
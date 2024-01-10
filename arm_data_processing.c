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

void arm_shifter_op_data(arm_core p, uint32_t ins, uint32_t *index) {
    uint8_t shift, rm, mode, rs, shift_imm;

    rm = get_bits(ins, 3, 0);
    mode = get_bit(ins, 4); //immediate or register
    shift = get_bits(ins, 6, 5); //shift opcode
    
    if(mode){
        rs = get_bits(ins, 11, 8);
    }
    else{
        shift_imm = get_bits (ins, 11, 7); 
    }

    switch (shift) {
        case LSL:
            if(mode){
                *index = arm_read_register (p, rm) << arm_read_register(p, rs);
            }
            else{
                *index = arm_read_register (p, rm) << shift_imm;
            }
            break;
        case LSR:
            if(mode){
                *index = arm_read_register (p, rm) >> arm_read_register(p, rs);
            }
            else{
                *index = arm_read_register (p, rm) >> shift_imm;
            }
            break;
        case ASR:
            if(mode){
                *index = asr(arm_read_register (p, rm), (uint8_t)arm_read_register(p, rs));
            }
            else{
                *index = asr(arm_read_register (p, rm), shift_imm);
            }
            break;
        case ROR:
            if(mode){
                *index = ror(arm_read_register (p, rm), (uint8_t)arm_read_register(p, rs));
            }
            else{
                *index = ror(arm_read_register (p, rm), shift_imm);
            }
            break;
    }
}

/* Decoding functions for different classes of instructions */
int arm_data_processing_shift(arm_core p, uint32_t ins) {
    uint8_t opcode;
    uint32_t index, rn;

    opcode = get_bits(ins, 24, 21);

    rn = arm_read_register(p, get_bits(ins, 19, 16));

    arm_shifter_op_data(p, ins, &index); //shifter operand calc

    return arm_data_processing_operation(1, p, ins, opcode, rn, index);
}

int arm_data_processing_immediate_msr(arm_core p, uint32_t ins) {
    uint8_t opcode, immed_8;
    uint32_t rn;

    opcode = get_bits(ins, 24, 21);

    rn = arm_read_register(p, get_bits(ins, 19, 16));
    immed_8 = get_bits(ins, 7, 0);

    return arm_data_processing_operation(0, p, ins, opcode, rn, immed_8);
}

int arm_data_processing_operation(int shift, arm_core p, uint32_t ins, uint8_t opcode, uint32_t rn, uint32_t index) {
    uint32_t res;
    uint8_t bit_id, bit_r, rd = get_bits(ins, 15, 12);

    if(shift){
        bit_id = get_bit(ins, 20);
        bit_r = get_bit(ins, 22);
    }

    switch(opcode){
        case AND:
            res = rn & index;
            break;
        case EOR:
            res = rn ^ index;
            break;
        case SUB:
            res = rn - index;
            break;
        case RSB:
            res = index - rn;
            break;
        case ADD:
            res = rn + index;
            break;
        case ADC:
            res = rn + index + get_bit(arm_read_cpsr(p), C);
            break;
        case SBC:
            res = rn - index - ~get_bit(arm_read_cpsr(p), C);
            break;
        case RSC:
            res = index - rn - ~get_bit(arm_read_cpsr(p), C);
            break;
        case TST_MRS:
            if(shift && bit_id == 0x00){ // MRS
                res = mrs_instruction(p, bit_r);
            } else { //TST
                res = rn & index;
                update_flags(p, res, rn, index, opcode);
                return 0;
            }
            break;
        case TEQ:
            res = rn ^ index;
            break;
        case CMP_MRS:
            if(shift && bit_id == 0x00){ // MRS
                res = mrs_instruction(p, bit_r);
            } else { //CMP
                res = rn - index;
                update_flags(p, res, rn, index, opcode);
                return 0;
            }
            break;
        case CMN_MISC:
            if(shift && bit_id == 0x00){ // miscellaneous instruction
                return arm_miscellaneous(p, ins); //CLZ
            } else {
                res = rn + index;
                update_flags(p, res, rn, index, opcode);
                return 0;
            }
            break;
        case ORR:
            res = rn | index;
            break;
        case MOV:
            res = index;
            break;
        case BIC:
            res = rn & ~(index);
            break;
        case MVN:
            res = ~(index);
            break;
        default:
            return UNDEFINED_INSTRUCTION;
    }
    arm_write_register(p, rd, res);
    if (get_bit(ins, 20) == 0x01){
        if (rd == 0x0F){ // rd = 15
            if(arm_current_mode_has_spsr(p)){
                arm_write_cpsr(p, arm_read_spsr(p));
            } else {
                // Unpredictable
                return DATA_ABORT;
            }
        } else {
            update_flags(p, res, rn, index, opcode);
        }
    }
    return 0;
}

void update_flags(arm_core p, uint32_t res, uint32_t rn, uint32_t index, uint8_t opcode){

    uint32_t cpsr = arm_read_cpsr(p);

    if(res == 0x00000000){
        cpsr = set_bit(cpsr, Z);
    } else {
        cpsr = clr_bit(cpsr, Z);
    }
    if(get_bit(res, 31) == 0x01){
        cpsr = set_bit(cpsr, N);
    } else {
        cpsr = clr_bit(cpsr, N);
    } 

    if(opcode == ADD || opcode == ADC){
        if(res < rn || res < index){ // ADD ou ADC
            cpsr = set_bit(cpsr, C);
        }
        else {
            cpsr = clr_bit(cpsr, C);
        }
    } else if(opcode == SUB || opcode == SBC){
        if(res > rn){ // SUB ou SUB
            cpsr = set_bit(cpsr, C);
        } else {
            cpsr = clr_bit(cpsr, C);
        }
    }

    cpsr = clr_bit(cpsr, V); // on est en non signé donc pas de V, on le force donc à 0
    arm_write_cpsr(p, cpsr);
}

uint32_t mrs_instruction(arm_core p, uint8_t bit_r){
    uint32_t res;

    if(bit_r == 0x01){
        res = arm_read_spsr(p);
    } else {
        res = arm_read_cpsr(p);
    }
    return res;
}
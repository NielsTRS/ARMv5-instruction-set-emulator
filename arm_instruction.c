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
    uint32_t *pc;
    int result;
    int instr;
    uint8_t br;

    result = arm_fetch(p, pc);
    br = get_bits(*pc, 28, 4);
    
    if(br == 0x0D){ //cond = 1110(ALWAYS)
        instr = switch_type(p, pc);
    }
    else if(br == 0x0F){ //cond = 1111 (cas chelou)
        result = arm_miscellaneous(p, pc);
    }
    else{
        result = arm_branch(p, pc);
    }

    

    

    return result;
}

int switch_type(arm_core p, uint32_t* pc){
    int result;
    uint8_t type;
    type = get_bits(*pc, 25, 3);

    switch (type){
        case 0x00: //INSTR de type add r4, r3
            result = arm_data_processing_shift(p, pc);
            break;
        case 0x01: //INSTR de type add r4, #5
            result = arm_data_processing_immediate_msr(p, pc);
            break;
        case 0x02: //INSTR de type ldr r4, [r2, #5]
            result = arm_load_store(p, pc);
            break;
        case 0x03: //INSTR de type ldr r4, [r2,r3]
            result = arm_load_store_multiple(p, pc);
            break;
        case 0x05: //INSTR de type BL etiq
            result = arm_branch(p, pc);
            break;
        default: 
            result = -1; 
            break; 
    }
}

int arm_step(arm_core p) {
    int result;

    result = arm_execute_instruction(p);
    if (result) {
        return arm_exception(p, result);
    }
    return result;
}

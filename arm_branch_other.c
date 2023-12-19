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
#include "arm_branch_other.h"
#include "arm_constants.h"
#include "util.h"
#include <debug.h>
#include <stdlib.h>

int arm_branch(arm_core p, uint32_t ins) {
    uint8_t cond = get_bits(ins, 31, 28);
    uint8_t l_bit = get_bit(ins, 24);
    uint8_t a_bit = get_bit(ins, 23);
    uint32_t pc = arm_read_register(p, 15);
    uint32_t address;
    int result;

    switch (cond) {
        case EQ:
            result = get_bit(arm_read_cpsr(p), Z) == 1;
            break;
        case NE:
            result = get_bit(arm_read_cpsr(p), Z) == 0;
            result = 0; //test vu qu'il y a pas de mises a jours des flags pour le moment
            break;
        case CS_HS:
            result = get_bit(arm_read_cpsr(p), C) == 1;
            break;
        case CC_LO:
            result = get_bit(arm_read_cpsr(p), C) == 0;
            break;
        case MI:
            result = get_bit(arm_read_cpsr(p), N) == 1;
            break;
        case PL:
            result = get_bit(arm_read_cpsr(p), N) == 0;
            break;
        case VS:
            result = get_bit(arm_read_cpsr(p), V) == 1;
            break;
        case VC:
            result = get_bit(arm_read_cpsr(p), V) == 0;
            break;
        case HI:
            result = get_bit(arm_read_cpsr(p), C) == 1 && get_bit(arm_read_cpsr(p), Z) == 0;
            break;
        case LS:
            result = get_bit(arm_read_cpsr(p), C) == 0 || get_bit(arm_read_cpsr(p), Z) == 1;
            break;
        case GE:
            result = get_bit(arm_read_cpsr(p), N) == get_bit(arm_read_cpsr(p), V);
            break;
        case LT:
            result = get_bit(arm_read_cpsr(p), N) != get_bit(arm_read_cpsr(p), V);
            break;
        case GT:
            result = get_bit(arm_read_cpsr(p), Z) == 0 && (get_bit(arm_read_cpsr(p), N) == get_bit(arm_read_cpsr(p), V));
            break;
        case LE:
            result = get_bit(arm_read_cpsr(p), Z) == 1 || (get_bit(arm_read_cpsr(p), N) != get_bit(arm_read_cpsr(p), V));
            break;
        case AL:
            result = 1;
            break;
        default:
            result = -1;
            break;
    }

    if (result == 1){ // Condition vrai
        address = get_bits(ins, 23, 0);
        if(a_bit == 0x01){ // bit 23 à 1 (nombre négatif)
            // insérer 6 bits vallant 1 gauche
            address |= 0x3F000000;
        }
        address <<= 2;
        address += pc;

        if(l_bit == 0x01){ // L = 1
            arm_write_register(p, 14, pc - 4); // R14 / LR
        }
        arm_write_register(p, 15, address); // R15 / PC
        return 0;
    } else if (result == 0){ // Condition Fausse
        arm_write_register(p, 15, pc-4);
        return 0;
    } else {
        return UNDEFINED_INSTRUCTION;
    }
}

int arm_coprocessor_others_swi(arm_core p, uint32_t ins) {
    if (get_bit(ins, 24)) {
        return SOFTWARE_INTERRUPT;
    }
    return UNDEFINED_INSTRUCTION;
}

int arm_miscellaneous(arm_core p, uint32_t ins) {
    return UNDEFINED_INSTRUCTION;
}

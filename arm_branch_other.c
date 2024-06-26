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
    uint8_t l_bit = get_bit(ins, 24);
    uint8_t a_bit = get_bit(ins, 23);
    uint32_t pc = arm_read_register(p, 15);
    uint32_t address = get_bits(ins, 23, 0);

    if(l_bit == 0x01){ // L = 1
        arm_write_register(p, 14, pc - 4); // R14 / LR
    }

    if(a_bit == 0x01){ // bit 23 à 1 (nombre négatif)
        // insérer 6 bits vallant 1 gauche
        address |= 0x3F000000;
    }
    address <<= 2;

    arm_write_register(p, 15, pc + address); // R15 / PC
    return 0;
}

int arm_coprocessor_others_swi(arm_core p, uint32_t ins) {
    if (get_bit(ins, 24)) {
        return SOFTWARE_INTERRUPT;
    }
    return UNDEFINED_INSTRUCTION;
}

int arm_miscellaneous(arm_core p, uint32_t ins) {
    // CLZ instruction
    uint8_t rd = get_bits(ins, 15, 12);
    uint8_t rm = get_bits(ins, 7, 4);
    int position = 0;

    if(rm == 0x00){
        arm_write_register(p, rd, 32);
    } else {
        for(int i = 0; i < 32; i++){
            if(get_bit(arm_read_register(p, rm), i) == 1){
                position = i;
            }
        }
        arm_write_register(p, rd, 31 - position);
    }
    return 0;
}

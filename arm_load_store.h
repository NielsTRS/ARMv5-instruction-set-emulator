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
#ifndef __ARM_LOAD_STORE_H__
#define __ARM_LOAD_STORE_H__
#include <stdint.h>
#include "arm_core.h"

int arm_load_store(arm_core p, uint32_t ins);
int arm_load_store_multiple(arm_core p, uint32_t ins);
int arm_coprocessor_load_store(arm_core p, uint32_t ins);

int arm_get_index (arm_core p, uint32_t ins, uint32_t *index);
int arm_get_address_word_byte (arm_core p, uint32_t ins, uint32_t *address);
int arm_get_address_half (arm_core p, uint32_t ins, uint32_t *address);
int arm_ldr(arm_core p, uint32_t ins, uint32_t adress);
int arm_str(arm_core p, uint32_t ins, uint32_t adress);
int arm_ldrb(arm_core p, uint32_t ins, uint32_t adress);
int arm_strb(arm_core p, uint32_t ins, uint32_t adress);
int arm_ldrh(arm_core p, uint32_t ins, uint32_t adress);
int arm_strh(arm_core p, uint32_t ins, uint32_t adress);

int count_nb_set(uint16_t register_list);
int arm_get_start_end_address(arm_core p, uint32_t ins, uint32_t *start_address, uint32_t *end_address);
int arm_ldm(arm_core p, uint32_t ins, uint32_t start_address, uint32_t end_address);
int arm_stm(arm_core p, uint32_t ins, uint32_t start_address, uint32_t end_address);

#endif

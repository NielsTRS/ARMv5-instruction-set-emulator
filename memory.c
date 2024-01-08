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
#include <stdlib.h>
#include "memory.h"
#include "util.h"
#include "arm_constants.h"

struct memory_data {
    size_t size;
    uint8_t *data;
};

memory memory_create(size_t size) {
    memory mem = malloc(sizeof(struct memory_data));
    mem->size = size;
    mem->data = calloc(size,sizeof(uint8_t));
    if(mem->data == NULL){
        free(mem);
        return  NULL;
    }
    return mem;
}

size_t memory_get_size(memory mem) {
    return mem->size;
}

void memory_destroy(memory mem) {
    free(mem->data);
    free(mem);
}

int memory_read_byte(memory mem, uint32_t address, uint8_t *value) {
    if(mem != NULL && address < mem->size){
        *value = mem->data[address];
        return 0;
    }
    return DATA_ABORT;
}

int memory_read_half(memory mem, uint32_t address, uint16_t *value, uint8_t be) {
    if(mem != NULL && address < mem->size - 1){
        *value = (uint16_t)mem->data[address] | (uint16_t)mem->data[address + 1] << 8;
        if (be) {
            *value = reverse_2(*value);
        }
        return 0;
    }
    return DATA_ABORT;
}

int memory_read_word(memory mem, uint32_t address, uint32_t *value, uint8_t be) {
    if(mem != NULL && address < mem->size - 3){
        *value = *(mem->data + address) | *(mem->data + address + 1) << 8 | *(mem->data + address + 2) << 16 | *(mem->data + address + 3) << 24;
        if (be) {
            *value = reverse_4(*value);
        }
        return 0;
    }
    return DATA_ABORT;
}

int memory_write_byte(memory mem, uint32_t address, uint8_t value) {
    if(mem != NULL && address < mem->size){
        *(mem->data + address) = value;
        return 0;
    }
    return DATA_ABORT;
}

int memory_write_half(memory mem, uint32_t address, uint16_t value, uint8_t be) {
    if(mem != NULL && address < mem->size - 1){
        if (be) {
            value = reverse_2(value);
        }
        mem->data[address] = (uint8_t)value;
        mem->data[address+1] = (uint8_t)(value >> 8);
        return 0;
    }
    return DATA_ABORT;
}

int memory_write_word(memory mem, uint32_t address, uint32_t value, uint8_t be) {
    if(mem != NULL && address < mem->size - 3){
        if (be) {
            value = reverse_4(value);
        }
        mem->data[address] = (uint8_t)value;
        mem->data[address+1] = (uint8_t)(value >> 8);
        mem->data[address+2] = (uint8_t)(value >> 16);
        mem->data[address+3] = (uint8_t)(value >> 24);
        return 0;
    }
    return DATA_ABORT;
}

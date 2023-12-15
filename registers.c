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
#include "registers.h"
#include "arm_constants.h"
#include <stdlib.h>

struct registers_data {
    /*   compl ter... */
    uint32_t reg[13]; // r0-r12
    uint32_t sp;    // r13
    uint32_t lr;    // r14
    uint32_t pc;    // r15
    uint32_t cpsr;  // CPSR
    uint32_t spsr;  // SPSR
};

// Définition des modes ARMv5
#define MODE_USER 0x10
#define MODE_FIQ  0x11 // Fast Interrupt Request
#define MODE_IRQ  0x12 // Interrupt Request
#define MODE_SVC  0x13 // Supervisor
#define MODE_ABT  0x17 // Abort
#define MODE_UND  0x1B // Undefined

registers registers_create() {
    /* registers r = NULL; */
    /*   compl ter... */

    registers r;
    r->reg[0] = 0;
    r->reg[1] = 0;
    r->reg[2] = 0;
    r->reg[3] = 0;
    r->reg[4] = 0;
    r->reg[5] = 0;
    r->reg[6] = 0;
    r->reg[7] = 0;
    r->reg[8] = 0;
    r->reg[9] = 0;
    r->reg[10] = 0;
    r->reg[11] = 0;
    r->reg[12] = 0;

    r->sp = 0;
    r->lr = 0;
    r->pc = 0;
    r->cpsr = 0;
    r->spsr = 0;

    return r;
}

void registers_destroy(registers r) {
    /*   compl ter... */
    free(r);
}

uint8_t registers_get_mode(registers r) {
    // Masque pour extraire le champ du mode dans CPSR
    uint32_t modeMask = 0x1F; // 5 bits de poids faible de CPSR pour le mode

    // Extraire le champ du mode actuel
    uint32_t currentMode = r->cpsr & modeMask;

    // Convertir le mode en uint8_t si nécessaire
    uint8_t mode = (uint8_t)currentMode;

    return mode;
    /* return SVC; */
}

static int registers_mode_has_spsr(registers r, uint8_t mode) { //POURQUOI METTRE r ??
    /*   compl ter... */
    switch (mode) {
        case MODE_FIQ:
        case MODE_IRQ:
        case MODE_SVC:
        case MODE_ABT:
        case MODE_UND:
            return 1; // True, le mode a un SPSR associé
        default:
            return 0; // False, le mode n'a pas de SPSR associé ( mode=MODE_USER )
    }
}

int registers_current_mode_has_spsr(registers r) {
    return registers_mode_has_spsr(r, registers_get_mode(r));
}

int registers_in_a_privileged_mode(registers r) {
    // Masque pour extraire le champ du mode dans CPSR (5 bits de poids faible)
    uint32_t modeMask = 0x1F;

    // Extraire le champ du mode actuel
    uint32_t currentMode = r.cpsr & modeMask;

    // Vérifier si le mode est un mode privilégié (SVC ou supérieur)
    return (currentMode >= MODE_SVC);
}

uint32_t registers_read(registers r, uint8_t reg, uint8_t mode) {
    switch (mode) {
        case MODE_USER:
        case MODE_FIQ:     
        case MODE_IRQ:
        case MODE_SVC:
        case MODE_ABT:
        case MODE_UND:
            return r->reg[reg];
        default:
            // Mode inconnu
            return 0;
    }
}

uint32_t registers_read_cpsr(registers r) {
    /* uint32_t value = 0; */
    /*   compl ter... */
    return r->cpsr;
    /* return value; */
}

uint32_t registers_read_spsr(registers r, uint8_t mode) {
    switch (mode) {
        case MODE_FIQ:
        case MODE_IRQ:
        case MODE_SVC:
        case MODE_ABT:
        case MODE_UND:
            return r->spsr;
        default:
            // Mode inconnu
            return 0;
    }
}

void registers_write(registers r, uint8_t reg, uint8_t mode, uint32_t value) {
    /*   compl ter... */
}

void registers_write_cpsr(registers r, uint32_t value) {
    /*   compl ter... */
}

void registers_write_spsr(registers r, uint8_t mode, uint32_t value) {
    /*   compl ter... */
}

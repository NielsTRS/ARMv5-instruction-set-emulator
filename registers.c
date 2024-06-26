/*
Armator - simulateur de jeu d'instruction ARMv5T   but pédagogique
Copyright (C) 2011 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique Générale GNU publiée par la Free Software
Foundation (version 2 ou bien toute autre version ultérieure choisie par vous).

Ce programme est distribué  car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but spécifique. Reportez-vous à la
Licence Publique Générale GNU pour plus de détails.

Vous devez avoir re u une copie de la Licence Publique Générale GNU en même
temps que ce programme ; si ce n'est pas le cas, écrivez à la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
états-Unis.

Contact: Guillaume.Huard@imag.fr
	 Bâtiment IMAG
	 700 avenue centrale, domaine universitaire
	 38401 Saint Martin d'Hères
*/
#include "registers.h"
#include "arm_constants.h"
#include <stdlib.h>
#include "arm_constants.h"

struct registers_data {
    uint32_t *reg; // r0-r12:utilisation libre, r13:sp, r14:lr, r15:pc
    uint32_t cpsr;  // CPSR
    uint32_t spsr;  // SPSR
};

registers registers_create() {

    registers r = malloc(sizeof(struct registers_data));
    if (r == NULL) {
        return NULL;
    }
    r->reg = calloc(16, sizeof(uint32_t));
    if (r->reg == NULL) {
        free(r);
        return NULL;
    }
    r->cpsr = 0;
    r->spsr = 0;

    return r;
}

void registers_destroy(registers r) {
    free(r->reg);
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
}

static int registers_mode_has_spsr(registers r, uint8_t mode) { //POURQUOI METTRE r ??
    switch (mode) {
        case FIQ:
        case IRQ:
        case SVC:
        case ABT:
        case UND:
            return 1; // True, le mode a un SPSR associé
        default:
            return 0; // False, le mode n'a pas de SPSR associé ( mode=SYS ou mode=USR )
    }
}

int registers_current_mode_has_spsr(registers r) {
    return registers_mode_has_spsr(r, registers_get_mode(r));
}

int registers_in_a_privileged_mode(registers r) {
    // Masque pour extraire le champ du mode dans CPSR (5 bits de poids faible)
    uint32_t modeMask = 0x1F;

    // Extraire le champ du mode actuel
    uint32_t currentMode = r->cpsr & modeMask;

    // Vérifier si le mode est un mode privilégié (SVC ou supérieur)
    return (currentMode >= SVC);
}

uint32_t registers_read(registers r, uint8_t reg, uint8_t mode) {
    switch (mode) {
        case USR:
        case FIQ:
        case IRQ:
        case SVC:
        case ABT:
        case UND:
        case SYS:
            return r->reg[reg];
        default:
            // Mode inconnu
            return UNDEFINED_INSTRUCTION;
    }
}

uint32_t registers_read_cpsr(registers r) {
    return r->cpsr;
}

uint32_t registers_read_spsr(registers r, uint8_t mode) {
    switch (mode) {
        case USR:
        case FIQ:
        case IRQ:
        case SVC:
        case ABT:
        case UND:
        case SYS:
            return r->spsr;
        default:
            // Mode inconnu
            return UNDEFINED_INSTRUCTION;
    }
}

void registers_write(registers r, uint8_t reg, uint8_t mode, uint32_t value) {
    switch (mode) {
        case USR:
        case FIQ:
        case IRQ:
        case SVC:
        case ABT:
        case UND:
        case SYS:
            r->reg[reg] = value;
            break;
        default:
            // Mode inconnu
            break;
    }
}

void registers_write_cpsr(registers r, uint32_t value) {
    r->cpsr = value;
}

void registers_write_spsr(registers r, uint8_t mode, uint32_t value) {
    switch (mode) {
        case USR:
        case FIQ:
        case IRQ:
        case SVC:
        case ABT:
        case UND:
        case SYS:
            r->spsr = value;
            break;
        default:
            // Mode inconnu
            break;
    }
}

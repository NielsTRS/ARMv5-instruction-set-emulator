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
#ifndef __ARM_CONSTANTS_H__
#define __ARM_CONSTANTS_H__
#include <stdint.h>

/* Ajout des macros data_processing */
#define AND 0x00
#define EOR 0x01
#define SUB 0x02
#define RSB 0x03
#define ADD 0x04
#define ADC 0x05
#define SBC 0x06
#define RSC 0x07
#define TST 0x08
#define TEQ 0x09
#define CMP 0x0A
#define CMN 0x0B
#define ORR 0x0C
#define MOV 0x0D
#define BIC 0x0E
#define MVN 0x0F

/* Ajout des macros load_store */
#define LDR 0x00
#define LDRB 0x01
#define LDRH 0x02
#define STR 0x03
#define STRB 0x04
#define STRH 0x05
#define LDM 0x06
#define STM 0x07

/* ARM Cond */
#define EQ 0x00
#define NE 0x01
#define CS_HS 0x02
#define CC_LO 0x03
#define MI 0x04
#define PL 0x05
#define VS 0x06
#define VC 0x07
#define HI 0x08
#define LS 0x09
#define GE 0x0A
#define LT 0x0B
#define GT 0x0C
#define LE 0x0D
#define AL 0x0E

/* ARM Modes */
#define USR 0x10
#define FIQ 0x11
#define IRQ 0x12
#define SVC 0x13
#define ABT 0x17
#define UND 0x1b
#define SYS 0x1f

/* ARM Exceptions (by priority) */
#define RESET                   1
#define DATA_ABORT              2
#define FAST_INTERRUPT          3
#define INTERRUPT               4
#define IMPRECISE_ABORT         5	// Unsupported, ARMV6
#define PREFETCH_ABORT          6
#define UNDEFINED_INSTRUCTION   7
#define SOFTWARE_INTERRUPT      8
/* The last one is not realy an exception, but, as we handle software interrupts
 * within the simulator and we decide there to end the simulation, this is a way
 * to tell it to the outside world
 */
#define END_SIMULATION          9

/* Some CPSR bits */
#define N 31
#define Z 30
#define C 29
#define V 28

/* shift operations */
#define LSL 0
#define LSR 1
#define ASR 2
#define ROR 3

/* Bit mask constants for msr */
/* We simulate architecture v5T */
#define UnallocMask 0x0FFFFF00
#define UserMask    0xF0000000
#define PrivMask    0x0000000F
#define StateMask   0x00000020

char *arm_get_exception_name(unsigned char exception);
char *arm_get_mode_name(uint8_t mode);
int8_t arm_get_mode_number(char *name);
char *arm_get_register_name(uint8_t reg);

#endif

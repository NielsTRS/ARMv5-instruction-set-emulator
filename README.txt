

GUIDE de compilation/execution du simulateur 
fichier run.sh options -r -m -d 
./run.sh -m : config + make all
./run.sh -r : lancement du simulateur + gdb set en big endian et connecter au simulateur
./run.sh -d : a utiliser avec -r, option debug active toutes les traces + redirection des erreurs de gdb + sim dans un fichier

Fonctionalités implementées:
les instructions de data processing AND EOR SUB RSB ADD ADC SBC RSC TST TEQ CMP CMN ORR MOV BIC MVN MRS
l'option de mise a joue des flags suivant le bit S
les instructions de load/store LDR LDRB LDRH STR STRB STRH LDM STM
les shift LSL LSR ASR ROR (version immediate et entre registres)

Bogues connus:
instruction CLZ: ne compile pas CLZ ? instr pas reconnu en armv5
mise a joue du flag C suivant shifter_carry_out (possiblement reglé pas eu le temps de test)

TESTS effectué:
Tout les principaux tests effectués sont dans le dossier Exemples, ils sont tous commentés, 
chaque fichier test un groupe d'instructions specifique. (test commentés, juste a verifier manuellement la bonne execution)

ඞ
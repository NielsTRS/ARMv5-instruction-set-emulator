CC=$(GCC_CROSS)
CCAS=$(GCC_CROSS)
# Our simulator :
# - is big-endian
# - does not initialize properly fp (so we omit it), see arm_core.c
# - has a non standard main (no OS so no return value), we disable the warning
AM_CFLAGS=-mbig-endian -fomit-frame-pointer -Wno-main
# Still big-endian for ARM assembly language sources
AM_CCASFLAGS=-mbig-endian
# We don't have any OS, so no stdlib as well
# The linker script maps loadable segments into memory, the overall size must match the simulator memory
# The entry point will always be main
AM_LDFLAGS=-nostdlib -T $(srcdir)/linker_script -n --entry main
# Our simulator is big-endian
AM_LDFLAGS+=-Wl,-EB

if HAVE_ARM_COMPILER
noinst_PROGRAMS=example1 example2 example3 example4 example5 example6 example7 example8 example9 example10 \
 						 example11 example12 example13 example14 example15 example16 example17 example18 example19 example20 example21 insertion_sort

all-am: $(PROGS)
endif

example1_SOURCES=example1.s
example2_SOURCES=example2.s
example3_SOURCES=example3.s
example4_SOURCES=example4.s
example5_SOURCES=example5.s
example6_SOURCES=example6.s
example7_SOURCES=example7.s
example8_SOURCES=example8.s
example9_SOURCES=example9.s
example10_SOURCES=example10.s
example11_SOURCES=example11.s
example12_SOURCES=example12.s
example13_SOURCES=example13.s
example14_SOURCES=example14.s
example15_SOURCES=example15.s
example16_SOURCES=example16.s
example17_SOURCES=example17.s
example18_SOURCES=example18.s
example19_SOURCES=example19.s
example20_SOURCES=example20.s
example21_SOURCES=example21.s
insertion_sort_SOURCES=insertion_sort.c my_syscalls.s my_syscalls.h

EXTRA_DIST=linker_script

clean-local:
	$(RM) $(PROGS)
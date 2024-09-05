# Location of top-level MicroPython directory
MPY_DIR = /home/sjaak/Documents/micropython/micropython

# Architecture to build for (x86, x64, armv7m, xtensa, xtensawin)
ARCH = xtensawin

# Name of module
MOD = perlin

# Source files (.c or .py)
SRC = perlin.c

# Stuff to make soft-float work
# If symbols are undefined, use tools/find_symbols.py to locate object files to add
ifeq ($(ARCH), armv7emsp)
	SOFTFP_O := _arm_addsubdf3.o _arm_truncdfsf2.o _arm_fixdfsi.o
	SOFTFP_ENABLE=1
	SRC_O += $(SOFTFP_O)
endif
ifeq ($(ARCH), xtensawin)
	SOFTFP_O := _extendsfdf2.o _truncdfsf2.o _divsf3.o _sqrtf.o _floatsidf.o _fixdfsi.o
	SOFTFP_ENABLE=1
	SRC_O += $(SOFTFP_O)
endif

# Include to get the rules for compiling and linking the module
include $(MPY_DIR)/py/dynruntime.mk

# CROSS is defined by the included
LIBGCC_FILENAME = $(shell $(CROSS)gcc $(CFLAGS) -print-libgcc-file-name)
$(info $(LIBGCC_FILENAME))

ifeq ($(ARCH), armv7emsp)
_arm_addsubdf3.o:
	$(CROSS)ar -x $(LIBGCC_FILENAME) $(SOFTFP_O)
_arm_truncdfsf2.o:
	$(CROSS)ar -x $(LIBGCC_FILENAME) $(SOFTFP_O)
_arm_fixdfsi.o:
	$(CROSS)ar -x $(LIBGCC_FILENAME) $(SOFTFP_O)
endif
ifeq ($(ARCH), xtensawin)
_extendsfdf2.o:
	$(CROSS)ar -x $(LIBGCC_FILENAME) $(SOFTFP_O)
_truncdfsf2.o:
	$(CROSS)ar -x $(LIBGCC_FILENAME) $(SOFTFP_O)
_divsf3.o:
	$(CROSS)ar -x $(LIBGCC_FILENAME) $(SOFTFP_O)
_sqrtf.o:
	$(CROSS)ar -x $(LIBGCC_FILENAME) $(SOFTFP_O)
_floatsidf.o:
	$(CROSS)ar -x $(LIBGCC_FILENAME) $(SOFTFP_O)
_fixdfsi.o:
	$(CROSS)ar -x $(LIBGCC_FILENAME) $(SOFTFP_O)
endif
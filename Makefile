# From: linuxdevcenter.com

# Buildroot output path (change this to suit your configuration)
BR_OUTPUT := /home/luisky/cours/M2_LSE/SEE/TP/TP5-9/buildroot/output/

# Cross compiler location
GCC_CROSS := $(BR_OUTPUT)host/usr/bin/arm-linux-gnueabihf-gcc

# default is the default make target.  The rule here says to run make
# with a working directory of the directory containing the kernel
# source and compile only the modules in the PWD (local) directory.
default:
	$(GCC_CROSS) -o K2000 k2000.c
	$(GCC_CROSS) -o counter counter.c
	$(GCC_CROSS) -o gensig gensig.c

.PHONY: clean
clean:
	$(RM) -r	K2000 counter gensig
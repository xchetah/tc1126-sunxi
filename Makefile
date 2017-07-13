#
# Makefile for the touchscreen drivers.
#

# Each configuration option enables a list of files.
ifeq ($(filter y,$(CONFIG_ARCH_SUN8IW3P1) $(CONFIG_ARCH_SUN8IW5P1)),y)
DRIVER=cn1100_a23_a33
else
DRIVER=cn1100_a31
endif
obj-$(CONFIG_TOUCHSCREEN_TC1126_TS)       += tc1126_ts.o
tc1126_ts-objs := $(DRIVER).o debug_proc.o TC1126_data.o TC1126_hwService.o


ifneq ($(KERNELRELEASE),)
# kbuild part of makefile
obj-m := hello_pasha.o
ccflags-y += -g # add debugging info
else
# normal makefile
KDIR ?= /lib/modules/`uname -r`/build

default:
	$(MAKE) -C $(KDIR) M=$$PWD
	cp hello_pasha.ko hello_pasha.ko.unstripped
	$(CROSS_COMPILE)strip -g hello_pasha.ko # strip only debugging info
clean:
	$(MAKE) -C $(KDIR) M=$$PWD clean
%.s %.i: %.c # just use make hello_pasha.s instead of objdump
	$(MAKE) -C $(KDIR) M=$$PWD $@
endif


obj-m +=q1.o
KDIR ?= /lib/modules/$(shell uname -r)/build
all:
	$(MAKE) -C $(KDIR) M=$(shell pwd) modules
clear:
	$(MAKE) -C $(KDIR) M=$(shell pwd) clean
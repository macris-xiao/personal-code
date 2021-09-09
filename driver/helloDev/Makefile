ifneq ($(KERNELRELEASE),)
obj-m := helloDev.o
else
PWD := $(shell pwd)
#KDIR:=/home/jinxin/linux-4.9.229
#KDIR:=/lib/modules/4.4.0-31-generic/build
KDIR := /home/xiaoy/test/net-next-private
all:
	make -C $(KDIR) M=$(PWD)
clean:
	rm -rf *.o *.ko* *.mod* *.symvers *.order *.c~ *~
endif

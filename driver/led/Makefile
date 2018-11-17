KENRELDIR := /home/zhang/linux
obj-m += led_driver.o
default:
		$(MAKE) -C $(KENRELDIR) M=$(shell pwd) modules
clean:
		$(MAKE) -C $(KENRELDIR) M=$(shell pwd) module clean

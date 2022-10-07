obj-m += my_driver.o
KDIR = /lib/modules/$(shell uname -r)/build

#hide output console
HIDE ?= @

build:
	make -C $(KDIR) M=$(shell pwd) modules
clean:
	make -C $(KDIR) M=$(shell pwd) clean



#$(HIDE)if test ! -d $(BUILDDIR); then mkdir $(BUILDDIR);fi;
#$(HIDE)$(RM) -rf $(BUILDDIR)
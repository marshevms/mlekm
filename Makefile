PWD := $(CURDIR)

.PHONY: build
build:
	make -C /lib/modules/$(shell uname -r)/build M=$$PWD/src modules

.PHONY: clean
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$$PWD/src clean

.PHONY: help
help:
	make -C /lib/modules/$(shell uname -r)/build help
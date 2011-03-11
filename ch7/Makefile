obj-m += chardev.o

source := ioctl.c
out := ioctl

program:
	gcc -Wall -Wextra -O2 -g -o $(out) $(source)

all:	program
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f $(out)

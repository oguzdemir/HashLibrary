#type make to compile with this Makefile

#you may change the path below to your compile directory
MYPATH = /home/oguz/Desktop/p3


all: libhash  


libhash: hash.c
	gcc -c -Wall -fpic hash.c
	gcc -shared -o libhash.so hash.o -lpthread


#type "make clean" to clean your directory for recompile
clean: 
	rm -fr dp.o *.so *.o *~

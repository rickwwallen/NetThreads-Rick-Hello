 TARGET=rick_hello
 include ../bench.mk
 rick_hello: rick_hello.o pktbuff.o memcpy.o arp.o dev.o

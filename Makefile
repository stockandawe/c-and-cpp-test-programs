#
# This makefile contains modifications submitted by Richard Braakman
# (dark@xs4all.nl) for the shared library generation.
#

# By default, ftplib uses PASV.  If you need it to use  PORT
# instead, uncomment the next line

#testprog : main.o ftplib.o
#    cc -o testprog main.o ftplib.o

#main.o : ftp.c
#    cc -c ftp.c

ftp.o : ftplib.c ftplib.h
    cc -c ftplib.c

clean :
    rm main.o testprog

CC = /usr/bin/gcc

objs = simM.o
node_objs = node_process.o

all: simM node_process

simM: $(objs)
	$(CC) -o simM $(objs)

node_process: $(node_objs)
	$(CC) -o node_process $(node_objs)

clean:
	-rm -f simM node_process

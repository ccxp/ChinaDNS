objects = chinadns.o gfw_util.o iBase64.o local_ns_parser.o

all: $(objects) chinadns

$(objects): %.o: %.c 
	$(CC) -c $(CFLAGS) $< -o $@ 

chinadns : chinadns.o
	$(CC) $(LDFLAGS) $(objects) -o chinadns -lresolv

clean :
	rm *.o chinadns

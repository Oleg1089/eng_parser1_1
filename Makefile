CRCMODULES = main.c eng_functions.c eng_functions.h
OBJMODULES = $(CRCMODULES:.c=.o)
#CC = E:/gcc64/bin/gcc
CFLAGS = -Wall -m64

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

engdata_parser: $< $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@

run: engdata_parser
	./engdata_parser RA672307.ENG

clean:
	rm -f *.o engdata_parser
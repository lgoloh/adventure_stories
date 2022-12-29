CFLAGS=-std=gnu++98 -pedantic -Wall -Werror -ggdb3
PROGS=cyoa-step1 cyoa-step2 cyoa-step3 cyoa-step4
OBJS=$(patsubst %,%.o,$(PROGS)) cyoa.o page.o choice.o
all: $(PROGS)
cyoa-step%: cyoa-step%.o cyoa.o page.o choice.o story.o
	g++ -o $@ $^
%.o: %.cpp %.hpp
	g++ $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f *~ $(PROGS) $(OBJS)

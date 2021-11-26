TARGET_EXEC ?= stencil3d

OPT ?= -O3 -ftree-vectorize 
GCC ?= gcc

CFLAGS ?=  -g $(OPT) -Wall -std=c11 -D_GNU_SOURCE

LDFLAGS ?= -lm

FILE = $(shell basename "$$PWD").tar.gz
TARGETS = stencil.c cookie.txt comment.txt
SERVER = lnxsrv07
API = http://${SERVER}.seas.ucla.edu:15213/upload 

$(TARGET_EXEC): main.o stencil.o stencil-check.o
	$(GCC) $^ -o $@ $(LDFLAGS)

# Add this option "-fopt-info-vec-all" to see the compiler's vectorization report!! It's so much fun.
stencil.o: stencil.c stencil.h
	$(GCC) $(CFLAGS) -c $< -o $@

main.o: main.c main.h stencil.h 
	$(GCC) $(CFLAGS) -c $< -o $@

submit: build
		echo 'sending request to the server...'
		echo "it might take a few seconds..."
		curl -F 'file=@$(FILE)' $(API)
		
		
.SILENT:
build: 
		@tar -czvf $(FILE) $(TARGETS) 2>/dev/null

.PHONY: clean

clean:
	$(RM) -r $(TARGET_EXEC) stencil.o main.o



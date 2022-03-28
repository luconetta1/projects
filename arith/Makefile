############## Variables ###############

CC = gcc # The compiler being used

# Updating include path to use Comp 40 .h files and CII interfaces
IFLAGS = -I/comp/40/build/include/ -I/usr/sup/cii40/include/cii/ 

# Compile flags
# Set debugging information, allow the c99 standard,
# max out warnings, and use the updated include path
CFLAGS = -g -std=gnu99 -Wall -Wextra -Werror -Wfatal-errors -pedantic $(IFLAGS)

# Linking flags
# Set debugging information and update linking path
# to include course binaries and CII implementations
LDFLAGS = -g -L/comp/40/build/lib -L/usr/sup/cii40/lib64

# Libraries needed for linking
# All programs cii40 (Hanson binaries) and *may* need -lm (math)
# arith40 is a catch-all for this assignment, netpbm is needed for pnm
LDLIBS = -l40locality -larith40 -lnetpbm -lcii40 -lm -lrt 
# -l40locality -l40arith 

# Collect all .h files in your directory.
# This way, you can never forget to add
# a local .h file in your dependencies.
#
# This bugs Mark, who dislikes false dependencies, but
# he agrees with Noah that you'll probably spend hours 
# debugging if you forget to put .h files in your 
# dependency list.
INCLUDES = $(shell echo *.h)

############### Rules ###############

all: ppmdiff test_bit 40image-6


## Compile step (.c files -> .o files)

# To get *any* .o file, compile its .c file with the following rule.
%.o: %.c $(INCLUDES)
		$(CC) $(CFLAGS) -c $< -o $@


## Linking step (.o -> executable program)

test_bit: testBitpack.o bitpack.o
		$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)


40image: 40image.o compress40.o uarray2.o a2plain.o bitpack.o trans6.o print.o
		$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)


40image-6: 40image.o compress40.o uarray2.o a2plain.o bitpack.o trans.o print.o
		$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

ppmdiff: ppmdiff.o uarray2.o a2plain.o 
		$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

clean:
		rm -f ppmdiff 40image test_bit *.o
# Builds circ demo

# Requirements:
# 1. Gmake must be used.
# 2. The cc65 compiler must be properly setup.

ifndef CC65_TARGET
	CC65_TARGET:=apple2enh
endif

CC=cl65
CFLAGS=-O -t $(CC65_TARGET) -DTRACE
# The -S $6000 makes the start address $6000 so that both hi-res
# pages are available.
LDFLAGS=-t $(CC65_TARGET)
DISK_VOL=circ
DISK=$(DISK_VOL).dsk
PGM=circ
GRAF_DRVR=a2e.hi
DYN_GRAF_DRVR=a2e.hi.tgi
BASIC_AUX_TYPE=0x0801
READ_TIME_LOAD_ADDR=0x0260
AC=java -jar AppleCommander-ac-1.4.0.jar
SYS_LOAD_ADDR=0x2000
BIN_LOAD_ADDR=0x0803
MKDISK=$(AC) -pro140 $(DISK) $(DISK_VOL)

########################################

all: $(DISK)

$(DISK): $(PGM) 
	$(RM) $(DISK)
	$(AC) -pro140 $(DISK) $(DISK_VOL)
	$(AC) -as $(DISK) $(PGM) BIN < $(PGM)
	$(AC) -p $(DISK) $(DYN_GRAF_DRVR) BIN < $(DYN_GRAF_DRVR)

$(PGM): $(PGM).o $(GRAF_DRVR).o
	$(CC) $(LDFLAGS) -o $@ $^

$(GRAF_DRVR).o: $(GRAF_DRVR).s
	ca65 $^

$(GRAF_DRVR).s: $(GRAF_DRVR).tgi
	co65 --code-label _a2e_hi $^

clean:
	$(RM) *.o $(PGM) $(DISK) $(GRAF_DRVR).s


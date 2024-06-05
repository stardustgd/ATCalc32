project=ATCalc32
bin=bin
programmer=atmelice_isp
avrType=atmega32
avrFreq=1000000

SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c, $(bin)/%.o, $(SRCS))

cflags=-g -DF_CPU=${avrFreq} -Wall -Os -Wextra

all: $(bin)/$(project).flash.hex

$(bin)/%.o: %.c
	@mkdir -p $(bin)
	avr-gcc $(cflags) -mmcu=$(avrType) -Wa,-ahlmns=$(bin)/$(notdir $<).lst -c -o $@ $<

$(bin)/$(project).elf: $(OBJS)
	avr-gcc $(cflags) -mmcu=$(avrType) -o $@ $^

$(bin)/$(project).flash.hex: $(bin)/$(project).elf
	avr-objcopy -j .text -j .data -O ihex $< $@

flash: $(bin)/$(project).flash.hex
	avrdude -c$(programmer) -p$(avrType) -v -U flash:w:$<

clean:
	$(RM) $(bin)/*

zip:
	zip $(project) *.c *.h

fuse:
	@echo "1. 8 MHz external crystal"
	@echo "2. Default"
	@read -p ": " choice; \
	case $$choice in \
		1) avrdude -c$(programmer) -p$(avrType) -v -U lfuse:w:0xff:m -U hfuse:w:0x99:m ;; \
		2) avrdude -c$(programmer) -p$(avrType) -v -U lfuse:w:0xe1:m -U hfuse:w:0x99:m ;; \
	esac

.PHONY: clean flash zip fuse

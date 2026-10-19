CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude

MCU = atmega328p
F_CPU = 16000000UL
PORT = /dev/ttyUSB0

TARGET = main
SRC = main.c

CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os

all: hex flash

$(TARGET).bin: $(SRC)
	$(CC) $(CFLAGS) -o $@ $<

hex: $(TARGET).bin
	$(OBJCOPY) -j .text -j .data -O ihex $< $(TARGET).hex

flash:
	$(AVRDUDE) -p $(MCU) -c arduino -P $(PORT) -b 115200 -U flash:w:$(TARGET).hex:i

clean:
	rm -f $(TARGET).bin $(TARGET).hex

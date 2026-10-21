# Codie robot firmware — rebuild from source
#
#   make            build build/fw.elf + build/fw.bin   (STM32G071RB, 128K flash)
#   make flash      upload build/fw.bin over USB DFU
#   make size       show section sizes
#
# Toolchain: any arm-none-eabi-gcc >= 9.  Override with
#   make TOOLCHAIN_PATH=/path/to/arm-none-eabi/bin
# or put arm-none-eabi-gcc on your PATH.

TOOLCHAIN_PATH ?= $(firstword $(wildcard $(CURDIR)/../tools/xpack-arm-none-eabi-gcc-*/bin) $(shell command -v arm-none-eabi-gcc 2>/dev/null | xargs dirname 2>/dev/null))
ifeq ($(strip $(TOOLCHAIN_PATH)),)
$(error No arm-none-eabi toolchain found. Install one or set TOOLCHAIN_PATH=...)
endif

CC      := $(TOOLCHAIN_PATH)/arm-none-eabi-gcc
OBJCOPY := $(TOOLCHAIN_PATH)/arm-none-eabi-objcopy
SIZE    := $(TOOLCHAIN_PATH)/arm-none-eabi-size

TARGET  := fw
BUILD   := build
SRC     := src
MODULES := modules
INCLUDE := include

# -mcpu=cortex-m0plus : STM32G0x1 is Cortex-M0+
CFLAGS := -mcpu=cortex-m0plus -mthumb -Os -g3 -ffunction-sections -fdata-sections \
          -Wall -Wextra -Wno-unused-parameter -fno-common \
          -DSTM32G071xx -DUSE_HAL_DRIVER=0 \
          -I$(INCLUDE) -I$(INCLUDE)/CMSIS -I$(SRC) -I$(MODULES)

LDFLAGS := -mcpu=cortex-m0plus -mthumb -T$(CURDIR)/linker/stm32g071rb.ld \
           -nostartfiles -Wl,--gc-sections -Wl,-Map=$(BUILD)/$(TARGET).map --specs=nosys.specs

SRCS := $(shell find $(SRC) $(MODULES) -name '*.c' 2>/dev/null)
OBJS := $(addprefix $(BUILD)/,$(SRCS:.c=.o))
DEPS := $(OBJS:.o=.d)

all: $(BUILD)/$(TARGET).bin size

$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD)/$(TARGET).elf: $(OBJS) linker/stm32g071rb.ld
	@mkdir -p $(BUILD)
	$(CC) $(OBJS) $(LDFLAGS) -o $@
	$(SIZE) $@

$(BUILD)/$(TARGET).bin: $(BUILD)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@
	@echo "built $@: $$(stat -c%s $<) bytes"

size: $(BUILD)/$(TARGET).elf
	@$(SIZE) -A $< | head -20

flash: $(BUILD)/$(TARGET).bin
	./flash.sh $(BUILD)/$(TARGET).bin

# Host side tests: the protocol layer against the expected bytes, and the
# whole firmware running against a fake memory-mapped peripheral map.
HOSTCC ?= cc
test: $(BUILD)/$(TARGET).elf
	@echo "== protocol layer =="
	$(HOSTCC) -std=c99 -Iinclude ../tools/protocol_test.c modules/proxy.c -o $(BUILD)/protocol_test
	$(BUILD)/protocol_test
	@echo
	@echo "== full firmware in the host simulator =="
	../tools/run_sim.sh

clean:
	rm -rf $(BUILD)

-include $(DEPS)

.PHONY: all size flash clean test

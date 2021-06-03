#******************************************************************************
# Originally emitted by VisualTeensy (https://github.com/luni64/VisualTeensy)
# hacked for Linux and WSL by sutaburosu
#
# Board              Teensy 4.1
# USB Type           Serial
# CPU Speed          600 MHz
# Optimize           Fastest
# Keyboard Layout    US English
#
#******************************************************************************

# Uncomment this to for a silent build (except warnings and errors)
QUIET            := \#

# Keep the settings in `.vscode/c_cpp_properties.json` synced with these

TARGET_NAME      := playpen
BOARD_ID         := TEENSY41

MCU              := imxrt1062

ARDUINO_BASE     := /opt/arduino-1.8.15

LIBS_SHARED_BASE := ${HOME}/Arduino/libraries
LIBS_SHARED      := SmartMatrix FastLED

LIBS_LOCAL_BASE  := lib
LIBS_LOCAL       := 

LIBS_CORE_BASE   := $(ARDUINO_BASE)/hardware/teensy/avr/libraries
LIBS_CORE        := SPI

CORE_BASE        := $(ARDUINO_BASE)/hardware/teensy/avr/cores/teensy4
GCC_BASE         := $(ARDUINO_BASE)/hardware/tools/arm/bin
UPL_PJRC_B       := $(ARDUINO_BASE)/hardware/tools

#******************************************************************************
# Flags and Defines
#******************************************************************************
FLAGS_CPU   := -mthumb -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-d16
FLAGS_OPT   := -O3
FLAGS_COM   := -g -Wall -ffunction-sections -fdata-sections -nostdlib -MMD
FLAGS_LSP   := 

FLAGS_CPP   := -std=gnu++14 -fno-exceptions -fpermissive -fno-rtti -fno-threadsafe-statics -felide-constructors -Wno-error=narrowing
FLAGS_C     := 
FLAGS_S     := -x assembler-with-cpp
FLAGS_LD    := -Wl,--print-memory-usage,--gc-sections,--relax,--defsym=__rtc_localtime=$(shell date +%s) -T$(CORE_BASE)/imxrt1062_t41.ld

LIBS        := -larm_cortexM7lfsp_math -lm -lstdc++

DEFINES     := -D__IMXRT1062__ -DTEENSYDUINO=154 -DARDUINO_TEENSY41 -DARDUINO=10815
DEFINES     += -DF_CPU=600000000 -DUSB_SERIAL -DLAYOUT_US_ENGLISH

CPP_FLAGS   := $(FLAGS_CPU) $(FLAGS_OPT) $(FLAGS_COM) $(DEFINES) $(FLAGS_CPP)
C_FLAGS     := $(FLAGS_CPU) $(FLAGS_OPT) $(FLAGS_COM) $(DEFINES) $(FLAGS_C)
S_FLAGS     := $(FLAGS_CPU) $(FLAGS_OPT) $(FLAGS_COM) $(DEFINES) $(FLAGS_S)
LD_FLAGS    := $(FLAGS_CPU) $(FLAGS_OPT) $(FLAGS_LSP) $(FLAGS_LD)
AR_FLAGS    := rcs
NM_FLAGS    := --numeric-sort --defined-only --demangle --print-size

#******************************************************************************
# Colors
#******************************************************************************
COL_CORE    := $(shell tput setaf 4)
COL_LIB     := $(shell tput setaf 6)
COL_SRC     := $(shell tput setaf 7)
COL_LINK    := $(shell tput setaf 6)
COL_ERR     := $(shell tput setaf 1)
COL_OK      := $(shell tput setaf 2)
COL_RESET   := $(shell tput setaf 9)

#******************************************************************************
# Folders and Files
#******************************************************************************
USR_SRC         := src
LIB_SRC         := lib
CORE_SRC        := $(CORE_BASE)

BIN             := build
FULL_PATH       := $(shell pwd)/$(BIN)
USR_BIN         := $(BIN)/src
CORE_BIN        := $(BIN)/core
LIB_BIN         := $(BIN)/lib
CORE_LIB        := $(BIN)/core.a
TARGET_HEX      := $(BIN)/$(TARGET_NAME).hex
TARGET_ELF      := $(BIN)/$(TARGET_NAME).elf
TARGET_LST      := $(BIN)/$(TARGET_NAME).lst
TARGET_SYM      := $(BIN)/$(TARGET_NAME).sym

#******************************************************************************
# BINARIES
#******************************************************************************
CC              := $(GCC_BASE)/arm-none-eabi-gcc
CXX             := $(GCC_BASE)/arm-none-eabi-g++
AR              := $(GCC_BASE)/arm-none-eabi-gcc-ar
NM              := $(GCC_BASE)/arm-none-eabi-gcc-nm
SIZE            := $(GCC_BASE)/arm-none-eabi-size
OBJDUMP         := $(GCC_BASE)/arm-none-eabi-objdump
OBJCOPY         := $(GCC_BASE)/arm-none-eabi-objcopy
UPL_PJRC        := "$(UPL_PJRC_B)/teensy_post_compile" -test -file="$(TARGET_NAME)" -path="$(FULL_PATH)" -tools="$(UPL_PJRC_B)" -board=$(BOARD_ID) -reboot
UPL_TYCMD       := $(UPL_TYCMD_B)/tyCommanderC upload $(TARGET_HEX) --autostart --wait --multi
UPL_CLICMD      := $(UPL_CLICMD_B)/teensy_loader_cli -mmcu=$(MCU) -v $(TARGET_HEX)
UPL_JLINK       := $(UPL_JLINK_B)/jlink -commanderscript .vsteensy/flash.jlink

#******************************************************************************
# Source and Include Files
#******************************************************************************
# Recursively create list of source and object files in USR_SRC and CORE_SRC
# and corresponding subdirectories.
# The function rwildcard is taken from http://stackoverflow.com/a/12959694)

rwildcard =$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

#User Sources -----------------------------------------------------------------
USR_C_FILES     := $(call rwildcard,$(USR_SRC)/,*.c)
USR_CPP_FILES   := $(call rwildcard,$(USR_SRC)/,*.cpp)
USR_INO_FILES   := $(call rwildcard,$(USR_SRC)/,*.ino)
USR_S_FILES     := $(call rwildcard,$(USR_SRC)/,*.S)
USR_OBJ         := $(USR_S_FILES:$(USR_SRC)/%.S=$(USR_BIN)/%.o) $(USR_C_FILES:$(USR_SRC)/%.c=$(USR_BIN)/%.o) $(USR_CPP_FILES:$(USR_SRC)/%.cpp=$(USR_BIN)/%.o) $(USR_INO_FILES:$(USR_SRC)/%.ino=$(USR_BIN)/%.o)

# Core library sources --------------------------------------------------------
CORE_CPP_FILES  := $(call rwildcard,$(CORE_SRC)/,*.cpp)
CORE_C_FILES    := $(call rwildcard,$(CORE_SRC)/,*.c)
CORE_S_FILES    := $(call rwildcard,$(CORE_SRC)/,*.S)
CORE_OBJ        := $(CORE_S_FILES:$(CORE_SRC)/%.S=$(CORE_BIN)/%.o) $(CORE_C_FILES:$(CORE_SRC)/%.c=$(CORE_BIN)/%.o) $(CORE_CPP_FILES:$(CORE_SRC)/%.cpp=$(CORE_BIN)/%.o)

# User library sources (see https://github.com/arduino/arduino/wiki/arduino-ide-1.5:-library-specification)
LIB_DIRS_SHARED := $(foreach d, $(LIBS_SHARED), $(LIBS_SHARED_BASE)/$d/ $(LIBS_SHARED_BASE)/$d/utility/)      # base and /utility
LIB_DIRS_SHARED += $(foreach d, $(LIBS_SHARED), $(LIBS_SHARED_BASE)/$d/src/ $(dir $(call rwildcard,$(LIBS_SHARED_BASE)/$d/src/,*/.)))                          # src and all subdirs of base

LIB_DIRS_LOCAL  := $(foreach d, $(LIBS_LOCAL), $(LIBS_LOCAL_BASE)/$d/ $(LIBS_LOCAL_BASE)/$d/utility/ )        # base and /utility
LIB_DIRS_LOCAL  += $(foreach d, $(LIBS_LOCAL), $(LIBS_LOCAL_BASE)/$d/src/ $(dir $(call rwildcard,$(LIBS_LOCAL_BASE)/$d/src/,*/.)))                          # src and all subdirs of base

LIB_DIRS_CORE   := $(foreach d, $(LIBS_CORE), $(LIBS_CORE_BASE)/$d/ $(LIBS_CORE_BASE)/$d/utility/ )
LIB_DIRS_CORE   += $(foreach d, $(LIBS_CORE), $(LIBS_CORE_BASE)/$d/src/ $(dir $(call rwildcard,$(LIBS_CORE_BASE)/$d/src/,*/.)))

LIB_CPP_SHARED  := $(foreach d, $(LIB_DIRS_SHARED),$(call wildcard,$d*.cpp))
LIB_C_SHARED    := $(foreach d, $(LIB_DIRS_SHARED),$(call wildcard,$d*.c))
LIB_S_SHARED    := $(foreach d, $(LIB_DIRS_SHARED),$(call wildcard,$d*.S))

LIB_CPP_LOCAL   := $(foreach d, $(LIB_DIRS_LOCAL),$(call wildcard,$d/*.cpp))
LIB_C_LOCAL     := $(foreach d, $(LIB_DIRS_LOCAL),$(call wildcard,$d/*.c))
LIB_S_LOCAL     := $(foreach d, $(LIB_DIRS_LOCAL),$(call wildcard,$d/*.S))

LIB_CPP_CORE    := $(foreach d, $(LIB_DIRS_CORE),$(call wildcard,$d*.cpp))
LIB_C_CORE      := $(foreach d, $(LIB_DIRS_CORE),$(call wildcard,$d*.c))
LIB_S_CORE      := $(foreach d, $(LIB_DIRS_CORE),$(call wildcard,$d*.S))

LIB_OBJ         := $(LIB_CPP_SHARED:$(LIBS_SHARED_BASE)/%.cpp=$(LIB_BIN)/%.o) 
LIB_OBJ         += $(LIB_C_SHARED:$(LIBS_SHARED_BASE)/%.c=$(LIB_BIN)/%.o)
LIB_OBJ         += $(LIB_S_SHARED:$(LIBS_SHARED_BASE)/%.S=$(LIB_BIN)/%.o)

LIB_OBJ         += $(LIB_CPP_LOCAL:$(LIBS_LOCAL_BASE)/%.cpp=$(LIB_BIN)/%.o)
LIB_OBJ         += $(LIB_C_LOCAL:$(LIBS_LOCAL_BASE)/%.c=$(LIB_BIN)/%.o)
LIB_OBJ         += $(LIB_S_LOCAL:$(LIBS_LOCAL_BASE)/%.S=$(LIB_BIN)/%.o)

LIB_OBJ         += $(LIB_CPP_CORE:$(LIBS_CORE_BASE)/%.cpp=$(LIB_BIN)/%.o)
LIB_OBJ         += $(LIB_C_CORE:$(LIBS_CORE_BASE)/%.c=$(LIB_BIN)/%.o)
LIB_OBJ         += $(LIB_S_CORE:$(LIBS_CORE_BASE)/%.S=$(LIB_BIN)/%.o)

# Includes -------------------------------------------------------------
INCLUDE         := -I./$(USR_SRC) -I$(CORE_SRC)
INCLUDE         += $(foreach d, $(LIB_DIRS_SHARED), -I$d)
INCLUDE         += $(foreach d, $(LIB_DIRS_LOCAL), -I$d)
INCLUDE         += $(foreach d, $(LIB_DIRS_CORE), -I$d)

# Generate directories --------------------------------------------------------
DIRECTORIES     :=  $(sort $(dir $(CORE_OBJ) $(USR_OBJ) $(LIB_OBJ)))
generateDirs    := $(foreach d, $(DIRECTORIES), $(shell if [ ! -e "$d" ]; then mkdir -p "$d"; fi))

# $(info dirs: $(DIRECTORIES))
# $(info includes: $(INCLUDE))
# $(info LIB_OBJ: $(LIB_OBJ))

#******************************************************************************
# Rules:
#******************************************************************************

.PHONY: directories all rebuild upload uploadTy uploadCLI clean cleanUser cleanCore cleanLib

all:  $(TARGET_LST) $(TARGET_SYM) $(TARGET_HEX)

rebuild: cleanUser all

clean: cleanUser cleanCore cleanLib
	@$(QUIET)echo $(COL_OK)cleaning done$(COL_RESET)

upload: all
	@$(QUIET)echo $(COL_OK)Uploading to Teensy
	@echo -n $(COL_ERR)
	@$(UPL_PJRC) || :
	@echo -n $(COL_RESET)

uploadTy: all
	@$(UPL_TYCMD)

uploadCLI: all
	@$(UPL_CLICMD)

uploadJLink: all
	@$(UPL_JLINK)

# Core library ----------------------------------------------------------------
$(CORE_BIN)/%.o: $(CORE_SRC)/%.S
	@$(QUIET)echo $(COL_CORE)CORE [ASM] $(notdir $<)
	@echo -n $(COL_ERR)
	@"$(CC)" $(S_FLAGS) $(INCLUDE) -o $@ -c $<
	@echo -n $(COL_RESET)

$(CORE_BIN)/%.o: $(CORE_SRC)/%.c
	@$(QUIET)echo $(COL_CORE)CORE [CC]  $(notdir $<)
	@echo -n $(COL_ERR)
	@"$(CC)" $(C_FLAGS) $(INCLUDE) -o $@ -c $<
	@echo -n $(COL_RESET)

$(CORE_BIN)/%.o: $(CORE_SRC)/%.cpp
	@$(QUIET)echo $(COL_CORE)CORE [CPP] $(notdir $<)
	@echo -n $(COL_ERR)
	@"$(CXX)" $(CPP_FLAGS) $(INCLUDE) -o $@ -c $<
	@echo -n $(COL_RESET)

$(CORE_LIB) : $(CORE_OBJ)
	@$(QUIET)echo $(COL_LINK)CORE [AR] $@
	@echo -n $(COL_ERR)
	@$(AR) $(AR_FLAGS) $@ $^
	@$(QUIET)echo $(COL_OK)Teensy core built successfully
	@echo -n $(COL_RESET)

# Shared Libraries ------------------------------------------------------------
$(LIB_BIN)/%.o: $(LIBS_SHARED_BASE)/%.S
	@$(QUIET)echo $(COL_LIB)SHARED_LIB [ASM] $(notdir $<)
	@echo -n $(COL_ERR)
	@"$(CC)" $(S_FLAGS) $(INCLUDE) -o $@ -c $<
	@echo -n $(COL_RESET)

$(LIB_BIN)/%.o: $(LIBS_SHARED_BASE)/%.cpp
	@$(QUIET)echo $(COL_LIB)SHARED_LIB [CPP] $(notdir $<)
	@echo -n $(COL_ERR)
	@"$(CXX)" $(CPP_FLAGS) $(INCLUDE) -o $@ -c $<
	@echo -n $(COL_RESET)

$(LIB_BIN)/%.o: $(LIBS_SHARED_BASE)/%.c
	@$(QUIET)echo $(COL_LIB)SHARED_LIB [CC]  $(notdir $<)
	@echo -n $(COL_ERR)
	@"$(CC)" $(C_FLAGS) $(INCLUDE) -o $@ -c $<
	@echo -n $(COL_RESET)

# Local Libraries -------------------------------------------------------------
$(LIB_BIN)/%.o: $(LIBS_LOCAL_BASE)/%.S
	@$(QUIET)echo $(COL_LIB)LOCAL_LIB [ASM] $(notdir $<)
	@echo -n $(COL_ERR)
	@"$(CC)" $(S_FLAGS) $(INCLUDE) -o $@ -c $<
	@echo -n $(COL_RESET)

$(LIB_BIN)/%.o: $(LIBS_LOCAL_BASE)/%.cpp
	@$(QUIET)echo $(COL_LIB)LOCAL_LIB [CPP] $(notdir $<)
	@echo -n $(COL_ERR)
	@"$(CXX)" $(CPP_FLAGS) $(INCLUDE) -o $@ -c $<
	@echo -n $(COL_RESET)

$(LIB_BIN)/%.o: $(LIBS_LOCAL_BASE)/%.c
	@$(QUIET)echo $(COL_LIB)LOCAL_LIB [CC]  $(notdir $<)
	@echo -n $(COL_ERR)
	@"$(CC)" $(C_FLAGS) $(INCLUDE) -o $@ -c $<
	@echo -n $(COL_RESET)

# Core Libraries -------------------------------------------------------------
$(LIB_BIN)/%.o: $(LIBS_CORE_BASE)/%.S
	@$(QUIET)echo $(COL_LIB)CORE_LIB [ASM] $(notdir $<)
	@echo -n $(COL_ERR)
	@"$(CC)" $(S_FLAGS) $(INCLUDE) -o $@ -c $<
	@echo -n $(COL_RESET)

$(LIB_BIN)/%.o: $(LIBS_CORE_BASE)/%.cpp
	@$(QUIET)echo $(COL_LIB)CORE_LIB [CPP] $(notdir $<)
	@echo -n $(COL_ERR)
	@"$(CXX)" $(CPP_FLAGS) $(INCLUDE) -o $@ -c $<
	@echo -n $(COL_RESET)

$(LIB_BIN)/%.o: $(LIBS_CORE_BASE)/%.c
	@$(QUIET)echo $(COL_LIB)CORE_LIB [CC]  $(notdir $<)
	@echo -n $(COL_ERR)
	@"$(CC)" $(C_FLAGS) $(INCLUDE) -o $@ -c $<
	@echo -n $(COL_RESET)

# Handle user sources ---------------------------------------------------------
$(USR_BIN)/%.o: $(USR_SRC)/%.S
	@$(QUIET)echo $(COL_SRC)USER [ASM] $<
	@echo -n $(COL_ERR)
	@"$(CC)" $(S_FLAGS) $(INCLUDE) -o "$@" -c $<
	@echo -n $(COL_RESET)

$(USR_BIN)/%.o: $(USR_SRC)/%.c
	@$(QUIET)echo $(COL_SRC)USER [CC]  $(notdir $<)
	@echo -n $(COL_ERR)
	@"$(CC)" $(C_FLAGS) $(INCLUDE) -o "$@" -c $<
	@echo -n $(COL_RESET)

$(USR_BIN)/%.o: $(USR_SRC)/%.cpp
	@$(QUIET)echo $(COL_SRC)USER [CPP] $(notdir $<)
	@echo -n $(COL_ERR)
	@"$(CXX)" $(CPP_FLAGS) $(INCLUDE) -o "$@" -c $<
	@echo -n $(COL_RESET)

$(USR_BIN)/%.o: $(USR_SRC)/%.ino
	@$(QUIET)echo $(COL_SRC)USER [INO] $(notdir $<)
	@echo -n $(COL_ERR)
	@"$(CXX)" $(CPP_FLAGS) $(INCLUDE) -o "$@" -x c++ -c $<
	@echo -n $(COL_RESET)

# Linking ---------------------------------------------------------------------
$(TARGET_ELF): $(CORE_LIB) $(LIB_OBJ) $(USR_OBJ)
	@$(QUIET)echo $(COL_LINK)[LD]  $@
	@echo -n $(COL_RESET)
	@$(CC) $(LD_FLAGS) -o "$@" $(USR_OBJ) $(LIB_OBJ) $(CORE_LIB) $(LIBS)
	@$(QUIET)echo $(COL_OK)User code built and linked to libraries
	@echo -n $(COL_RESET)

%.lst: %.elf
	@$(QUIET)echo $(COL_LINK)[LST] $@
	@$(OBJDUMP) -d -S --demangle --no-show-raw-insn "$<" > "$@"
	@$(QUIET)echo $(COL_OK)Sucessfully built project
	@echo -n $(COL_RESET)

%.sym: %.elf
	@$(QUIET)echo $(COL_LINK)[SYM] $@
	@$(NM) $(NM_FLAGS) "$<" > "$@"
	@echo -n $(COL_RESET)

%.hex: %.elf
	@$(QUIET)echo $(COL_LINK)[HEX] $@
	@$(OBJCOPY) -O ihex -R.eeprom "$<" "$@"
	@echo -n $(COL_RESET)

# Cleaning --------------------------------------------------------------------
cleanUser:
	@$(QUIET)echo $(COL_LINK)Cleaning user binaries...
	@echo -n $(COL_ERR)
	@rm -rf "$(USR_BIN)" || :
	@rm -rf "$(TARGET_ELF)" || :
	@rm -rf "$(TARGET_HEX)" || :
	@rm -rf "$(TARGET_LST)" || :
	@rm -rf "$(TARGET_SYM)" || :
	@echo -n $(COL_RESET)

cleanCore:
	@$(QUIET)echo $(COL_LINK)Cleaning core binaries...$(COL_RESET)
	@if [ -e "$(CORE_BIN)" ]; then rm -rf "$(CORE_BIN)"; fi
	@if [ -e "$(CORE_LIB)" ]; then rm -rf "$(CORE_LIB)"; fi

cleanLib:
	@$(QUIET)echo $(COL_LINK)Cleaning user library binaries...$(COL_RESET)
	@if [ -e "$(LIB_BIN)" ]; then rm -rf "$(LIB_BIN)"; fi

# compiler generated dependency info ------------------------------------------
-include $(CORE_OBJ:.o=.d)
-include $(USR_OBJ:.o=.d)
-include $(LIB_OBJ:.o=.d)

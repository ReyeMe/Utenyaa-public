JO_ENGINE_SRC_DIR=src/jo_engine
COMPILER_DIR=Compiler

CMNDIR = $(COMPILER_DIR)/COMMON
IPFILE = $(CMNDIR)/IP.BIN
LDFILE = $(CMNDIR)/sgl.linker

BUILD_ELF = build.elf
BUILD_ISO = $(BUILD_ELF:.elf=.iso)
BUILD_CUE = $(BUILD_ELF:.elf=.cue)
BUILD_RAW = $(BUILD_ELF:.elf=.raw)
BUILD_MAP = $(BUILD_ELF:.elf=.map)

# Find all sources with ".c" extension.
SOURCES = $(patsubst ./%,%,$(shell find src/ -name '*.c'))
CXX_SOURCES = $(patsubst ./%,%,$(shell find src/ -name '*.cxx'))
OBJECTS = $(SOURCES:.c=.o) $(CXX_SOURCES:.cxx=.o)

SGLDIR = $(COMPILER_DIR)/COMMON/SGL_302j
SGLLDIR = $(SGLDIR)/LIB_ELF
SGLIDIR = $(SGLDIR)/inc

SYSOBJECTS = $(SGLLDIR)/SGLAREA.O

LIBS = $(SGLLDIR)/LIBCPK.A $(SGLLDIR)/LIBSND.A $(SGLLDIR)/SEGA_SYS.A $(SGLLDIR)/LIBCD.A $(SGLLDIR)/LIBSGL.A

CCFLAGS = -DJO_FRAMERATE=1\
		-DJO_COMPILE_USING_SGL\
		-DJO_COMPILE_WITH_FAST_BUT_LESS_ACCURATE_MATH\
		-DJO_COMPILE_WITH_TGA_SUPPORT\
		-DJO_COMPILE_WITH_3D_SUPPORT\
		-DJO_COMPILE_WITH_FS_SUPPORT\
		-DJO_COMPILE_WITH_EFFECTS_SUPPORT\
		-DJO_GLOBAL_MEMORY_SIZE_FOR_MALLOC=250000\
		-DJO_MAX_SPRITE=100\
		-DJO_MAX_FILE_IN_IMAGE_PACK=1\
		-DJO_MAP_MAX_LAYER=1\
		-DJO_MAX_SPRITE_ANIM=1\
		-DJO_MAX_FS_BACKGROUND_JOBS=1\
		-DJO_DEBUG\
		-DJO_COMPILE_WITH_PRINTF_SUPPORT\
		-DJO_COMPILE_WITH_SPRITE_HASHTABLE\
		-DJO_PAL_VERSION\
		-DENABLE_DEBUG\
		-DUTE_MAX_SPRITE=100\
		-W -m2 -c -O2 -Wno-strict-aliasing -I$(JO_ENGINE_SRC_DIR) -I$(SGLIDIR)

#\
		-DJO_COMPILE_WITH_RAM_CARD_SUPPORT\
		-DJO_COMPILE_WITH_PSEUDO_MODE7_SUPPORT\
		-DJO_COMPILE_WITH_KEYBOARD_SUPPORT\
		-DJO_COMPILE_WITH_CONSOLE_SUPPORT\
		-DJO_COMPILE_WITH_VIDEO_SUPPORT\
		-DJO_COMPILE_WITH_BACKUP_SUPPORT\
		-DJO_COMPILE_WITH_AUDIO_SUPPORT\
		-DJO_COMPILE_WITH_SOFTWARE_RENDERER_SUPPORT\
		-DJO_COMPILE_WITH_STORYBOARD_SUPPORT\
		-DJO_COMPILE_WITH_DUAL_CPU_SUPPORT\

LDFLAGS = -T$(LDFILE) -Wl,-Map,$(BUILD_MAP),-e,___Start -nostartfiles

ASSETS_DIR=./cd

%.o : %.c 
	sh2eb-elf-gcc.exe $< $(CCFLAGS) -std=c2x -o $@

%.o : %.cxx 
	sh2eb-elf-g++.exe $< $(CCFLAGS) -std=c++23 -fpermissive -fno-exceptions -fno-rtti -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-threadsafe-statics -fno-use-cxa-atexit -o $@

compile_objects : $(OBJECTS)
	sh2eb-elf-gcc.exe $(LDFLAGS) $(SYSOBJECTS) $(OBJECTS) $(LIBS) -o $(BUILD_ELF)
	
convert_binary : compile_objects
	sh2eb-elf-objcopy.exe -O binary $(BUILD_ELF) ./cd/0.bin

create_iso : convert_binary
	mkisofs -quiet -sysid "SEGA SATURN" -volid "SaturnApp" -volset "SaturnApp" -sectype 2352 \
	-publisher "SEGA ENTERPRISES, LTD." -preparer "SEGA ENTERPRISES, LTD." -appid "SaturnApp" \
	-abstract "$(ASSETS_DIR)/ABS.TXT" -copyright "$(ASSETS_DIR)/CPY.TXT" -biblio "$(ASSETS_DIR)/BIB.TXT" -generic-boot $(IPFILE) \
	-full-iso9660-filenames -o $(BUILD_ISO) $(ASSETS_DIR) $(ENTRYPOINT)

create_cue : create_iso
	JoEngineCueMaker.exe
	
clean:
	rm -f $(OBJECTS) $(BUILD_ELF) $(BUILD_ISO) $(BUILD_MAP) ./cd/0.bin

build : create_cue
	
all: clean build

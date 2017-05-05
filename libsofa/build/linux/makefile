#==============================================================================
#
#	@file		makefile
#	@brief		make file for libsofa
#	@author     Thibaut Carpentier
#	@date       18/07/2012
#
#==============================================================================

#==============================================================================
ifndef STRIP
	STRIP=strip
endif

ifndef AR
	AR=ar
endif

ifndef CONFIG
	CONFIG=Release
endif

#==============================================================================
# source files.
SRC = ../../src/SOFAAPI.cpp
SRC += ../../src/SOFAAttributes.cpp 
SRC += ../../src/SOFACoordinates.cpp 
SRC += ../../src/SOFADate.cpp 
SRC += ../../src/SOFAEmitter.cpp 
SRC += ../../src/SOFAExceptions.cpp 
SRC += ../../src/SOFAFile.cpp 
SRC += ../../src/SOFAHelper.cpp
SRC += ../../src/SOFAListener.cpp 
SRC += ../../src/SOFANcFile.cpp 
SRC += ../../src/SOFAPoint3.cpp 
SRC += ../../src/SOFAPosition.cpp 
SRC += ../../src/SOFAReceiver.cpp 
SRC += ../../src/SOFASimpleFreeFieldHRIR.cpp 
SRC += ../../src/SOFASimpleFreeFieldSOS.cpp
SRC += ../../src/SOFASimpleHeadphoneIR.cpp 
SRC += ../../src/SOFAMultiSpeakerBRIR.cpp 
SRC += ../../src/SOFASingleRoomDRIR.cpp 
SRC += ../../src/SOFAGeneralTF.cpp 
SRC += ../../src/SOFAGeneralFIR.cpp 
SRC += ../../src/SOFAGeneralFIRE.cpp 
SRC += ../../src/SOFASource.cpp 
SRC += ../../src/SOFAString.cpp 
SRC += ../../src/SOFAUnits.cpp


#==============================================================================
# compiler
#
# the -fpic option is required to properly build mex functions
#==============================================================================
CXX  = g++ 
CXX += -std=c++14 
CXX += -fpic 
CXX += -fvisibility=hidden 
CXX += -fvisibility-inlines-hidden

#==============================================================================		
ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
endif		
	
#==============================================================================
# object files
OBJECTS := $(SRC:.cpp=.o)
	
#==============================================================================
# header search paths
INCLUDES  = -I/usr/include
INCLUDES += -I../../dependencies/include
INCLUDES += -I../../src


#==============================================================================
# output		
OUTDIR	:= ../../lib
	
#==============================================================================
# RELEASE
#==============================================================================		
ifeq ($(CONFIG),Release)		
			
	#==============================================================================
	# output library
	TARGET  := libsofa.a	
				
	#==============================================================================
	# preprocessor macros
	LIBSOFA_MACROS  = -DNDEBUG=1
	LIBSOFA_MACROS += -DLINUX=1 

	#==============================================================================
	# Warning levels
	# NB : -Wno-attributes because we dont want many warning about visibility for template functions
	WARNING_CFLAGS  = -Wno-unknown-pragmas
	WARNING_CFLAGS += -Wno-reorder
	WARNING_CFLAGS += -Wno-unused-value
	WARNING_CFLAGS += -Wno-unused
	WARNING_CFLAGS += -Wno-attributes
	WARNING_CFLAGS += -Wno-multichar

	#==============================================================================
	# C++ compiler flags (-g -O2 -Wall)
	CCFLAGS  = $(LIBSOFA_MACROS)
	CCFLAGS += -g
	CCFLAGS += -O3
	CCFLAGS += $(WARNING_CFLAGS)

	#==============================================================================
	# library search paths
	LDFLAGS 	= --static -L../../../libsofa/dependencies/lib/linux

	#==============================================================================
	# linker flags
	LDLIBS	 	= -lstdc++ -lnetcdf_c++4 -lnetcdf -lhdf5_hl -lhdf5 -lcurl -lm -lz -ldl

endif


#==============================================================================
# DEBUG
#==============================================================================	
ifeq ($(CONFIG),Debug)
	#==============================================================================
	# output library
	TARGET  := libsofa_debug.a	
				
	#==============================================================================
	# preprocessor macros
	LIBSOFA_MACROS  = -DDEBUG=1
	LIBSOFA_MACROS += -DLINUX=1 

	#==============================================================================
	# Warning levels
	# NB : -Wno-attributes because we dont want many warning about visibility for template functions
	WARNING_CFLAGS  = -Wall

	#==============================================================================
	# C++ compiler flags (-g -O2 -Wall)
	CCFLAGS  = $(LIBSOFA_MACROS)
	CCFLAGS += -g
	CCFLAGS += -O0
	CCFLAGS += $(WARNING_CFLAGS)

	#==============================================================================
	# library search paths
	LDFLAGS 	= --static -L../../../libsofa/dependencies/lib/linux

	#==============================================================================
	# linker flags
	LDLIBS	 	= -lstdc++ -lnetcdf_c++4 -lnetcdf -lhdf5_hl -lhdf5 -lcurl -lm -lz -ldl
endif

#==============================================================================
# output file
OUTFILE := $(OUTDIR)/$(TARGET)


#==============================================================================
.PHONY: clean

all:    $(OUTFILE)
		@echo " "
		@echo  Build libsofa is OK !!
		@echo " "

$(OUTFILE): $(OBJECTS)
		$(AR) rcs $(OUTFILE) $(OBJECTS) 
	
# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file) 
# (see the gnu make manual section about automatic variables)
.cpp.o:
		@echo "\nCompiling file $< ..."
		$(CXX) $(CCFLAGS) $(INCLUDES) -o "$@" -c "$<"

clean:	
		@echo "\nCleaning..."
		$(RM) $(OBJECTS) *~ $(OUTFILE)

strip:
		@echo Stripping libsofa
		-@$(STRIP) --strip-unneeded $(OUTFILE)
		
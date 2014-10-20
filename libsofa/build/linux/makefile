#************************************************************************************
#
#	@file		makefile
#	@brief		make file for libsofa
#	@author     Thibaut Carpentier
#	@version    1.0.0
#	@date       18/07/2012
#
#************************************************************************************


#************************************************************************************
# source files.
SRC 		=  	../../src/SOFAAPI.cpp \
				../../src/SOFAAttributes.cpp \
				../../src/SOFACoordinates.cpp \
				../../src/SOFADate.cpp \
				../../src/SOFAEmitter.cpp \
				../../src/SOFAExceptions.cpp \
				../../src/SOFAFile.cpp \
				../../src/SOFAHelper.cpp \
				../../src/SOFAListener.cpp \
				../../src/SOFANcFile.cpp \
				../../src/SOFAPoint3.cpp \
				../../src/SOFAPosition.cpp \
				../../src/SOFAReceiver.cpp \
				../../src/SOFASimpleFreeFieldHRIR.cpp \
				../../src/SOFASource.cpp \
				../../src/SOFAString.cpp \
				../../src/SOFAUnits.cpp

#************************************************************************************
# where to build the .o files
BUILDDIR 	= .

#************************************************************************************
# object files
OBJ 		= 	$(SRC:.cpp=.o)

#************************************************************************************
# output library
OUT 		= 	../../lib/libsofa.a

#************************************************************************************
# header search paths
INCLUDES 	= 	-I/usr/include \
				-I../../src \
				-I../../dependencies/include
				
				
#************************************************************************************
# preprocessor macros
LIBSOFA_MACROS 		= 	-DNDEBUG=1 \
						-DLINUX=1 \
						-D__unix__=1 \
						-DHOST_ENDIAN_LITTLE=1

#************************************************************************************
# Warning levels
WARNING_CFLAGS	= -Wno-unknown-pragmas -Wno-unused-value -Wno-unused

#************************************************************************************
# C++ compiler flags (-g -O2 -Wall)
CCFLAGS 		= 	$(LIBSOFA_MACROS) \
					-g \
					-O3 \
					$(WARNING_CFLAGS)

#************************************************************************************
# compiler
#
# the -fpic option is required to properly build mex functions
CCC 		= g++ -fpic

#************************************************************************************
# library search paths
LDFLAGS 	= --static -L../../../libsofa/dependencies/lib/linux

#************************************************************************************
# linker flags
LDLIBS	 	= -lstdc++ -lnetcdf_c++4 -lnetcdf -lhdf5_hl -lhdf5 -lcurl -lm -lz -ldl

#************************************************************************************

.PHONY: depend clean

all:    $(OUT)
		@echo " "
		@echo  Build libsofa is OK !!
		@echo " "

$(OUT): $(OBJ)		
		ar rcs $(OUT) $(OBJ) 		
	
# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file) 
# (see the gnu make manual section about automatic variables)
.cpp.o:
		@echo " "
		@echo " "
		@echo "Compiling $< ..."
		@echo " "
		$(CCC) $(CCFLAGS) $(INCLUDES) -c $<  -o $@ 

clean:	
		@echo "Cleaning..."
		$(RM) $(OBJ) *~ $(OUT)
		@echo " "

depend: $(SRC)
		@echo "Generating dependencies for $^"
		makedepend $(INCLUDES) $^
		@echo " "

# DO NOT DELETE THIS LINE -- make depend needs it	
	
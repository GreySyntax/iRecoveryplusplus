CXXFLAGS =	-O2 -g -Wall -fmessage-length=0 -I./include
CXXALL = -lreadline
CXXCSS = -I"/usr/local/include" -I"/opt/local/include" -L"/usr/local/lib" -L"/opt/local/lib"
CXXWIN = -lusb -I"C:\MinGW\include" -L"C:\MinGW\lib"
CXXNIX = -lusb-1.0
CXXOSX = -lusb-1.0 -framework CoreFoundation -framework IOKit

DIRS =		../src ../include
OBJS =		Program.o USBDevice.o
TARGET =	../bin/iRecovery

EXTRA_FILES = ../Makefile ../src/Makefile
SOURCE := $(foreach DIR,$(DIRS),$(wildcard $(DIR)/*.cpp))
HEADERS := $(foreach DIR,$(DIRS),$(wildcard $(DIR)/*.h))

all:	
	@echo 'iRecovery++, requires you run make with a platform argument (win, linux, mach).'

start:
	@echo ''
	@echo 'Building iRecovery++'
	@echo ''

linux:	clean $(OBJS)
	@mkdir ../bin;
	$(CXX) -o $(TARGET) $(OBJS) $(CXXCSS) $(CXXALL) $(CXXNIX)
	
mach:	start clean $(OBJS)
	@mkdir ../bin;
	@$(CXX) -o $(TARGET) $(OBJS) $(CXXCSS) $(CXXALL) $(CXXOSX)

win:	clean $(OBJS)
	@mkdir ../bin;
	@$(CXX) -o $(TARGET) $(OBJS) $(CXXALL) $(CXXWIN)

backup:
	@if [ ! -e ../backup ]; then mkdir ../backup; fi;
	@zip ../backup/`date +%d-%m-%y_%H.%M`.zip $(SOURCE) $(HEADERS) $(EXTRA_FILES)
	@echo $(SOURCE)
clean:
	@rm *.o
	@rm -rf ../bin


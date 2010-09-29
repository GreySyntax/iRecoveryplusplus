CXXFLAGS =	-O2 -g -Wall -fmessage-length=0 -I./include
CXXALL =	-lreadline
CXXCSS =	-I"../include" -I"/usr/local/include" -I"/opt/local/include" -L"/usr/local/lib" -L"/opt/local/lib"
CXXWIN =	-lusb -I"C:\MinGW\include" -L"C:\MinGW\lib"
CXXNIX =	-lusb-1.0
CXXOSX =	-lusb-1.0 -framework CoreFoundation -framework IOKit

DIRS =		../src ../include
TARGET =	../bin/iRecovery

EXTRA_FILES = 	../Makefile ../src/Makefile
SOURCE :=	$(foreach DIR,$(DIRS),$(wildcard $(DIR)/*.cpp))
HEADERS :=	$(foreach DIR,$(DIRS),$(wildcard $(DIR)/*.h))

all:	
	@echo 'iRecovery++, requires you run make with a platform argument (win, linux, mach, backup).'

start:
	@echo ''
	@echo 'Building iRecovery++'
	@echo ''
	@rm -rf ../bin;

linux:	start $(SOURCE)
	@mkdir ../bin;
	$(CXX) -o $(TARGET) $(SOURCE) $(CXXCSS) $(CXXALL) $(CXXNIX)
	
mach:	start $(SOURCE)
	@mkdir ../bin;
	$(CXX) -o $(TARGET) $(SOURCE) $(CXXCSS) $(CXXALL) $(CXXOSX)
	@rm ../src/*.o

win:	start $(SOURCE)
	@mkdir ../bin;
	$(CXX) -o $(TARGET) $(SOURCE) $(CXXALL) $(CXXWIN)
	@rm ../src/*.o

backup:
	@echo $(SOURCE) $(HEADERS) $(EXTRA_FILES)
	@if [ ! -e ../backup ]; then mkdir ../backup; fi;
	@zip ../backup/`date +%d-%m-%y_%H.%M`.zip $(SOURCE) $(HEADERS) $(EXTRA_FILES)
	@echo $(SOURCE)

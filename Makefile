CXXFLAGS =	-O2 -g -Wall -fmessage-length=0 -I./include
CXXALL = -lreadline
CXXCSS = -I"/usr/local/include" -I"/opt/local/include" -L"/usr/local/lib" -L"/opt/local/lib"
CXXWIN = -lusb -I"C:\MinGW\include" -L"C:\MinGW\lib"
CXXNIX = -lusb-1.0
CXXOSX = -lusb-1.0 -framework CoreFoundation -framework IOKit

OBJS =		Program.o USBDevice.o

LIBS =

TARGET =	iRecovery

$(TARGET):	$(OBJS)
	$(CXX) -o bin/$(TARGET) src/$(OBJS) $(LIBS)

all:	
	@echo 'iRecovery++, requires you run make with a platform argument (win, linux, mach).'
	
linux:
	@echo 'Building iRecovery++ for linux.'
	$(TARGET) $(CXXCSS) $(CXXALL) $(CXXNIX)
	
mach:
	@echo 'Building iRecovery++ for Mac OS X.'
	$(TARGET) $(CXXCSS) $(CXXALL) $(CXXOSX)

win:
	@echo 'Building iRecovery++ for win32 (goodluck you need it)'
	$(TARGET) $(CXXWIN)

clean:
	rm -f $(OBJS) $(TARGET)

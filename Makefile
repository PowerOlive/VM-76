#==============================================================================
# ■ makefile
#==============================================================================

include inc.makefile

PROJECTS = VMDE VMGS

all: $(PROJECTS)

$(PROJECTS):
	cd $@ && $(MAKE) -f build/$(PLATFORM).makefile

clean:
ifeq "$(PLATFORM)" "msw"
	del /s *.o
	del VMGS\output.log
else
	find . -name "*.o" -print -delete
	$(RM) VMGS/output.log
endif

run: all
ifeq "$(PLATFORM)" "msw"
	call windows_path.bat && cd VMGS && VMGS.exe
else
	cd VMGS && ./VMGS
endif

debug:
	$(MAKE) $(PROJECTS) DEBUG=1
ifeq "$(PLATFORM)" "msw"
	call windows_path.bat && cd VMGS && gdb VMGS.exe
else
	cd VMGS && gdb VMGS
endif

platform:
	@echo $(PLATFORM)

.PHONY: all clean run debug $(PROJECTS) platform

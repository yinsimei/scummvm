MODULE := engines/sludge
 
MODULE_OBJS := \
	detection.o \
	sludge.o \
	console.o \
	builtin.o \
	moreio.o \
	variable.o \
	function.o \
	fileset.o \
	language.o
 
MODULE_DIRS += \
	engines/sludge
 
# This module can be built as a plugin
ifeq ($(ENABLE_SLUDGE), DYNAMIC_PLUGIN)
PLUGIN := 1
endif
 
# Include common rules 
include $(srcdir)/rules.mk
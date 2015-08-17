FILTER_OUT = $(wildcard makefile*) tags inc lib

ifndef SUBDIRS
	SUBDIRS = $(filter-out $(FILTER_OUT), $(wildcard *))
endif

.PHONY: all clean subdirs $(SUBDIRS)

all: $(SUBDIRS)

clean: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) $(MAKECMDGOALS) -C $@

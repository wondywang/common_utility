FILTER_OUT = $(wildcard makefile*) tags inc lib

ifndef SUBDIRS
	SUBDIRS = $(filter-out $(FILTER_OUT), $(shell find . -maxdepth 1 -type d ! -path .))
endif

.PHONY: all clean subdirs $(SUBDIRS)

all: $(SUBDIRS)

clean: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) $(MAKECMDGOALS) -C $@

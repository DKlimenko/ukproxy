UK_ROOT ?= $(PWD)/workdir/unikraft
UK_LIBS ?= $(PWD)/workdir/libs
UK_BUILD ?= $(PWD)/workdir/build
LIBS := $(UK_LIBS)/libcxxabi:$(UK_LIBS)/libcxx:$(UK_LIBS)/libunwind:$(UK_LIBS)/compiler-rt:$(UK_LIBS)/musl:$(UK_LIBS)/lwip:$(UK_LIBS)/openssl:$(UK_LIBS)/zlib:$(UK_LIBS)/usockets:$(UK_LIBS)/uwebsockets

all:
	@$(MAKE) -C $(UK_ROOT) A=$(PWD) L=$(LIBS) O=$(UK_BUILD)

$(MAKECMDGOALS):
	@$(MAKE) -C $(UK_ROOT) A=$(PWD) L=$(LIBS) O=$(UK_BUILD) $(MAKECMDGOALS)

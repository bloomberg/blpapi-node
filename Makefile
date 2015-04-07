
BIN_PREFIX := $(shell npm bin)
TSC := tsc --module commonjs --target ES5 --noImplicitAny --noEmitOnError --sourceMap

SRCS_TS += $(shell ls *.ts)
SRCS_JS += $(patsubst %.ts,%.js,$(SRCS_TS))

TSLINT := tslint

.PHONY: all build-typescript clean

all: build-typescript

clean:
	@rm -f $(SRCS_JS) $(patsubst %.js,%.js.map,$(SRCS_JS))

build-typescript:
	@$(TSC) $(SRCS_TS)
	@$(TSLINT) $(foreach file,$(SRCS_TS),-f $(file))

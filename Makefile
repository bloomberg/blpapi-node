
BIN_PREFIX := $(shell npm bin)
TSC := tsc --module commonjs --target ES5 --noImplicitAny --noEmitOnError --sourceMap

SRCS_TS += $(shell ls *.ts)
SRCS_JS += $(patsubst %.ts,%.js,$(SRCS_TS))

TSLINT := tslint

BLPAPIJS_DTS := blpapijs.d.ts
BLPAPIJS_DTS_SOURCE := ./typings/blpapijs/$(BLPAPIJS_DTS)
BLPAPIJS_DTS_TARGET := ./build/Release/$(BLPAPIJS_DTS)

.PHONY: all build-typescript clean

all: build-typescript

clean:
	@rm -f $(SRCS_JS) $(patsubst %.js,%.js.map,$(SRCS_JS)) $(BLPAPIJS_DTS_TARGET)

build-typescript: $(BLPAPIJS_DTS_TARGET)
	@$(TSC) $(SRCS_TS)
	@$(TSLINT) $(foreach file,$(SRCS_TS),-f $(file))

$(BLPAPIJS_DTS_TARGET): $(BLPAPIJS_DTS_SOURCE)
	@cp $(BLPAPIJS_DTS_SOURCE) $(BLPAPIJS_DTS_TARGET)

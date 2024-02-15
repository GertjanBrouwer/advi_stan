CMDSTAN := /home/gertjan/Documents/Thesis/cmdstan-2.33.1

CXXFLAGS := -I $(CMDSTAN)/stan/src
CXXFLAGS += -I $(CMDSTAN)/stan/lib/rapidjson_1.1.0
CXXFLAGS += -I $(CMDSTAN)/stan/lib/stan_math
CXXFLAGS += -I $(CMDSTAN)/stan/lib/stan_math/lib/eigen_3.4.0 
CXXFLAGS += -I $(CMDSTAN)/stan/lib/stan_math/lib/tbb
CXXFLAGS += -I $(CMDSTAN)/stan/lib/stan_math/lib/tbb_2020.3/include
CXXFLAGS += -I $(CMDSTAN)/stan/lib/stan_math/lib/sundials_6.1.1/include
CXXFLAGS += -I $(CMDSTAN)/stan/lib/stan_math/lib/boost_1.78.0
CXXFLAGS += -D_REENTRANT -DBOOST_DISABLE_ASSERTS

DEBUG ?= 0
ifeq ($(DEBUG), 1)
	CXXFLAGS += -g -O0
endif

LDFLAGS := $(CMDSTAN)/stan/lib/stan_math/lib/tbb/libtbb.so.2 -Wl,-rpath,$(CMDSTAN)/stan/lib/stan_math/lib/tbb

.PHONY: all clean

all: build/prototype

clean:
	rm -f build/prototype build/mean_model.hpp

build/mean_model.hpp: models/mean_model.stan | build
	$(CMDSTAN)/bin/stanc $< --o $@

build/prototype: prototype.cpp build/mean_model.hpp
	g++ $(CXXFLAGS) $< -o $@ $(LDFLAGS)

build:
	mkdir build

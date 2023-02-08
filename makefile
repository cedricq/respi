# ARM_GCC_BIN_FOLDER needs to be set as part of your environment variables 
# ARM_GCC_BIN_FOLDER=$(CURRENT_DIR)/../gcc-arm-none-eabi-9-2020-q2-update/bin

CURRENT_DIR=$(shell pwd)

# export ARM_GCC_BIN_FOLDER="/c/arm-gcc/11.3_rel1/bin"

ARM_TOOLCHAIN_FILE = $(CURRENT_DIR)/arm-none-eabi-gcc.cmake

define RUN_UNITTEST
	$(if $(ARG), cd $(1) && ctest -V -R $(ARG), cd $(1) && ctest --timeout 240)
endef

# Default config target                                                                                                                                                                                   
config: config-target

# Default build target
target: build-target

# Default tests
test: build-test
run:  run-test

# Default clean target                                                                                                                                                                                    
clean:
	rm -rf build

# Config and build for target
config-target: build/target/Makefile
build/target/Makefile:
	@echo ${CURRENT_DIR}
	@echo $(CURRENT_DIR)
	mkdir -p build/target
	cd build/target && cmake -G "Unix Makefiles" -DARM_TOOLCHAIN_DIR=${ARM_GCC_BIN_FOLDER} -DCMAKE_TOOLCHAIN_FILE=${ARM_TOOLCHAIN_FILE} -DCMAKE_BUILD_TYPE:STRING=debug $(CURRENT_DIR)

build-target: config-target
	cd build/target && $(MAKE) -j8 VERBOSE=1

clean-target:
	rm -rf build/target

        
# unit test config/build/run/clean targets                                                                                                                                                              
config-test: build/test/Makefile
build/test/Makefile:
	mkdir -p build/test
	cd build/test && cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE:STRING=debug -DTEST:BOOL=True $(CURRENT_DIR)

build-test: config-test
	cd build/test && $(MAKE) $(ARG) -j8

run-test: build-test
	$(call RUN_UNITTEST,build/test)

clean-test:
	rm -rf build/test

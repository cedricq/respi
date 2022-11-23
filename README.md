# respi

1. Set the environment variable `ARM_GCC_BIN_FOLDER` to where your `gcc-arm-none-eabi` binaries are located:

```
export ARM_GCC_BIN_FOLDER="/home/cedric/workspace_hw/gcc-arm-none-eabi-9-2020-q2-update/bin"
```

2. From the source root directory, build the cross-compiled binary for stm32 target (binaries will be available in the `build` generated folder:

```
make target
```

3. From the source root directory, build and run the unit tests using your standard local gcc (eg. `/usr/bin/cc`):

```
make test && make run
```
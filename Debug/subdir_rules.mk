################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
biterr.obj: ../biterr.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C6000 Compiler'
	"D:/anzhuanglujing/ccs/ccsv7/tools/compiler/c6000_7.4.24/bin/cl6x" -mv6740 --abi=eabi -g --include_path="D:/anzhuanglujing/ti/bios_c6sdk/pdk_C6748_1_0_0_1/packages/ti/pspiom/cslr" --include_path="D:/anzhuanglujing/ti/cls_C6000/include" --include_path="ti/pspiom/cslr/" --include_path="D:/anzhuanglujing/ti/bios_c6sdk/pdk_C6748_1_0_0_1/packages" --include_path="D:/ccs_workspace/OFDM" --include_path="D:/anzhuanglujing/ccs/ccsv7/tools/compiler/c6000_7.4.24/include" --define=CHIP_6410 --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="biterr.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

config.obj: ../config.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C6000 Compiler'
	"D:/anzhuanglujing/ccs/ccsv7/tools/compiler/c6000_7.4.24/bin/cl6x" -mv6740 --abi=eabi -g --include_path="D:/anzhuanglujing/ti/bios_c6sdk/pdk_C6748_1_0_0_1/packages/ti/pspiom/cslr" --include_path="D:/anzhuanglujing/ti/cls_C6000/include" --include_path="ti/pspiom/cslr/" --include_path="D:/anzhuanglujing/ti/bios_c6sdk/pdk_C6748_1_0_0_1/packages" --include_path="D:/ccs_workspace/OFDM" --include_path="D:/anzhuanglujing/ccs/ccsv7/tools/compiler/c6000_7.4.24/include" --define=CHIP_6410 --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="config.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

dm642main.obj: ../dm642main.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C6000 Compiler'
	"D:/anzhuanglujing/ccs/ccsv7/tools/compiler/c6000_7.4.24/bin/cl6x" -mv6740 --abi=eabi -g --include_path="D:/anzhuanglujing/ti/bios_c6sdk/pdk_C6748_1_0_0_1/packages/ti/pspiom/cslr" --include_path="D:/anzhuanglujing/ti/cls_C6000/include" --include_path="ti/pspiom/cslr/" --include_path="D:/anzhuanglujing/ti/bios_c6sdk/pdk_C6748_1_0_0_1/packages" --include_path="D:/ccs_workspace/OFDM" --include_path="D:/anzhuanglujing/ccs/ccsv7/tools/compiler/c6000_7.4.24/include" --define=CHIP_6410 --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="dm642main.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

ofdm.obj: ../ofdm.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C6000 Compiler'
	"D:/anzhuanglujing/ccs/ccsv7/tools/compiler/c6000_7.4.24/bin/cl6x" -mv6740 --abi=eabi -g --include_path="D:/anzhuanglujing/ti/bios_c6sdk/pdk_C6748_1_0_0_1/packages/ti/pspiom/cslr" --include_path="D:/anzhuanglujing/ti/cls_C6000/include" --include_path="ti/pspiom/cslr/" --include_path="D:/anzhuanglujing/ti/bios_c6sdk/pdk_C6748_1_0_0_1/packages" --include_path="D:/ccs_workspace/OFDM" --include_path="D:/anzhuanglujing/ccs/ccsv7/tools/compiler/c6000_7.4.24/include" --define=CHIP_6410 --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="ofdm.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

seeddm642_cpld.obj: ../seeddm642_cpld.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C6000 Compiler'
	"D:/anzhuanglujing/ccs/ccsv7/tools/compiler/c6000_7.4.24/bin/cl6x" -mv6740 --abi=eabi -g --include_path="D:/anzhuanglujing/ti/bios_c6sdk/pdk_C6748_1_0_0_1/packages/ti/pspiom/cslr" --include_path="D:/anzhuanglujing/ti/cls_C6000/include" --include_path="ti/pspiom/cslr/" --include_path="D:/anzhuanglujing/ti/bios_c6sdk/pdk_C6748_1_0_0_1/packages" --include_path="D:/ccs_workspace/OFDM" --include_path="D:/anzhuanglujing/ccs/ccsv7/tools/compiler/c6000_7.4.24/include" --define=CHIP_6410 --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="seeddm642_cpld.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

sync.obj: ../sync.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C6000 Compiler'
	"D:/anzhuanglujing/ccs/ccsv7/tools/compiler/c6000_7.4.24/bin/cl6x" -mv6740 --abi=eabi -g --include_path="D:/anzhuanglujing/ti/bios_c6sdk/pdk_C6748_1_0_0_1/packages/ti/pspiom/cslr" --include_path="D:/anzhuanglujing/ti/cls_C6000/include" --include_path="ti/pspiom/cslr/" --include_path="D:/anzhuanglujing/ti/bios_c6sdk/pdk_C6748_1_0_0_1/packages" --include_path="D:/ccs_workspace/OFDM" --include_path="D:/anzhuanglujing/ccs/ccsv7/tools/compiler/c6000_7.4.24/include" --define=CHIP_6410 --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="sync.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '



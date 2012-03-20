################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/dao/storagedisk.cpp \
../src/dao/storagememory.cpp 

OBJS += \
./src/dao/storagedisk.o \
./src/dao/storagememory.o 

CPP_DEPS += \
./src/dao/storagedisk.d \
./src/dao/storagememory.d 


# Each subdirectory must supply rules for building sources it contributes
src/dao/%.o: ../src/dao/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



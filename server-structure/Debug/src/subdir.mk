################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/client-run-commented.cpp \
../src/messagecontroler.cpp \
../src/server-run.cpp 

OBJS += \
./src/client-run-commented.o \
./src/messagecontroler.o \
./src/server-run.o 

CPP_DEPS += \
./src/client-run-commented.d \
./src/messagecontroler.d \
./src/server-run.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



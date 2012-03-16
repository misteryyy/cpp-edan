################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/messagemontroler.cpp \
../src/server-2.cpp 

CC_SRCS += \
../src/connection.cc \
../src/server.cc 

OBJS += \
./src/connection.o \
./src/messagemontroler.o \
./src/server-2.o \
./src/server.o 

CC_DEPS += \
./src/connection.d \
./src/server.d 

CPP_DEPS += \
./src/messagemontroler.d \
./src/server-2.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



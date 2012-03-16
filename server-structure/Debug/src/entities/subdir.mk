################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/entities/article.cpp \
../src/entities/newsgroup.cpp \
../src/entities/user.cpp 

OBJS += \
./src/entities/article.o \
./src/entities/newsgroup.o \
./src/entities/user.o 

CPP_DEPS += \
./src/entities/article.d \
./src/entities/newsgroup.d \
./src/entities/user.d 


# Each subdirectory must supply rules for building sources it contributes
src/entities/%.o: ../src/entities/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



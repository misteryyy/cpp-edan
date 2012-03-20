################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/messagecontroler.cpp \
../src/newsserver.cpp \
../src/server-2.cpp 

CC_SRCS += \
../src/connection.cc \
../src/server.cc 

OBJS += \
./src/connection.o \
./src/messagecontroler.o \
./src/newsserver.o \
./src/server-2.o \
./src/server.o 

CC_DEPS += \
./src/connection.d \
./src/server.d 

CPP_DEPS += \
./src/messagecontroler.d \
./src/newsserver.d \
./src/server-2.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"C:\MinGW\bin\" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"C:\MinGW\bin\" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/newsserver.o: ../src/newsserver.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"C:\MinGW\bin\" -I"C:\Program Files (x86)\PHP\ C:\MinGW\bin\ C:\Program Files (x86)\NVIDIA Corporation\PhysX\Common C:\Windows\system32 C:\Windows C:\Windows\System32\Wbem C:\Windows\System32\WindowsPowerShell\v1.0\ C:\Program Files\WIDCOMM\Bluetooth Software\ C:\Program Files\WIDCOMM\Bluetooth Software\syswow64" -I"C:\MinGW\lib\gcc\mingw32\4.6.2\include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"src/newsserver.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



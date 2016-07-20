################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../encrypt.o \
../ex2.o 

CPP_SRCS += \
../encrypt.cpp \
../ex2.cpp 

OBJS += \
./encrypt.o \
./ex2.o 

CPP_DEPS += \
./encrypt.d \
./ex2.d 


# Each subdirectory must supply rules for building sources it contributes
encrypt.o: ../encrypt.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -Iusr/include -Iusr/local/include/cryptopp -Iusr/local/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"encrypt.d" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



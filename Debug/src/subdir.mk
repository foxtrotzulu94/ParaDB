################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/db.c \
../src/even.c \
../src/menu.c \
../src/odd.c 

OBJS += \
./src/db.o \
./src/even.o \
./src/menu.o \
./src/odd.o 

C_DEPS += \
./src/db.d \
./src/even.d \
./src/menu.d \
./src/odd.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	mpicc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



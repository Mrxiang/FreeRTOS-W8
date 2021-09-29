C_SRCS += $(PROJ_ROOT)/src/main.c
C_SRCS += $(PROJ_ROOT)/src/uart5.c
C_SRCS += $(PROJ_ROOT)/src/uart8.c
C_SRCS += $(PROJ_ROOT)/src/power.c
C_SRCS += $(PROJ_ROOT)/src/commen.c
C_SRCS += $(PROJ_ROOT)/src/uart_driver.c

OBJS += $(BUILD_TMP)/main.o
OBJS += $(BUILD_TMP)/uart5.o
OBJS += $(BUILD_TMP)/uart8.o
OBJS += $(BUILD_TMP)/power.o
OBJS += $(BUILD_TMP)/commen.o
OBJS += $(BUILD_TMP)/uart_driver.o

C_DEPS += $(BUILD_TMP)/main.d
C_DEPS += $(BUILD_TMP)/uart5.d
C_DEPS += $(BUILD_TMP)/uart8.d
C_DEPS += $(BUILD_TMP)/power.d
C_DEPS += $(BUILD_TMP)/commen.d
C_DEPS += $(BUILD_TMP)/uart_driver.d

# Each subdirectory must supply rules for building sources it contributes
$(BUILD_TMP)/%.o: $(PROJ_ROOT)/src/%.c
	@echo 'Building file: $<'
	gcc -DUSE_STDIO=1 -D__GCC_POSIX__=1 $(TARGET_INC) -O2 -Wall -c -fmessage-length=0 -pthread -lrt -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
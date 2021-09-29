C_SRCS += $(PROJ_ROOT)/src/main.c
C_SRCS += $(PROJ_ROOT)/src/uart5.c
C_SRCS += $(PROJ_ROOT)/src/uart8.c

OBJS += $(BUILD_TMP)/main.o
OBJS += $(BUILD_TMP)/uart5.o
OBJS += $(BUILD_TMP)/uart8.o

C_DEPS += $(BUILD_TMP)/main.d
C_DEPS += $(BUILD_TMP)/uart5.d
C_DEPS += $(BUILD_TMP)/uart8.d

# Each subdirectory must supply rules for building sources it contributes
$(BUILD_TMP)/%.o: $(PROJ_ROOT)/src/%.c
	@echo 'Building file: $<'
	gcc -DUSE_STDIO=1 -D__GCC_POSIX__=1 $(TARGET_INC) -O2 -Wall -c -fmessage-length=0 -pthread -lrt -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
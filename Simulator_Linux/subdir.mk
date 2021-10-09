C_SRCS += $(PROJ_ROOT)/src/main.c
C_SRCS += $(PROJ_ROOT)/src/uart5.c
C_SRCS += $(PROJ_ROOT)/src/uart8.c
C_SRCS += $(PROJ_ROOT)/src/mcu.c
C_SRCS += $(PROJ_ROOT)/src/commen.c
C_SRCS += $(PROJ_ROOT)/src/interlayer.c
C_SRCS += $(PROJ_ROOT)/src/uart_driver.c
C_SRCS += $(PROJ_ROOT)/src/cjson/cJSON.c
C_SRCS += $(PROJ_ROOT)/src/cjson/cJSON_Utils.c



OBJS += $(BUILD_TMP)/main.o
OBJS += $(BUILD_TMP)/uart5.o
OBJS += $(BUILD_TMP)/uart8.o
OBJS += $(BUILD_TMP)/mcu.o
OBJS += $(BUILD_TMP)/commen.o
OBJS += $(BUILD_TMP)/interlayer.o
OBJS += $(BUILD_TMP)/uart_driver.o
OBJS += $(BUILD_TMP)/cJSON.o
OBJS += $(BUILD_TMP)/cJSON_Utils.o


C_DEPS += $(BUILD_TMP)/main.d
C_DEPS += $(BUILD_TMP)/uart5.d
C_DEPS += $(BUILD_TMP)/uart8.d
C_DEPS += $(BUILD_TMP)/mcu.d
C_DEPS += $(BUILD_TMP)/commen.d
C_DEPS += $(BUILD_TMP)/interlayer.d
C_DEPS += $(BUILD_TMP)/uart_driver.d
C_DEPS += $(BUILD_TMP)/cJSON.d
C_DEPS += $(BUILD_TMP)/cJSON_Utils.d
#OBJS= $(C_SRCS:.cpp=.o)
#C_DEPS= $(OBJS:.o=.d)
#$@--目标文件，$^--所有的依赖文件，$<--第一个依赖文件。
# Each subdirectory must supply rules for building sources it contributes
$(BUILD_TMP)/%.o: $(PROJ_ROOT)/src/%.c
	@echo 'Building file: $<'
	gcc -DUSE_STDIO=1 -D__GCC_POSIX__=1 $(TARGET_INC) -O2 -Wall -c -fmessage-length=0 -pthread -lrt -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(BUILD_TMP)/%.o: $(PROJ_ROOT)/src/cjson/%.c
	@echo 'Building file: $<'
	gcc -DUSE_STDIO=1 -D__GCC_POSIX__=1 $(TARGET_INC) -O2 -Wall -c -fmessage-length=0 -pthread -lrt -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

# FreeRTOS
RTOS_PATH = $(CORE_DIR)/RTOS
include $(RTOS_PATH)/RTOS.mk


C_SOURCES += \
$(wildcard $(CORE_DIR)/Diagnostic/*.c)

DEFS += \
-DTRUE=1 \
-DNULL=0
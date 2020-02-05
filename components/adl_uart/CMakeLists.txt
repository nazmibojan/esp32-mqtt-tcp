set(COMPONENT_SRCS "adl_uart.c")
set(COMPONENT_ADD_INCLUDEDIRS "include" "../adl_mqtt/include")
set(COMPONENT_REQUIRES mqtt)
set(COMPONENT_PRIV_REQUIRES nvs_flash)

register_component()
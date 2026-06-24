zephyr_library()
zephyr_library_sources_ifdef(
    CONFIG_INPUT_SYNAPTICS_RMI_I2C
    input_synaptics_rmi_i2c.c
)

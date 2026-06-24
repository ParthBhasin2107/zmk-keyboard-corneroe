#define DT_DRV_COMPAT parth_synaptics_rmi_i2c

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/input/input.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(synaptics_rmi_i2c, CONFIG_INPUT_LOG_LEVEL);

struct synaptics_config {
    struct i2c_dt_spec i2c;
    uint32_t poll_interval_ms;
};

struct synaptics_data {
    const struct device *dev;
    struct k_work_delayable poll_work;
    uint8_t last_buttons;
};

static void synaptics_poll(struct k_work *work) {
    struct k_work_delayable *dwork = k_work_delayable_from_work(work);
    struct synaptics_data *data =
        CONTAINER_OF(dwork, struct synaptics_data, poll_work);
    const struct device *dev = data->dev;
    const struct synaptics_config *cfg = dev->config;

    uint8_t buf[16];
    uint8_t cmd = 0x00;

    int ret = i2c_write_read_dt(&cfg->i2c, &cmd, 1, buf, sizeof(buf));
    if (ret == 0) {
        uint8_t buttons = buf[3];
        int8_t dx = (int8_t)buf[4];
        int8_t dy = -(int8_t)buf[5];

        if (dx != 0) {
            input_report_rel(dev, INPUT_REL_X, dx, false, K_NO_WAIT);
        }
        if (dy != 0) {
            input_report_rel(dev, INPUT_REL_Y, dy, false, K_NO_WAIT);
        }
        if (buttons != data->last_buttons) {
            input_report_key(dev, INPUT_BTN_0,
                buttons & 0x01, false, K_NO_WAIT);
            data->last_buttons = buttons;
        }
        if (dx != 0 || dy != 0 || buttons != data->last_buttons) {
            input_report(dev, 0, 0, 0, true, K_NO_WAIT);
        }
    }

    k_work_schedule(&data->poll_work,
        K_MSEC(cfg->poll_interval_ms));
}

static int synaptics_init(const struct device *dev) {
    struct synaptics_data *data = dev->data;
    const struct synaptics_config *cfg = dev->config;

    if (!i2c_is_ready_dt(&cfg->i2c)) {
        LOG_ERR("I2C bus not ready");
        return -ENODEV;
    }

    data->dev = dev;
    data->last_buttons = 0;

    k_work_init_delayable(&data->poll_work, synaptics_poll);
    k_work_schedule(&data->poll_work, K_MSEC(500));

    LOG_INF("Synaptics RMI I2C touchpad initialized at %s",
        dev->name);
    return 0;
}

#define SYNAPTICS_INIT(n)                                           \
    static struct synaptics_data synaptics_data_##n;               \
    static const struct synaptics_config synaptics_config_##n = {  \
        .i2c = I2C_DT_SPEC_INST_GET(n),                           \
        .poll_interval_ms = DT_INST_PROP(n, poll_interval_ms),    \
    };                                                             \
    DEVICE_DT_INST_DEFINE(n, synaptics_init, NULL,                 \
        &synaptics_data_##n,                                       \
        &synaptics_config_##n,                                     \
        POST_KERNEL, CONFIG_INPUT_INIT_PRIORITY, NULL);

DT_INST_FOREACH_STATUS_OKAY(SYNAPTICS_INIT)

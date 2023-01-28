#ifndef MLEKM_FAN_MODE_H
#define MLEKM_FAN_MODE_H

#include <linux/kobject.h>

#include "constans.h"
#include "global_store.h"
#include "utils.h"

static ssize_t mlekm_fan_mode_show(struct kobject* obj, struct kobj_attribute* attr, char* buf)
{
    MLEKM_READ_EC(fan_mode_value, MLEKM_FAN_MODE_LEN, config->addrs.fan_mode);

    for (int i = 0; i < config->values.fan_modes.size; ++i) {
        if (config->values.fan_modes.data[i].value == fan_mode_value[0]) {
            return sprintf(buf, ("%s\n"), mlekm_fan_mode_to_string(config->values.fan_modes.data[i].mode));
        }
    }

    return sprintf(buf, "unknown\n");
}

static ssize_t mlekm_fan_mode_store(struct kobject* obj, struct kobj_attribute* attr, const char* buf, size_t size)
{
    if (config->ec_specified == false) {
        pr_warn("ec is unspecified\n");
        return -EROFS;
    }

    enum MLEKM_FAN_MODE fan_mode = mlekm_string_to_fan_mode(buf);
    if (fan_mode == MLEKM_FAN_MODE_UNKNOWN) {
        return -EINVAL;
    }

    int res = 0;
    for (int i = 0; i < config->values.fan_modes.size; ++i) {
        if (config->values.fan_modes.data[i].mode == fan_mode) {
            MLEKM_WRITE_EC(&config->values.fan_modes.data[i].value, MLEKM_FAN_MODE_LEN, config->addrs.fan_mode);
            break;
        }
    }

    return size;
}

static struct kobj_attribute mlekm_fan_mode_attribute = {
    .attr = { .name = "fan_mode", .mode = 0644 },
    .show = mlekm_fan_mode_show,
    .store = mlekm_fan_mode_store,
};

#endif // MLEKM_FAN_MODE_H
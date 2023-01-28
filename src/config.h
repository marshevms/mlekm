#ifndef MLEKM_CONFIG_H
#define MLEKM_CONFIG_H

#include <asm-generic/int-ll64.h>
#include <linux/slab.h>

#include "constans.h"

struct mlekm_config_fan_mode_default_value {
    u8 value;
    enum MLEKM_FAN_MODE mode;
};

struct mlekm_config_fan_mode_default_values_array {
    struct mlekm_config_fan_mode_default_value* data;
    u8 size;
};

struct mlekm_config_cooler_boost_status_default_value {
    u8 value;
    enum MLEKM_COOLER_BOOST_STATUS status;
};

struct mlekm_config_cooler_boost_status_default_values_array {
    struct mlekm_config_cooler_boost_status_default_value* data;
    u8 size;
};

struct mlekm_config_pu_default_values_array {
    u8* data;
    u8 size;
};

struct mlekm_config_pu_default_values {
    struct mlekm_config_pu_default_values_array temps;
    struct mlekm_config_pu_default_values_array fan_speeds;
};

struct mlekm_config_pu_addresses {
    u8 realtime_temp;
    u8 realtime_fan_speed;
    u8 realtime_fan_rpm;
    u8 temps;
    u8 fan_speeds;
};

struct mlekm_config_addresses {
    struct mlekm_config_pu_addresses cpu;
    struct mlekm_config_pu_addresses gpu;
    u8 fan_mode;
    u8 cooler_boost;
};

struct mlekm_config_default_values {
    struct mlekm_config_pu_default_values cpu;
    struct mlekm_config_pu_default_values gpu;
    struct mlekm_config_fan_mode_default_values_array fan_modes;
    struct mlekm_config_cooler_boost_status_default_values_array cooler_boost_statuses;
};

struct mlekm_config {
    bool ec_specified;
    struct mlekm_config_addresses addrs;
    struct mlekm_config_default_values values;
};

static struct mlekm_config* mlekm_config_init_default(void)
{
    struct mlekm_config_fan_mode_default_value* fan_modes = NULL;
    struct mlekm_config_cooler_boost_status_default_value* cooler_boost_statuses = NULL;
    struct mlekm_config* config = NULL;

    fan_modes = kmalloc_array(3, sizeof(struct mlekm_config_fan_mode_default_value), GFP_KERNEL);
    if (fan_modes == NULL) {
        return (void*)fan_modes;
    }

    cooler_boost_statuses = kmalloc_array(2, sizeof(struct mlekm_config_cooler_boost_status_default_value), GFP_KERNEL);
    if (cooler_boost_statuses == NULL) {
        kfree(fan_modes);
        return (void*)cooler_boost_statuses;
    }

    config = kmalloc(sizeof(struct mlekm_config), GFP_KERNEL);
    if (config == NULL) {
        kfree(fan_modes);
        kfree(cooler_boost_statuses);
        return config;
    }

    fan_modes[0].value = 0x0C;
    fan_modes[1].value = 0x4C;
    fan_modes[2].value = 0x8C;

    fan_modes[0].mode = MLEKM_FAN_MODE_AUTO;
    fan_modes[1].mode = MLEKM_FAN_MODE_BASIC;
    fan_modes[2].mode = MLEKM_FAN_MODE_ADVANCED;

    cooler_boost_statuses[0].value = 0x82;
    cooler_boost_statuses[1].value = 0x02;

    cooler_boost_statuses[0].status = MLEKM_COOLER_BOOST_STATUS_ONN;
    cooler_boost_statuses[1].status = MLEKM_COOLER_BOOST_STATUS_OFF;

    struct mlekm_config temp = {
        .ec_specified = false,
        .addrs = {
            .cpu = {
                .realtime_temp = 0x68,
                .realtime_fan_speed = 0x71,
                .realtime_fan_rpm = 0xCC,
                .temps = 0x6A,
                .fan_speeds = 0x72,
            },
            .gpu = {
                .realtime_temp = 0x80,
                .realtime_fan_speed = 0x89,
                .realtime_fan_rpm = 0xCA,
                .temps = 0x82,
                .fan_speeds = 0x8A,
            },
            .fan_mode = 0xF4,
            .cooler_boost = 0x98,
        },
        .values = {
            .cpu = {
                .temps = {
                    .data = NULL,
                    .size = 0,
                },
                .fan_speeds = {
                    .data = NULL,
                    .size = 0,
                },
            },
            .gpu = {
                .temps = {
                    .data = NULL,
                    .size = 0,
                },
                .fan_speeds = {
                    .data = NULL,
                    .size = 0,
                },
            },
            .fan_modes = {
                .data = fan_modes,
                .size = 3,
            },
            .cooler_boost_statuses = {
                .data = cooler_boost_statuses,
                .size = 2,
            },
        },
    };

    *config = temp;
    return config;
}

static void mlekm_config_free(const struct mlekm_config* config)
{
    if (config == NULL) {
        return;
    }

    if (config->values.cpu.temps.data != NULL) {
        kfree(config->values.cpu.temps.data);
    }
    if (config->values.cpu.fan_speeds.data != NULL) {
        kfree(config->values.cpu.fan_speeds.data);
    }

    if (config->values.gpu.temps.data != NULL) {
        kfree(config->values.gpu.temps.data);
    }
    if (config->values.gpu.fan_speeds.data != NULL) {
        kfree(config->values.gpu.fan_speeds.data);
    }

    if (config->values.cooler_boost_statuses.data != NULL) {
        kfree(config->values.cooler_boost_statuses.data);
    }

    if (config->values.fan_modes.data != NULL) {
        kfree(config->values.fan_modes.data);
    }

    kfree(config);
}

#endif // MLEKM_CONFIG_H
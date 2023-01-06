#ifndef MLEKM_CONFIG_16J5EMS3_108_H
#define MLEKM_CONFIG_16J5EMS3_108_H

#include "../config.h"

static const struct mlekm_config* mlekm_config_16j5ems3_108_init(void)
{
    u8 cpu_temps[] = { 55, 61, 67, 73, 79, 79 };
    u8 cpu_fan_speeds[] = { 47, 50, 56, 64, 72, 80, 80 };
    u8 gpu_temps[] = { 55, 60, 65, 70, 75, 75 };
    u8 gpu_fan_speeds[] = { 0, 53, 57, 61, 65, 69, 69 };

    struct mlekm_config* res = mlekm_config_init_default();
    if (res == NULL) {
        return res;
    }

    res->values.cpu.temps.data = kmalloc(sizeof(cpu_temps), GFP_KERNEL);
    res->values.cpu.fan_speeds.data = kmalloc(sizeof(cpu_fan_speeds), GFP_KERNEL);
    res->values.gpu.temps.data = kmalloc(sizeof(gpu_temps), GFP_KERNEL);
    res->values.gpu.fan_speeds.data = kmalloc(sizeof(gpu_fan_speeds), GFP_KERNEL);

    if (res->values.cpu.temps.data == NULL || res->values.cpu.fan_speeds.data == NULL || res->values.gpu.temps.data == NULL || res->values.gpu.fan_speeds.data == NULL) {
        mlekm_config_free(res);
        return NULL;
    }

    memcpy(res->values.cpu.temps.data, cpu_temps, sizeof(cpu_temps));
    memcpy(res->values.cpu.fan_speeds.data, cpu_fan_speeds, sizeof(cpu_fan_speeds));
    memcpy(res->values.gpu.temps.data, gpu_temps, sizeof(gpu_temps));
    memcpy(res->values.gpu.fan_speeds.data, gpu_fan_speeds, sizeof(gpu_fan_speeds));

    res->values.cpu.temps.size = ARRAY_SIZE(cpu_temps);
    res->values.cpu.fan_speeds.size = ARRAY_SIZE(cpu_fan_speeds);
    res->values.gpu.temps.size = ARRAY_SIZE(gpu_temps);
    res->values.gpu.fan_speeds.size = ARRAY_SIZE(gpu_fan_speeds);

    res->ec_specified = true;
    return res;
}

#endif // MLEKM_CONFIG_16J5EMS3_108_H
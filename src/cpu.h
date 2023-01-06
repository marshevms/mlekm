#ifndef MLEKM_CPU_H
#define MLEKM_CPU_H

#include <linux/kobject.h>

#include "constans.h"
#include "global_store.h"
#include "utils.h"

static ssize_t mlekm_cpu_realtime_temp_show(struct kobject* obj, struct kobj_attribute* attr, char* buf)
{
    MLEKM_READ_EC(temp, MLEKM_CPU_REALTIME_TEMP_LEN, config->addrs.cpu.realtime_temp);

    return sprintf(buf, "%d\n", temp[0]);
}

static ssize_t mlekm_cpu_realtime_fan_speed_show(struct kobject* obj, struct kobj_attribute* attr, char* buf)
{
    MLEKM_READ_EC(temp, MLEKM_CPU_REALTIME_FAN_SPEED_LEN, config->addrs.cpu.realtime_fan_speed);

    return sprintf(buf, "%d\n", temp[0]);
}

static ssize_t mlekm_cpu_realtime_fan_rpm_show(struct kobject* obj, struct kobj_attribute* attr, char* buf)
{
    MLEKM_READ_EC(temp, MLEKM_CPU_REALTIME_FAN_RPM_LEN, config->addrs.cpu.realtime_fan_rpm);

    int rpm = ((0 | temp[0]) << 8) | temp[1];
    if (rpm < 0) {
        return sprintf(buf, "0\n");
    }

    return sprintf(buf, "%d\n", MLEKM_FAN_RPM_DIVIDEND / rpm);
}

static ssize_t mlekm_cpu_temps_show(struct kobject* obj, struct kobj_attribute* attr, char* buf)
{
    MLEKM_READ_EC(temp, MLEKM_CPU_TEMPS_LEN, config->addrs.cpu.temps);

    return mlekm_to_array(temp, MLEKM_CPU_TEMPS_LEN, buf);
}

static ssize_t mlekm_cpu_temps_store(struct kobject* obj, struct kobj_attribute* attr, const char* buf, size_t size)
{
    if (size == 0) {
        return size;
    }

    if (config->ec_specified == false) {
        pr_warn("ec is unspecified\n");
        return -EROFS;
    }

    u8 cpu_temps[MLEKM_CPU_TEMPS_LEN] = { 0 };

    int res = mlekm_string_to_array(buf, size, cpu_temps, ARRAY_SIZE(cpu_temps));
    if (res < 0) {
        pr_err("failed to convert string to cpu temp values\n");
        return res;
    }

    if (mlekm_check_values(cpu_temps, ARRAY_SIZE(cpu_temps)) == false) {
        pr_err("wrong cpu temp values\n");
        return -EINVAL;
    }

    {
        MLEKM_WRITE_EC(cpu_temps, ARRAY_SIZE(cpu_temps), config->addrs.cpu.temps);
    }

    return size;
}

static ssize_t mlekm_cpu_fan_speeds_show(struct kobject* obj, struct kobj_attribute* attr, char* buf)
{
    MLEKM_READ_EC(temp, MLEKM_CPU_FAN_SPEEDS_LEN, config->addrs.cpu.fan_speeds);

    return mlekm_to_array(temp, MLEKM_CPU_FAN_SPEEDS_LEN, buf);
}

static ssize_t mlekm_cpu_fan_speeds_store(struct kobject* obj, struct kobj_attribute* attr, const char* buf, size_t size)
{
    if (size == 0) {
        return size;
    }

    if (config->ec_specified == false) {
        pr_warn("ec is unspecified\n");
        return -EROFS;
    }

    u8 cpu_fan_speeds[MLEKM_CPU_FAN_SPEEDS_LEN] = { 0 };

    int res = mlekm_string_to_array(buf, size, cpu_fan_speeds, ARRAY_SIZE(cpu_fan_speeds));
    if (res < 0) {
        pr_err("failed to convert string to cpu fan speed values\n");
        return res;
    }

    if (mlekm_check_values(cpu_fan_speeds, ARRAY_SIZE(cpu_fan_speeds)) == false) {
        pr_err("wrong cpu fan speed values\n");
        return -EINVAL;
    }

    {
        MLEKM_WRITE_EC(cpu_fan_speeds, ARRAY_SIZE(cpu_fan_speeds), config->addrs.cpu.fan_speeds);
    }

    return size;
}

static ssize_t mlekm_cpu_default_temps_show(struct kobject* obj, struct kobj_attribute* attr, char* buf)
{
    if (config->ec_specified == false) {
        pr_warn("ec is unspecified\n");
        return -EROFS;
    }

    return mlekm_to_array(config->values.cpu.temps.data, config->values.cpu.temps.size, buf);
}

static ssize_t mlekm_cpu_default_fan_speeds_show(struct kobject* obj, struct kobj_attribute* attr, char* buf)
{
    if (config->ec_specified == false) {
        pr_warn("ec is unspecified\n");
        return -EROFS;
    }

    return mlekm_to_array(config->values.cpu.fan_speeds.data, config->values.cpu.fan_speeds.size, buf);
}

static struct kobj_attribute mlekm_cpu_realtime_temp_attribute = {
    .attr = { .name = "realtime_temp", .mode = 0444 },
    .show = mlekm_cpu_realtime_temp_show,
};
static struct kobj_attribute mlekm_cpu_realtime_fan_speed_attribute = {
    .attr = { .name = "realtime_fan_speed", .mode = 0444 },
    .show = mlekm_cpu_realtime_fan_speed_show,
};
static struct kobj_attribute mlekm_cpu_realtime_fan_rpm_attribute = {
    .attr = { .name = "realtime_fan_rpm", .mode = 0444 },
    .show = mlekm_cpu_realtime_fan_rpm_show,
};
static struct kobj_attribute mlekm_cpu_temps_attribute = {
    .attr = { .name = "temps", .mode = 0644 },
    .show = mlekm_cpu_temps_show,
    .store = mlekm_cpu_temps_store,
};
static struct kobj_attribute mlekm_cpu_fan_speeds_attribute = {
    .attr = { .name = "fan_speeds", .mode = 0644 },
    .show = mlekm_cpu_fan_speeds_show,
    .store = mlekm_cpu_fan_speeds_store,
};

static struct kobj_attribute mlekm_cpu_default_temps_attribute = {
    .attr = { .name = "default_temps", .mode = 0444 },
    .show = mlekm_cpu_default_temps_show,
};

static struct kobj_attribute mlekm_cpu_default_fan_speeds_attribute = {
    .attr = { .name = "default_fan_speeds", .mode = 0444 },
    .show = mlekm_cpu_default_fan_speeds_show,
};

static struct attribute* mlekm_cpu_attributes[] = {
    &mlekm_cpu_realtime_temp_attribute.attr,
    &mlekm_cpu_realtime_fan_speed_attribute.attr,
    &mlekm_cpu_realtime_fan_rpm_attribute.attr,
    &mlekm_cpu_temps_attribute.attr,
    &mlekm_cpu_fan_speeds_attribute.attr,
    &mlekm_cpu_default_temps_attribute.attr,
    &mlekm_cpu_default_fan_speeds_attribute.attr,
    NULL,
};

static struct attribute_group mlekm_cpu_group = {
    .name = "cpu",
    .attrs = mlekm_cpu_attributes,
};

#endif // MLEKM_CPU_H
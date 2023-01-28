#include <linux/acpi.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/module.h>

#include "configs.h"
#include "constans.h"
#include "cooler_boost_status.h"
#include "cpu.h"
#include "ec.h"
#include "fan_mode.h"
#include "global_store.h"
#include "gpu.h"
#include "utils.h"

#define CHECK_ERR(err, msg)      \
    do {                         \
        if (err < 0) {           \
            pr_err("%s\n", msg); \
            return err;          \
        }                        \
    } while (false)

#define CHECK_PTR(ptr, msg)      \
    do {                         \
        if (ptr == NULL) {       \
            pr_err("%s\n", msg); \
            return -ENOMEM;      \
        }                        \
    } while (false)

static struct kobject* mlekm = NULL;

int init_module(void)
{
    pr_info("start init mlekm\n");

    int err = 0;

    if (acpi_disabled) {
        pr_err("acpi is disabled\n");
        return -ENODEV;
    }

    char ec[MLEKM_EC_VERSION_LEN + 1] = { 0 };
    err = mlekm_get_ec_version(ec);
    CHECK_ERR(err, "failed to get ec version");

    config = mlekm_get_config(ec);
    CHECK_PTR(config, "failed to get config");

    mlekm = kobject_create_and_add("mlekm", firmware_kobj);
    CHECK_PTR(mlekm, "failed to create kobj");

    err = sysfs_create_file(mlekm, &mlekm_ec_version_attribute.attr);
    CHECK_ERR(err, "failed to create ec version file");

    err = sysfs_create_file(mlekm, &mlekm_ec_attribute.attr);
    CHECK_ERR(err, "failed to create ec file");

    err = sysfs_create_file(mlekm, &mlekm_fan_mode_attribute.attr);
    CHECK_ERR(err, "failed to create fan_mode file");

    err = sysfs_create_file(mlekm, &mlekm_cooler_boost_status_attribute.attr);
    CHECK_ERR(err, "failed to create cooler_boost_status file");

    err = sysfs_create_group(mlekm, &mlekm_cpu_group);
    CHECK_ERR(err, "failed to create cpu group");

    err = sysfs_create_group(mlekm, &mlekm_gpu_group);
    CHECK_ERR(err, "failed to create gpu group");

    pr_info("finish init mlekm\n");

    return 0;
}

void cleanup_module(void)
{
    pr_info("start terminate mlekm\n");

    mlekm_config_free(config);
    kobject_put(mlekm);

    pr_info("finish terminate mlekm\n");
}

MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("Marshev M.S. msmarshev@gmail.com");
MODULE_DESCRIPTION("Kernel Module For MSi Laptops EC");
MODULE_VERSION("0.1.0");
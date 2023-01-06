#ifndef MLEKM_COOLER_BOOST_STATUS_H
#define MLEKM_COOLER_BOOST_STATUS_H

#include <linux/kobject.h>

#include "constans.h"
#include "global_store.h"
#include "utils.h"

static ssize_t mlekm_cooler_boost_status_show(struct kobject* obj, struct kobj_attribute* attr, char* buf)
{
    MLEKM_READ_EC(cooler_boost_status_value, MLEKM_COOLER_BOOST_STATUS_LEN, config->addrs.cooler_boost);

    for (int i = 0; i < config->values.cooler_boost_statuses.size; ++i) {
        if (config->values.cooler_boost_statuses.data[i].value == cooler_boost_status_value[0]) {
            return sprintf(buf, "%s\n", mlekm_cooler_boost_status_to_string(config->values.cooler_boost_statuses.data[i].status));
        }
    }

    return sprintf(buf, "unknown\n");
}

static ssize_t mlekm_cooler_boost_status_store(struct kobject* obj, struct kobj_attribute* attr, const char* buf, size_t size)
{
    if (config->ec_specified == false) {
        pr_warn("ec is unspecified\n");
        return -EROFS;
    }

    enum MLEKM_COOLER_BOOST_STATUS cooler_boost_status = mlekm_string_to_cooler_boost_status(buf);
    if (cooler_boost_status == MLEKM_COOLER_BOOST_STATUS_UNKNOWN) {
        return -EINVAL;
    }

    int res = 0;
    for (int i = 0; i < config->values.cooler_boost_statuses.size; ++i) {
        if (config->values.cooler_boost_statuses.data[i].status == cooler_boost_status) {
            MLEKM_WRITE_EC(&config->values.cooler_boost_statuses.data[i].value, MLEKM_COOLER_BOOST_STATUS_LEN, config->addrs.cooler_boost);
            break;
        }
    }

    return size;
}

static struct kobj_attribute mlekm_cooler_boost_status_attribute = {
    .attr = { .name = "cooler_boost_status", .mode = 0644 },
    .show = mlekm_cooler_boost_status_show,
    .store = mlekm_cooler_boost_status_store,
};

#endif // MLEKM_COOLER_BOOST_STATUS_H
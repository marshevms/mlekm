#ifndef MLEKM_EC_H
#define MLEKM_EC_H

#include <linux/kobject.h>

#include "constans.h"
#include "utils.h"

static int mlekm_get_ec_version(char* ec)
{
    int res = mlekm_read_ec(MLEKM_EC_VERSION_ADDR, ec, MLEKM_EC_VERSION_LEN);
    if (res < 0) {
        return res;
    }

    return 0;
}

static ssize_t mlekm_ec_version_show(struct kobject* obj, struct kobj_attribute* attr, char* buf)
{
    MLEKM_READ_EC(ec_version, MLEKM_EC_VERSION_LEN, MLEKM_EC_VERSION_ADDR);

    return sprintf(buf, "%s\n", ec_version);
}

static ssize_t mlekm_ec_show(struct kobject* obj, struct kobj_attribute* attr, char* buf)
{
    MLEKM_READ_EC(ec, MLEKM_EC_LEN, MLEKM_EC_ADDR);

    res = sprintf(buf, "%x", ec[0]);
    if (res < 0) {
        pr_err("failed to print ec\n");
        return res;
    }

    int count = res;
    for (int i = 1; i < MLEKM_EC_LEN; ++i) {
        res = sprintf(buf + count, " %x", ec[i]);
        if (res < 0) {
            pr_err("failed to print ec\n");
            return res;
        }

        count += res;
    }

    res = sprintf(buf + count, "\n");
    if (res < 0) {
        pr_err("failed to print ec\n");
        return res;
    }

    return count + res;
}

static struct kobj_attribute mlekm_ec_version_attribute = {
    .attr = { .name = "ec_version", .mode = 0444 },
    .show = mlekm_ec_version_show,
};

static struct kobj_attribute mlekm_ec_attribute = {
    .attr = { .name = "ec", .mode = 0444 },
    .show = mlekm_ec_show,
};

#endif // MLEKM_EC_H
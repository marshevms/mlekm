#include <linux/string.h>

#include "configs/16J5EMS3.108.h"

typedef const struct mlekm_config*(ec_config_init)(void);

static enum EC_VERSION {
    EC_VERSION_UNKNOWN = -1,
    EC_VERSION_16J5EMS3_108,
} EC_VERSION;

static const char* const ec_versions[] = {
    [EC_VERSION_16J5EMS3_108] = "16J5EMS3.108"
};

static ec_config_init* inits[] = {
    [EC_VERSION_16J5EMS3_108] = mlekm_config_16j5ems3_108_init
};

_Static_assert(ARRAY_SIZE(ec_versions) == ARRAY_SIZE(inits), "length of ev_versions and inits are not equal");

const struct mlekm_config* mlekm_get_config(const char* ec_version)
{
    for (int i = 0; i < ARRAY_SIZE(ec_versions); ++i) {
        if (strcmp(ec_version, ec_versions[i]) == 0) {
            return inits[i]();
        }
    }

    return mlekm_config_init_default();
}
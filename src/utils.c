#include <linux/acpi.h>
#include <linux/backlight.h>

#include "utils.h"

// /* EC commands */
// enum ec_command
// {
//     ACPI_EC_COMMAND_READ = 0x80,
//     ACPI_EC_COMMAND_WRITE = 0x81,
// };

int mlekm_read_ec(const u8 addr, u8* const buf, const int len)
{
    // TODO: can we use ec_transaction to read all data instead of loop???
    for (int i = 0; i < len; ++i) {
        int res = ec_read(addr + i, &buf[i]);
        if (res < 0) {
            return res;
        }
    }

    return 0;
}

int mlekm_write_ec(const u8 addr, const u8* buf, const int len)
{
    // TODO: can we use ec_transaction to write all data instead of loop???
    for (int i = 0; i < len; ++i) {
        int res = ec_write(addr + i, buf[i]);
        if (res < 0) {
            return res;
        }
    }

    return 0;
}

int mlekm_to_array(const u8* data, u8 size, char* buf)
{
    int count = 0;
    count = sprintf(buf, "%d", data[0]);
    if (count < 0) {
        return count;
    }

    int res = 0;
    for (int i = 1; i < size; ++i) {
        res = sprintf(buf + count, ",%d", data[i]);
        if (res < 0) {
            return res;
        }

        count += res;
    }

    res = sprintf(buf + count, "\n");
    if (res < 0){
        return res;
    }

    return count+res;
}

int mlekm_string_to_array(const char* src, size_t src_size, u8* dst, size_t dst_size)
{
    char* buffer = kzalloc(src_size + 1, GFP_KERNEL);
    if (buffer == NULL) {
        return -ENOMEM;
    }

    memcpy_fromio(buffer, src, src_size);

    int curr = 0;
    int counter = 0;
    int res = 0;
    for (int i = 0; i < src_size; ++i) {
        if (buffer[i] == ',') {
            buffer[i] = '\0';

            res = kstrtou8(buffer + curr, 10, &dst[counter++]);
            if (res < 0) {
                kfree(buffer);
                return res;
            }

            if (counter == dst_size) {
                break;
            }

            curr = i + 1;
        }
    }

    if (counter < dst_size) {
        res = kstrtou8(buffer + curr, 10, &dst[counter]);
        if (res < 0) {
            kfree(buffer);
            return res;
        }
    }

    kfree(buffer);

    return 0;
}

const char* mlekm_fan_mode_to_string(const enum MLEKM_FAN_MODE mode)
{
    switch (mode) {
    case MLEKM_FAN_MODE_AUTO:
        return "auto";
    case MLEKM_FAN_MODE_BASIC:
        return "basic";
    case MLEKM_FAN_MODE_ADVANCED:
        return "advanced";
    case MLEKM_FAN_MODE_UNKNOWN:
        return "unknown";
    }

    return "unknown";
}

enum MLEKM_FAN_MODE mlekm_string_to_fan_mode(const char* str)
{
    if (strcmp("auto", str) == 0 || strcmp("auto\n", str) == 0) {
        return MLEKM_FAN_MODE_AUTO;
    }
    if (strcmp("basic", str) == 0 || strcmp("basic\n", str) == 0) {
        return MLEKM_FAN_MODE_BASIC;
    }
    if (strcmp("advanced", str) == 0 || strcmp("advanced\n", str) == 0) {
        return MLEKM_FAN_MODE_ADVANCED;
    }

    return MLEKM_FAN_MODE_UNKNOWN;
}

const char* mlekm_cooler_boost_status_to_string(const enum MLEKM_COOLER_BOOST_STATUS status)
{
    switch (status) {
    case MLEKM_COOLER_BOOST_STATUS_ONN:
        return "onn";
    case MLEKM_COOLER_BOOST_STATUS_OFF:
        return "off";
    case MLEKM_COOLER_BOOST_STATUS_UNKNOWN:
        return "unknown";
    }

    return "unknown";
}

enum MLEKM_COOLER_BOOST_STATUS mlekm_string_to_cooler_boost_status(const char* str)
{
    if (strcmp("onn", str) == 0 || strcmp("onn\n", str) == 0) {
        return MLEKM_COOLER_BOOST_STATUS_ONN;
    }
    if (strcmp("off", str) == 0 || strcmp("off\n", str) == 0) {
        return MLEKM_COOLER_BOOST_STATUS_OFF;
    }

    return MLEKM_COOLER_BOOST_STATUS_UNKNOWN;
}

bool mlekm_check_values(const u8* data, size_t size)
{
    for (int i = 1; i < size; ++i) {
        if (data[i - 1] > data[i]) {
            return false;
        }
    }

    return true;
}
#ifndef MLEKM_UTILS_H
#define MLEKM_UTILS_H

#include <asm-generic/int-ll64.h>

#include "constans.h"

int mlekm_read_ec(u8 addr, u8* buf, const int len);
int mlekm_write_ec(const u8 addr, const u8* buf, const int len);

#define MLEKM_READ_EC(name, size, addr)                                     \
    _Static_assert(size <= MLEKM_EC_LEN, "size is more then MLEKM_EC_LEN"); \
    u8 name[size + 1] = { 0 };                                              \
    int res = mlekm_read_ec(addr, name, size);                              \
    do {                                                                    \
        if (res < 0) {                                                      \
            pr_err("failed to read %s: %d\n", #name, res);                  \
            return res;                                                     \
        }                                                                   \
    } while (false)

#define MLEKM_WRITE_EC(buf, size, addr)                                     \
    _Static_assert(size <= MLEKM_EC_LEN, "size is more then MLEKM_EC_LEN"); \
    int res = mlekm_write_ec(addr, buf, size);                              \
    do {                                                                    \
        if (res < 0) {                                                      \
            pr_err("failed to write %s: %d\n", #buf, res);                  \
            return res;                                                     \
        }                                                                   \
    } while (false)

// write array of data to buf
// example: 24,25,26,27,28
int mlekm_to_array(const u8* data, u8 d_size, char* buf);

int mlekm_string_to_array(const char* src, size_t src_size, u8* dst, size_t dst_size);

const char* mlekm_fan_mode_to_string(const enum MLEKM_FAN_MODE mode);

enum MLEKM_FAN_MODE mlekm_string_to_fan_mode(const char* str);

const char* mlekm_cooler_boost_status_to_string(const enum MLEKM_COOLER_BOOST_STATUS status);

enum MLEKM_COOLER_BOOST_STATUS mlekm_string_to_cooler_boost_status(const char* str);

bool mlekm_check_values(const u8* data, size_t size);

#endif // MLEKM_UTILS_H
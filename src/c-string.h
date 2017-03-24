#pragma once

/*
 * String Helpers
 *
 * This is a collection of helper functions operating on standard C strings
 * (i.e., zero-terminated character arrays). They extend the ISO str*() class
 * of function and behave in a similar manner.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <c-macro.h>
#include <stdlib.h>

/**
 * c_string_equal() - compare strings for equality
 * @a:          first string to compare, or NULL
 * @b:          second string to compare, or NULL
 *
 * Compare to strings for equality, the same way strcmp() does it.
 * Additionally, NULL is allowed as input and compares equal to itself only.
 * Unlike strcmp(), this returns a boolean.
 *
 * Return: True if both are equal, false if not.
 */
_c_pure_ static inline bool c_string_equal(const char *a, const char *b) {
        return (!a || !b) ? (a == b) : !strcmp(a, b);
}

/**
 * c_string_prefix() - check prefix of a string
 * @str:        string to check
 * @prefix:     prefix to look for
 *
 * This checks whether @str starts with @prefix. If it does, a pointer to the
 * first character in @str after the prefix is returned, if not, NULL is
 * returned.
 *
 * Return: Pointer directly behind the prefix in @str, or NULL if not found.
 */
_c_pure_ static inline char *c_string_prefix(const char *str, const char *prefix) {
        size_t l = strlen(prefix);
        return !strncmp(str, prefix, l) ? (char *)str + l : NULL;
}

/**
 * c_string_to_hex() - encode string as ascii-hex
 * @str:        string to encode from
 * @n:          length of @str in bytes
 * @hex:        destination buffer
 *
 * This hex-encodes the source string into the destination buffer. The
 * destination buffer must be at least twice as big as the source.
 */
static inline void c_string_to_hex(const char *str, size_t n, char *hex) {
        static const char table[16] = "0123456789abcdef";

        while (n--) {
                *hex++ = table[(*str >> 4) & 0x0f];
                *hex++ = table[(*str++) & 0x0f];
        }
}

/**
 * c_string_from_hex() - decode ascii-hex string
 * @str:        string buffer to write into
 * @n:          length of @str in bytes
 * @hex:        hex encoded buffer to decode
 *
 * This hex-decodes @hex into the string buffer @str. Be aware that @hex must
 * be twice the size as @str / @n.
 *
 * Return: True if successful, false if invalid.
 */
static inline bool c_string_from_hex(char *str, size_t n, const char *hex) {
        static const uint8_t table[128] = {
                 -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
                 -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
                 -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
                0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9,  -1,  -1, -1,  -1,  -1,  -1,
                 -1, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf,  -1, -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
                 -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
                 -1, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf,  -1, -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
                 -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
        };
        uint8_t v1, v2;

        for ( ; n; --n, hex += 2) {
                v1 = table[hex[0] & 0x7f];
                v2 = table[hex[1] & 0x7f];
                if (_c_unlikely_((hex[0] | hex[1] | v1 | v2) & 0x80))
                        return false;

                *str++ = (v1 << 4) | v2;
        }

        return true;
}

#ifdef __cplusplus
}
#endif

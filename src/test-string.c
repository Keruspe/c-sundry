/*
 * Tests for string manipulation helpers
 */

#include <stdlib.h>
#include "c-macro.h"
#include "c-string.h"

static void test_compare(void) {
        int r;

        r = c_string_compare(NULL, NULL);
        assert(r == 0);

        r = c_string_compare("", NULL);
        assert(r > 0);

        r = c_string_compare(NULL, "");
        assert(r < 0);

        r = c_string_compare("a", "a");
        assert(r == 0);

        r = c_string_compare("a", "b");
        assert(r < 0);
}

static void test_equal(void) {
        bool b;

        b = c_string_equal(NULL, NULL);
        assert(b);

        b = c_string_equal("", NULL);
        assert(!b);

        b = c_string_equal(NULL, "");
        assert(!b);

        b = c_string_equal("a", "a");
        assert(b);

        b = c_string_equal("a", "b");
        assert(!b);
}

static void test_verify_from_hex(const char *hex) {
        _c_cleanup_(c_freep) char *raw = NULL, *copy = NULL;
        bool valid_hex1, valid_hex2;
        size_t n_hex;

        /* a hex-string is valid _iff_ it consists of 0-9a-fA-F */

        n_hex = strlen(hex);
        raw = calloc(1, n_hex);
        copy = calloc(1, n_hex);
        assert(raw && copy);

        valid_hex1 = !(n_hex % 2) && (n_hex == strspn(hex, "0123456789abcdefABCDEF"));
        valid_hex2 = !(n_hex % 2) && c_string_from_hex(raw, n_hex / 2, hex);

        assert(valid_hex1 == valid_hex2);

        /* verify one round through c_string_from/to_hex() keeps the form */

        if (valid_hex2) {
                c_string_to_hex(raw, n_hex / 2, copy);
                assert(!strncasecmp(hex, copy, n_hex));
        }
}

/* test hex en/de-coders */
static void test_hex(void) {
        test_verify_from_hex("0");
        test_verify_from_hex("00");
        test_verify_from_hex("0a");
        test_verify_from_hex("a" "0");
        test_verify_from_hex("0123456789abcdefABCDEF");
        test_verify_from_hex("a" "\x01");
        test_verify_from_hex("\x01" "a");
}

static void test_ascii(void) {
        char str[0x100];
        char *p = str;
        size_t len = sizeof(str);

        for (size_t i = 0; i < sizeof(str); ++i)
                str[i] = i;

        c_string_verify_ascii(&p, &len);
        assert(*p == 0x00);
        assert(p == str);
        assert(len == sizeof(str));

        ++p;
        --len;

        c_string_verify_ascii(&p, &len);
        assert((unsigned char)*p == 0x80);
        assert(p == str + 0x7F + 1);
        assert(len == sizeof(str) - 0x7F - 1);
}

static void test_utf8(void) {
        char *str = "Η Ελλάδα ή Ελλάς, επίσημα γνωστή ως Ελληνική Δημοκρατία, είναι χώρα της νοτιοανατολικής Ευρώπης στο νοτιότερο άκρο της Βαλκανικής χερσονήσου. Συνορεύει στα βορειοδυτικά με την Αλβανία, στα βόρεια με την πρώην Γιουγκοσλαβική Δημοκρατία της Μακεδονίας και τη Βουλγαρία και στα βορειοανατολικά με την Τουρκία. Česko, úředním názvem Česká republika, je stát ve střední Evropě. Jako formálně svrchovaný národní stát vznikla tehdejší Česká socialistická republika 1. ledna 1969 v rámci federalizace Československa. Od 6. března 1990 nese tento stát název Česká republika. 中華民國十年，中國共產黨立於上海。初附於中國國民黨，黨人得以兼國民黨，共理中華民國廣州軍政府，同謀北伐。其後國民黨人以共產黨人以公務營黨務，既下南京，蔣中正令捕殺共產黨人。遂奔江西。十六年起義於南昌，中國工農紅軍是立";
        char *p = str;
        size_t len = strlen(str) + 1;

        c_string_verify_utf8(&p, &len);
        assert(len == 1);
        assert(p == str + strlen(str));
        assert(*p == 0x00);
}

int main(int argc, char **argv) {
        test_compare();
        test_equal();
        test_hex();
        test_ascii();
        test_utf8();
        return 0;
}

#pragma once

#include "BaseTestTemplate.h"
#include "BufferHashMap.h"


CREATE_HASH_MAP_TYPE(int, int);
CREATE_HASH_MAP_TYPE(int, char);
CREATE_HASH_MAP_TYPE(int8_t, float, i8);
CREATE_HASH_MAP_TYPE(char*, int, str, strComparator, strHashCode);
CREATE_HASH_MAP_TYPE(char*, char*, cStr, cStr, strComparator, strHashCode);
CREATE_HASH_MAP_TYPE(str, str);
CREATE_HASH_MAP_TYPE(user, int);


void assertIntMap(int_intMap *intMap, int size, int capacity) {
    assert_uint32(intMap->size, ==, size);
    assert_uint32(intMap->capacity, ==, capacity);
    assert_not_null(intMap->entries);
}

static MunitResult testBuffMapCreation(const MunitParameter params[], void *data) {
    // alignment test
    assertIntMap(NEW_HASH_MAP(int, int, 4), 0, 8);
    assertIntMap(NEW_HASH_MAP(int, int, 1), 0, 2);
    assertIntMap(NEW_HASH_MAP(int, int, 3), 0, 6);
    assertIntMap(NEW_HASH_MAP(int, int, 11), 0, 22);
    assertIntMap(NEW_HASH_MAP(int, int, 33), 0, 66);

    assertIntMap(NEW_HASH_MAP_4(int, int), 0, 8);
    assertIntMap(NEW_HASH_MAP_8(int, int), 0, 16);
    assertIntMap(NEW_HASH_MAP_16(int, int), 0, 32);
    assertIntMap(NEW_HASH_MAP_32(int, int), 0, 64);
    assertIntMap(NEW_HASH_MAP_64(int, int), 0, 128);
    assertIntMap(NEW_HASH_MAP_128(int, int), 0, 256);
    assertIntMap(NEW_HASH_MAP_256(int, int), 0, 512);
    assertIntMap(NEW_HASH_MAP_512(int, int), 0, 1024);
    assertIntMap(NEW_HASH_MAP_1024(int, int), 0, 2048);

    assertIntMap(HASH_MAP_OF(int, int, { 1, 1 }), 1, 2);
    assertIntMap(HASH_MAP_OF(int, int, { 1, 1 }, { 2, 2 }), 2, 4);
    assertIntMap(HASH_MAP_OF(int, int, { 1, 1 }, { 2, 2 }, { 3, 3 }), 3, 6);
    assertIntMap(HASH_MAP_OF(int, int, { 1, 1 }, { 2, 2 }, { 3, 3 }, { 4, 4 }, { 5, 5 }, { 6, 6 }, { 7, 7 }, { 8, 8 }, { 9, 9 }, { 10, 10 }, { 11, 11 },{ 12, 12 }), 12, 24);

    str_strMap *strMap = HASH_MAP_OF(str, str, { "k1", "v1" }, { "k2", "v2" }, { "k3", "v3" });
    assert_uint32(strMap->size, ==, 3);
    assert_uint32(strMap->capacity, ==, 6);

    cStr_cStrMap *cStrMap = NEW_HASH_MAP_OF(6, cStr, cStr, { "k1", "v1" }, { "k2", "v2" }, { "k3", "v3" });
    assert_uint32(cStrMap->size, ==, 3);
    assert_uint32(cStrMap->capacity, ==, 12);

    i8_floatMap *i8Map = NEW_HASH_MAP_OF(15, i8, float, { 1, 1.1 }, { 2, 2.2 }, { 3, 3.3 });
    assert_uint32(i8Map->size, ==, 3);
    assert_uint32(i8Map->capacity, ==, 30);

    user_intMap *usrMap = HASH_MAP_OF(user, int, {{ "first", 23 }, 123 }, {{ "second", 18 }, 456 });
    assert_uint32(usrMap->size, ==, 2);
    assert_uint32(usrMap->capacity, ==, 6);
    return MUNIT_OK;
}

static MunitResult testBuffMapAdd(const MunitParameter params[], void *data) {
    str_strMap *strMap = NEW_HASH_MAP_4(str, str);
    assert_true(str_strMapAdd(strMap, "key_1", "value_1"));
    assert_true(str_strMapAdd(strMap, "key_2", "value_2"));
    assert_true(str_strMapAdd(strMap, "key_3", "value_3"));

    assert_false(str_strMapAdd(strMap, "key_3", "value_33"));
    assert_string_equal(str_strMapGet(strMap, "key_3"), "value_33");
    assert_uint32(strMap->size, ==, 3);

    assert_true(str_strMapAdd(strMap, "key_4", "value_4"));

    assert_false(str_strMapAdd(strMap, "key_5", "value_5"));
    assert_uint32(strMap->size, ==, 4);
    assert_uint32(strMap->capacity, ==, 8);
    return MUNIT_OK;
}

static MunitResult testBuffMapGet(const MunitParameter params[], void *data) {
    i8_floatMap *i8Map = NEW_HASH_MAP_OF(23, i8, float, { 1, 1.1 }, { 2, 2.2 }, { 3, 3.3 });
    assert_float(i8_floatMapGet(i8Map, 1), ==, 1.1);
    assert_float(i8_floatMapGet(i8Map, 2), ==, 2.2);
    assert_float(i8_floatMapGet(i8Map, 3), ==, 3.3);

    assert_float(i8_floatMapGet(i8Map, 4), ==, 0.0);
    assert_float(i8_floatMapGetOrDefault(i8Map, 4, -1.0), ==, -1.0);
    return MUNIT_OK;
}

static MunitResult testBuffMapRemove(const MunitParameter params[], void *data) {
    str_strMap *strMap = HASH_MAP_OF(str, str, { "k1", "v1" }, { "k2", "v2" }, { "k3", "v3" });
    assert_uint32(strMap->size, ==, 3);
    str value = str_strMapRemove(strMap, "k2");
    assert_uint32(strMap->size, ==, 2);
    assert_string_equal(value, "v2");
    return MUNIT_OK;
}

static MunitResult testBuffMapAddAll(const MunitParameter params[], void *data) {
    str_strMap *strMap1 = HASH_MAP_OF(str, str, { "k1", "v1" }, { "k2", "v2" }, { "k3", "v3" });
    str_strMap *strMap2 = NEW_HASH_MAP_OF(12, str, str, {"k4", "v4"}, {"k5", "v5"});
    str_strMapAddAll(strMap1, strMap2);
    assert_uint32(strMap2->size, ==, 5);
    assert_string_equal(str_strMapGet(strMap2, "k1"), "v1");
    assert_string_equal(str_strMapGet(strMap2, "k2"), "v2");
    assert_string_equal(str_strMapGet(strMap2, "k3"), "v3");
    assert_string_equal(str_strMapGet(strMap2, "k4"), "v4");
    assert_string_equal(str_strMapGet(strMap2, "k5"), "v5");
    return MUNIT_OK;
}

static MunitResult testBuffMapClear(const MunitParameter params[], void *data) {
    str_strMap *strMap = HASH_MAP_OF(str, str, { "k1", "v1" }, { "k2", "v2" }, { "k3", "v3" });
    assert_uint32(str_strMapSize(strMap), ==, 3);
    str_strMapClear(strMap);
    assert_uint32(str_strMapSize(strMap), ==, 0);
    return MUNIT_OK;
}

static MunitResult testBuffMapSize(const MunitParameter params[], void *data) {
    int_charMap *charMap = NEW_HASH_MAP_OF(12, int, char, { 1, 'a' }, { 2, 'b' }, { 3, 'c' });
    assert_uint32(int_charMapSize(charMap), ==, 3);
    int_charMap *charMap2 = NEW_HASH_MAP_4(int, char);
    assert_uint32(int_charMapSize(charMap2), ==, 0);
    return MUNIT_OK;
}

static MunitResult testBuffMapEmpty(const MunitParameter params[], void *data) {
    int_charMap *charMap = HASH_MAP_OF(int, char, { 1, 'a' }, { 2, 'b' }, { 3, 'c' });
    assert_false(is_int_charMapEmpty(charMap));
    assert_true(is_int_charMapNotEmpty(charMap));

    int_charMapClear(charMap);
    assert_true(is_int_charMapEmpty(charMap));
    assert_false(is_int_charMapNotEmpty(charMap));
    return MUNIT_OK;
}

static MunitResult testBuffMapContains(const MunitParameter params[], void *data) {
    str_strMap *strMap = HASH_MAP_OF(str, str, { "k1", "v1" }, { "k2", "v2" }, { "k3", "v3" });
    assert_true(str_strMapContains(strMap, "k1"));
    assert_false(str_strMapContains(strMap, "k4"));
    return MUNIT_OK;
}

static MunitResult testBuffMapIterator(const MunitParameter params[], void *data) {
    int_charMap *charMap = HASH_MAP_OF(int, char, { 1, 'a' }, { 2, 'b' }, { 3, 'c' });
    int_charMapIterator iter = int_charMapIter(charMap);
    while (int_charMapHasNext(&iter)) {
        assert_true(int_charMapContains(charMap, iter.key));
        char value = int_charMapGet(charMap, iter.key);
        assert_true(value == 'a' || value == 'b' || value == 'c');
    }
    return MUNIT_OK;
}



static MunitTest bufferHashMapTests[] = {
        {.name =  "Test new Map - should correctly create and init map", .test = testBuffMapCreation},
        {.name =  "Test <key>_<value>MapAdd() - should correctly add elements to map", .test = testBuffMapAdd},
        {.name =  "Test <key>_<value>MapGet() - should correctly get elements from map", .test = testBuffMapGet},
        {.name =  "Test <key>_<value>MapRemove() - should correctly remove elements from map", .test = testBuffMapRemove},
        {.name =  "Test <key>_<value>MapAddAll() - should correctly add all elements from one map to other", .test = testBuffMapAddAll},
        {.name =  "Test <key>_<value>MapClear() - should correctly remove all elements from map", .test = testBuffMapClear},
        {.name =  "Test <key>_<value>MapSize() - should correctly return elements count", .test = testBuffMapSize},
        {.name =  "Test is<key>_<value>MapEmpty() - should correctly check map emptiness", .test = testBuffMapEmpty},
        {.name =  "Test <key>_<value>MapHasKey() - should correctly check that map contains key", .test = testBuffMapContains},
        {.name =  "Test <key>_<value>MapIterator - should correctly check iterate through key/value", .test = testBuffMapIterator},
        END_OF_TESTS
};

static const MunitSuite bufferHashMapTestSuite = {
        .prefix = "BufferHashMap: ",
        .tests = bufferHashMapTests,
        .suites = NULL,
        .iterations = 1,
        .options = MUNIT_SUITE_OPTION_NONE
};


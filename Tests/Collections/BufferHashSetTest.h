#pragma once

#include "BaseTestTemplate.h"
#include "BufferHashSet.h"

CREATE_HASH_SET_TYPE(int);
CREATE_HASH_SET_TYPE(float);
CREATE_HASH_SET_TYPE(char);
CREATE_HASH_SET_TYPE(int8_t, i8);
CREATE_HASH_SET_TYPE(char*, cStr, strComparator, strHashCode);

typedef char *str;
CREATE_HASH_SET_TYPE(str);
CREATE_HASH_SET_TYPE(user);


static void assertIntSet(intHashSet *intSet, int size, int capacity) {
    assert_uint32(intSet->size, ==, size);
    assert_uint32(intSet->capacity, ==, capacity);
    assert_not_null(intSet->entries);
}


static MunitResult testBuffSetCreation(const MunitParameter params[], void *data) {
    // alignment test
    assertIntSet(NEW_HASH_SET(int, 4), 0, 8);
    assertIntSet(NEW_HASH_SET(int, 1), 0, 2);
    assertIntSet(NEW_HASH_SET(int, 3), 0, 8);
    assertIntSet(NEW_HASH_SET(int, 11), 0, 32);
    assertIntSet(NEW_HASH_SET(int, 30), 0, 64);

    assertIntSet(NEW_HASH_SET_4(int), 0, 8);
    assertIntSet(NEW_HASH_SET_8(int), 0, 16);
    assertIntSet(NEW_HASH_SET_16(int), 0, 32);
    assertIntSet(NEW_HASH_SET_32(int), 0, 64);
    assertIntSet(NEW_HASH_SET_64(int), 0, 128);
    assertIntSet(NEW_HASH_SET_128(int), 0, 256);
    assertIntSet(NEW_HASH_SET_256(int), 0, 512);
    assertIntSet(NEW_HASH_SET_512(int), 0, 1024);
    assertIntSet(NEW_HASH_SET_1024(int), 0, 2048);

    assertIntSet(HASH_SET_OF(int, { 1 }), 1, 2);
    assertIntSet(HASH_SET_OF(int, { 1 }, { 2 }), 2, 4);
    assertIntSet(HASH_SET_OF(int, { 1 }, { 2 }, { 3 }), 3, 8);
    assertIntSet(HASH_SET_OF(int, { 1 }, { 2 }, { 3 }, { 4 }, { 5 }, { 6 }, { 7 }, { 8 }, { 9 }, { 10 }, { 11 }, { 12 }, { 13 }, { 14 }, { 15 }, { 16 }, { 17 }, { 18 }, { 19 }, { 20 }), 20, 64);

    strHashSet *strSet = HASH_SET_OF(str, { "v1" }, { "v2" }, { "v3" });
    assert_uint32(strSet->size, ==, 3);
    assert_uint32(strSet->capacity, ==, 8);

    cStrHashSet *cStrSet = NEW_HASH_SET_OF(6, cStr,{ "v1" }, { "v2" }, { "v3" });
    assert_uint32(cStrSet->size, ==, 3);
    assert_uint32(cStrSet->capacity, ==, 16);

    floatHashSet *flSet = NEW_HASH_SET_OF(15, float, { 1.1 }, { 2.2 }, { 3.3 });
    assert_uint32(flSet->size, ==, 3);
    assert_uint32(flSet->capacity, ==, 32);

    userHashSet *usrSet = HASH_SET_OF(user, { "first", 23 }, { "second", 18 });
    assert_uint32(usrSet->size, ==, 2);
    assert_uint32(usrSet->capacity, ==, 8);
    return MUNIT_OK;
}

static MunitResult testBuffSetAdd(const MunitParameter params[], void *data) {
    strHashSet *strSet = NEW_HASH_SET_4(str);
    assert_true(strSetAdd(strSet, "value_1"));
    assert_true(strSetAdd(strSet, "value_2"));
    assert_true(strSetAdd(strSet, "value_3"));

    assert_false(strSetAdd(strSet, "value_3"));
    assert_uint32(strSet->size, ==, 3);

    assert_true(strSetAdd(strSet, "value_4"));

    assert_false(strSetAdd(strSet, "value_5"));
    assert_uint32(strSet->size, ==, 4);
    assert_uint32(strSet->capacity, ==, 8);
    return MUNIT_OK;
}

static MunitResult testBuffSetRemove(const MunitParameter params[], void *data) {
    strHashSet *strSet = HASH_SET_OF(str, { "v1" }, { "v2" }, { "v3" });
    assert_uint32(strSet->size, ==, 3);
    assert_true(strSetRemove(strSet, "v2"));
    assert_uint32(strSet->size, ==, 2);
    assert_false(strSetRemove(strSet, "v4"));
    assert_uint32(strSet->size, ==, 2);
    return MUNIT_OK;
}

static MunitResult testBuffSetAddAll(const MunitParameter params[], void *data) {
    strHashSet *strSet1 = HASH_SET_OF(str, { "v1" }, { "v2" }, { "v3" });
    strHashSet *strSet2 = NEW_HASH_SET_OF(12, str, {"v4"}, {"v5"});
    strSetAddAll(strSet1, strSet2);
    assert_uint32(strSet2->size, ==, 5);
    assert_true(strSetContainsAll(strSet2, HASH_SET_OF(str, { "v1" }, { "v2" }, { "v3" }, {"v4"}, {"v5"})));
    return MUNIT_OK;
}

static MunitResult testBuffSetClear(const MunitParameter params[], void *data) {
    strHashSet *strSet = HASH_SET_OF(str, { "v1" }, { "v2" }, { "v3" });
    assert_uint32(strSetSize(strSet), ==, 3);
    strSetClear(strSet);
    assert_uint32(strSetSize(strSet), ==, 0);
    return MUNIT_OK;
}

static MunitResult testBuffSetEmpty(const MunitParameter params[], void *data) {
    charHashSet *charSet = HASH_SET_OF(char, { 'a' }, { 'b' }, { 'c' });
    assert_false(ischarSetEmpty(charSet));
    assert_true(ischarSetNotEmpty(charSet));
    charSetClear(charSet);
    assert_true(ischarSetEmpty(charSet));
    assert_false(ischarSetNotEmpty(charSet));
    return MUNIT_OK;
}

static MunitResult testBuffSetContains(const MunitParameter params[], void *data) {
    strHashSet *strSet = HASH_SET_OF(str, { "v1" }, { "v2" }, { "v3" });
    assert_true(strSetContains(strSet, "v1"));
    assert_false(strSetContains(strSet, "v4"));

    strHashSet *strSet2 = NEW_HASH_SET_8(str);
    assert_false(strSetContains(strSet2, "v1"));
    return MUNIT_OK;
}

static MunitResult testBuffSetContainsAll(const MunitParameter params[], void *data) {
    strHashSet *strSet1 = HASH_SET_OF(str, { "v1" }, { "v2" }, { "v3" });
    strHashSet *strSet2 = HASH_SET_OF(str, { "v1" }, { "v3" });
    assert_true(strSetContainsAll(strSet1, strSet2));

    strHashSet *strSet3 = NEW_HASH_SET_8(str);
    assert_false(strSetContainsAll(strSet3, strSet2));
    return MUNIT_OK;
}

static MunitResult testBuffSetIterator(const MunitParameter params[], void *data) {
    charHashSet *charSet = HASH_SET_OF(char, { 'a' }, { 'b' }, { 'c' });
    charSetIterator iter = charSetIter(charSet);
    while (charSetHasNext(&iter)) {
        assert_true(charSetContains(charSet, iter.value));
    }
    return MUNIT_OK;
}


static MunitTest bufferHashSetTests[] = {
        {.name =  "Test new Set - should correctly create and init HashSet", .test = testBuffSetCreation},
        {.name =  "Test <type>SetAdd() - should correctly add values to Set", .test = testBuffSetAdd},
        {.name =  "Test <type>SetRemove() - should correctly remove values", .test = testBuffSetRemove},
        {.name =  "Test <type>SetAddAll() - should correctly add all values to set", .test = testBuffSetAddAll},
        {.name =  "Test <type>SetAddClear() - should correctly remove all values", .test = testBuffSetClear},
        {.name =  "Test is<type>SetEmpty() - should correctly check set emptiness", .test = testBuffSetEmpty},
        {.name =  "Test <type>SetContains() - should correctly check that value in set", .test = testBuffSetContains},
        {.name =  "Test <type>SetContainsAll() - should correctly check that all values in set", .test = testBuffSetContainsAll},
        {.name =  "Test <type>SetIterator() - should correctly iterate set values", .test = testBuffSetIterator},
        END_OF_TESTS
};

static const MunitSuite bufferHashSetTestSuite = {
        .prefix = "BufferHashSet: ",
        .tests = bufferHashSetTests,
        .suites = NULL,
        .iterations = 1,
        .options = MUNIT_SUITE_OPTION_NONE
};
#pragma once

#include "BaseTestTemplate.h"
#include "BufferVectorDeque.h"

CREATE_VECTOR_DEQ_TYPE(int);
CREATE_VECTOR_DEQ_TYPE(float);
CREATE_VECTOR_DEQ_TYPE(char);
CREATE_VECTOR_DEQ_TYPE(int8_t, i8);
CREATE_VECTOR_DEQ_TYPE(uint32_t, u32);
CREATE_VECTOR_DEQ_TYPE(char*, cStr, strComparator);
CREATE_VECTOR_DEQ_TYPE(User, user, userAgeComparator);
CREATE_VECTOR_DEQ_TYPE(str);


static void assertIntVecDeq(intVecDeq *intVec, int size, int capacity) {
    assert_uint32(intVecDeqSize(intVec), ==, size);
    assert_uint32(intVec->capacity, ==, capacity);
    assert_not_null(intVec->items);
}

static MunitResult testBuffVecDeqCreation(const MunitParameter params[], void *data) {
    assertIntVecDeq(NEW_VECTOR_DEQ(int, 1), 0, 2);
    assertIntVecDeq(NEW_VECTOR_DEQ(int, 2), 0, 2);

    assertIntVecDeq(NEW_VECTOR_DEQ_4(int), 0, 4);
    assertIntVecDeq(NEW_VECTOR_DEQ_8(int), 0, 8);
    assertIntVecDeq(NEW_VECTOR_DEQ_16(int), 0, 16);
    assertIntVecDeq(NEW_VECTOR_DEQ_32(int), 0, 32);
    assertIntVecDeq(NEW_VECTOR_DEQ_64(int), 0, 64);
    assertIntVecDeq(NEW_VECTOR_DEQ_128(int), 0, 128);
    assertIntVecDeq(NEW_VECTOR_DEQ_256(int), 0, 256);
    assertIntVecDeq(NEW_VECTOR_DEQ_512(int), 0, 512);
    assertIntVecDeq(NEW_VECTOR_DEQ_1024(int), 0, 1024);

    cStrVecDeq *strVecDeq = NEW_VECTOR_DEQ(char*, cStr, 2);
    assert_uint32(strVecDeq->size, ==, 0);
    assert_uint32(strVecDeq->capacity, ==, 2);

    charVecDeq *chVecDeq = NEW_VECTOR_DEQ(char, 3);
    assert_uint32(chVecDeq->size, ==, 0);
    assert_uint32(chVecDeq->capacity, ==, 4);

    floatVecDeq *flVecDeq = NEW_VECTOR_DEQ(float, 5);
    assert_uint32(flVecDeq->size, ==, 0);
    assert_uint32(flVecDeq->capacity, ==, 6);
    return MUNIT_OK;
}

static MunitResult testBuffVecDeqAddFirst(const MunitParameter params[], void *data) {
    intVecDeq *intVectDeq = NEW_VECTOR_DEQ_4(int);
    assert_true(intVecDeqAddFirst(intVectDeq, 1));
    assert_true(intVecDeqAddFirst(intVectDeq, 2));
    assert_true(intVecDeqAddFirst(intVectDeq, 3));
    assert_true(intVecDeqAddFirst(intVectDeq, 4));

    assert_false(intVecDeqAddFirst(intVectDeq, 5));
    assertIntVecDeq(intVectDeq, 4, 4);

    intVecDeq *intVectDeq2 = NEW_VECTOR_DEQ_4(int);
    assert_true(intVecDeqAddFirst(intVectDeq2, 1));
    assert_true(intVecDeqAddFirst(intVectDeq2, 2));
    assert_true(intVecDeqAddLast(intVectDeq2, 3));
    assert_true(intVecDeqAddLast(intVectDeq2, 4));

    assert_false(intVecDeqAddFirst(intVectDeq2, 5));
    assertIntVecDeq(intVectDeq2, 4, 4);
    return MUNIT_OK;
}

static MunitResult testBuffVecDeqAddLast(const MunitParameter params[], void *data) {
    intVecDeq *intVectDeq = NEW_VECTOR_DEQ_4(int);
    assert_true(intVecDeqAddLast(intVectDeq, 1));
    assert_true(intVecDeqAddLast(intVectDeq, 2));
    assert_true(intVecDeqAddLast(intVectDeq, 3));
    assert_true(intVecDeqAddLast(intVectDeq, 4));

    assert_false(intVecDeqAddLast(intVectDeq, 5));
    assertIntVecDeq(intVectDeq, 4, 4);

    intVecDeq *intVectDeq2 = NEW_VECTOR_DEQ_4(int);
    assert_true(intVecDeqAddLast(intVectDeq2, 1));
    assert_true(intVecDeqAddLast(intVectDeq2, 2));
    assert_true(intVecDeqAddFirst(intVectDeq2, 3));
    assert_true(intVecDeqAddFirst(intVectDeq2, 4));

    assert_false(intVecDeqAddLast(intVectDeq2, 5));
    assertIntVecDeq(intVectDeq2, 4, 4);
    return MUNIT_OK;
}

static MunitResult testBuffVecDeqGetFirst(const MunitParameter params[], void *data) {
    intVecDeq *intVectDeq = NEW_VECTOR_DEQ_4(int);
    assert_true(intVecDeqAddFirst(intVectDeq, 1));
    assert_int(1, ==, intVecDeqGetFirst(intVectDeq));

    assert_true(intVecDeqAddFirst(intVectDeq, 2));
    assert_true(intVecDeqAddFirst(intVectDeq, 3));
    assert_int(3, ==, intVecDeqGetFirst(intVectDeq));
    return MUNIT_OK;
}

static MunitResult testBuffVecDeqGetLast(const MunitParameter params[], void *data) {
    intVecDeq *intVectDeq = NEW_VECTOR_DEQ_4(int);
    assert_true(intVecDeqAddLast(intVectDeq, 1));
    assert_int(1, ==, intVecDeqGetLast(intVectDeq));

    assert_true(intVecDeqAddLast(intVectDeq, 2));
    assert_true(intVecDeqAddLast(intVectDeq, 3));
    assert_int(3, ==, intVecDeqGetLast(intVectDeq));
    return MUNIT_OK;
}

static MunitResult testBuffVecDeqRemoveFirst(const MunitParameter params[], void *data) {
    intVecDeq *intVectDeq = NEW_VECTOR_DEQ_4(int);
    intVecDeqAddFirst(intVectDeq, 1);
    intVecDeqAddFirst(intVectDeq, 2);

    assert_int(2, ==, intVecDeqRemoveFirst(intVectDeq));
    assertIntVecDeq(intVectDeq, 1, 4);
    assert_int(1, ==, intVecDeqGetFirst(intVectDeq));

    assert_int(1, ==, intVecDeqRemoveFirst(intVectDeq));
    assertIntVecDeq(intVectDeq, 0, 4);
    assert_int(0, ==, intVecDeqGetFirst(intVectDeq));

    assert_int(0, ==, intVectDeq->items[0]);
    assert_int(0, ==, intVectDeq->items[1]);
    assert_int(0, ==, intVectDeq->items[2]);
    assert_int(0, ==, intVectDeq->items[3]);
    return MUNIT_OK;
}

static MunitResult testBuffVecDeqRemoveLast(const MunitParameter params[], void *data) {
    intVecDeq *intVectDeq = NEW_VECTOR_DEQ_4(int);
    intVecDeqAddLast(intVectDeq, 1);
    intVecDeqAddLast(intVectDeq, 2);

    assert_int(2, ==, intVecDeqRemoveLast(intVectDeq));
    assertIntVecDeq(intVectDeq, 1, 4);
    assert_int(1, ==, intVecDeqGetLast(intVectDeq));

    assert_int(1, ==, intVecDeqRemoveLast(intVectDeq));
    assertIntVecDeq(intVectDeq, 0, 4);
    assert_int(0, ==, intVecDeqGetLast(intVectDeq));

    assert_int(0, ==, intVectDeq->items[0]);
    assert_int(0, ==, intVectDeq->items[1]);
    assert_int(0, ==, intVectDeq->items[2]);
    assert_int(0, ==, intVectDeq->items[3]);
    return MUNIT_OK;
}

static MunitResult testBuffVecDeqEmpty(const MunitParameter params[], void *data) {
    intVecDeq *intVectDeq = NEW_VECTOR_DEQ_4(int);
    assert_true(isintVecDeqEmpty(intVectDeq));
    assert_false(isintVecDeqNotEmpty(intVectDeq));

    intVecDeqAddFirst(intVectDeq, 1);
    assert_false(isintVecDeqEmpty(intVectDeq));
    assert_true(isintVecDeqNotEmpty(intVectDeq));

    intVecDeqAddFirst(intVectDeq, 2);
    intVecDeqAddFirst(intVectDeq, 3);
    assert_false(isintVecDeqEmpty(intVectDeq));
    assert_true(isintVecDeqNotEmpty(intVectDeq));

    intVecDeqClear(intVectDeq);
    assert_true(isintVecDeqEmpty(intVectDeq));
    assert_false(isintVecDeqNotEmpty(intVectDeq));

    assert_int(0, ==, intVectDeq->items[0]);
    assert_int(0, ==, intVectDeq->items[1]);
    assert_int(0, ==, intVectDeq->items[2]);
    assert_int(0, ==, intVectDeq->items[3]);
    return MUNIT_OK;
}

static MunitResult testBuffVecDeqContains(const MunitParameter params[], void *data) {
    intVecDeq *intVectDeq = NEW_VECTOR_DEQ_4(int);
    assert_false(intVecDeqContains(intVectDeq, 1));

    intVecDeqAddFirst(intVectDeq, 1);
    assert_true(intVecDeqContains(intVectDeq, 1));

    intVecDeqAddLast(intVectDeq, 2);
    assert_true(intVecDeqContains(intVectDeq, 2));
    return MUNIT_OK;
}

static MunitResult testBuffVecDeqRemoveFirstOccur(const MunitParameter params[], void *data) {
    intVecDeq *intVectDeq = NEW_VECTOR_DEQ_8(int);
    intVecDeqAddFirst(intVectDeq, 1);
    intVecDeqAddFirst(intVectDeq, 2);
    intVecDeqAddFirst(intVectDeq, 3);
    intVecDeqAddFirst(intVectDeq, 4);
    intVecDeqAddFirst(intVectDeq, 5);
    intVecDeqAddFirst(intVectDeq, 6);

    intVecDeqRemoveFirstOccur(intVectDeq, 6);
    assertIntVecDeq(intVectDeq, 5, 8);
    assert_int(1, ==, intVectDeq->items[7]);
    assert_int(2, ==, intVectDeq->items[6]);
    assert_int(3, ==, intVectDeq->items[5]);
    assert_int(4, ==, intVectDeq->items[4]);
    assert_int(5, ==, intVectDeq->items[3]);

    intVecDeqRemoveFirstOccur(intVectDeq, 3);
    assertIntVecDeq(intVectDeq, 4, 8);
    assert_int(1, ==, intVectDeq->items[7]);
    assert_int(2, ==, intVectDeq->items[6]);
    assert_int(4, ==, intVectDeq->items[5]);
    assert_int(5, ==, intVectDeq->items[4]);

    intVecDeqRemoveFirstOccur(intVectDeq, 8);
    assertIntVecDeq(intVectDeq, 4, 8);
    assert_int(1, ==, intVectDeq->items[7]);
    assert_int(2, ==, intVectDeq->items[6]);
    assert_int(4, ==, intVectDeq->items[5]);
    assert_int(5, ==, intVectDeq->items[4]);
    return MUNIT_OK;
}

static MunitResult testBuffVecDeqRemoveLastOccur(const MunitParameter params[], void *data) {
    intVecDeq *intVectDeq = NEW_VECTOR_DEQ_8(int);
    intVecDeqAddLast(intVectDeq, 1);
    intVecDeqAddLast(intVectDeq, 2);
    intVecDeqAddLast(intVectDeq, 3);
    intVecDeqAddLast(intVectDeq, 4);
    intVecDeqAddLast(intVectDeq, 5);
    intVecDeqAddLast(intVectDeq, 6);

    intVecDeqRemoveLastOccur(intVectDeq, 6);
    assertIntVecDeq(intVectDeq, 5, 8);
    assert_int(1, ==, intVectDeq->items[0]);
    assert_int(2, ==, intVectDeq->items[1]);
    assert_int(3, ==, intVectDeq->items[2]);
    assert_int(4, ==, intVectDeq->items[3]);
    assert_int(5, ==, intVectDeq->items[4]);

    intVecDeqRemoveLastOccur(intVectDeq, 3);
    assertIntVecDeq(intVectDeq, 4, 8);
    assert_int(1, ==, intVectDeq->items[0]);
    assert_int(2, ==, intVectDeq->items[1]);
    assert_int(4, ==, intVectDeq->items[2]);
    assert_int(5, ==, intVectDeq->items[3]);

    intVecDeqRemoveLastOccur(intVectDeq, 8);
    assertIntVecDeq(intVectDeq, 4, 8);
    assert_int(1, ==, intVectDeq->items[0]);
    assert_int(2, ==, intVectDeq->items[1]);
    assert_int(4, ==, intVectDeq->items[2]);
    assert_int(5, ==, intVectDeq->items[3]);
    return MUNIT_OK;
}


static MunitTest bufferVectorDeqTests[] = {
        {.name =  "Test new VectorDeq - should correctly create and init vector", .test = testBuffVecDeqCreation},
        {.name =  "Test <type>VecDeqAddFirst() - should correctly add elements to vector head", .test = testBuffVecDeqAddFirst},
        {.name =  "Test <type>VecDeqAddLast() - should correctly add elements to vector tail", .test = testBuffVecDeqAddLast},
        {.name =  "Test <type>VecDeqGetFirst() - should correctly get elements from vectors head", .test = testBuffVecDeqGetFirst},
        {.name =  "Test <type>VecDeqGetLast() - should correctly get elements from vectors tail", .test = testBuffVecDeqGetLast},
        {.name =  "Test <type>VecDeqRemoveFirst() - should correctly remove first vector element", .test = testBuffVecDeqRemoveFirst},
        {.name =  "Test <type>VecDeqRemoveLast() - should correctly remove last vector element", .test = testBuffVecDeqRemoveLast},
        {.name =  "Test <type>VecDeqRemoveLast() - should correctly remove last vector element", .test = testBuffVecDeqRemoveLast},
        {.name =  "Test is<type>VecDeqEmpty()/Not - should correctly check deq vector emptiness", .test = testBuffVecDeqEmpty},
        {.name =  "Test <type>VecDeqContains() - should check that vector have element", .test = testBuffVecDeqContains},
//        {.name =  "Test <type>VecDeqRemoveFirstOccur() - should remove head element and shift array", .test = testBuffVecDeqRemoveFirstOccur},
//        {.name =  "Test <type>VecDeqRemoveLastOccur() - should remove tail element and shift array", .test = testBuffVecDeqRemoveLastOccur},

        END_OF_TESTS
};

static const MunitSuite bufferVectorDeqTestSuite = {
        .prefix = "BufferVectorDeque: ",
        .tests = bufferVectorDeqTests,
        .suites = NULL,
        .iterations = 1,
        .options = MUNIT_SUITE_OPTION_NONE
};

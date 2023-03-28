#include "Collections/VectorTest.h"
#include "Collections/HashMapTest.h"
#include "Collections/BufferVectorTest.h"
#include "Collections/BufferHashMapTest.h"
#include "Collections/BufferHashSetTest.h"
#include "Collections/BufferVectorDequeTest.h"


int main(int argc, char *argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
    MunitTest emptyTests[] = {END_OF_TESTS};
    MunitSuite testSuitArray[] = {
            vectorTestSuite,
            bufferVectorTestSuite,
            hashMapTestSuite,
            bufferHashMapTestSuite,
            bufferHashSetTestSuite,
            bufferVectorDeqTestSuite
    };

    MunitSuite baseSuite = {
            .prefix = "",
            .tests = emptyTests,
            .suites = testSuitArray,
            .iterations = 1,
            .options = MUNIT_SUITE_OPTION_NONE
    };
    return munit_suite_main(&baseSuite, (void *) "µnit", argc, argv);
}

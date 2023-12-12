#include "Collections/VectorTest.h"
#include "Collections/BufferVectorTest.h"
#include "Collections/HashMapTest.h"
#include "Collections/BufferHashMapTest.h"
#include "Collections/BufferHashSetTest.h"
#include "Collections/BufferVectorDequeTest.h"


int main(int argc, char *argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
    MunitSuite testSuitArray[] = {
            vectorTestSuite,
            bufferVectorTestSuite,
            hashMapTestSuite,
            bufferHashMapTestSuite,
            bufferHashSetTestSuite,
            bufferVectorDeqTestSuite
    };

    for (int i = 0; i < ARRAY_SIZE(testSuitArray); i++) {
        int result = munit_suite_main(&testSuitArray[i], (void *) "µnit", argc, argv);
        if (result != EXIT_SUCCESS) return result;
    }
    return EXIT_SUCCESS;
}

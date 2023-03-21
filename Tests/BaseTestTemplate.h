#pragma once

#include <stdlib.h>
#include <string.h>
#include "Comparator.h"

#define MUNIT_ENABLE_ASSERT_ALIASES
#include "munit/munit.h"

#define END_OF_TESTS { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
#define END_OF_PARAMETERS {NULL, NULL}

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x)/sizeof((x)[0]))
#endif


static char *generateRandomString(size_t size) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJK...";
    char *string = malloc(size);
    if (string == NULL) return NULL;
    memset(string, 0, size);

    --size;
    for (size_t n = 0; n < size; n++) {
        int key = rand() % (int) (sizeof charset - 1);
        string[n] = charset[key];
    }
    string[size] = '\0';
    return string;
}

typedef struct User {   // Some test custom type
    char *name;
    int age;
} User;

int userAgeComparator(User one, User two) {    // users will be sorted by 'age'
    if (one.age == two.age) {
        return 0;
    } else if (one.age > two.age) {
        return 1;
    }
    return -1;
}

typedef User user;
typedef char *str;

CREATE_CUSTOM_COMPARATOR(user, User, one, two, strComparator(one.name, two.name));
CREATE_CUSTOM_HASH_CODE(user, User, user, strHashCode(user.name));

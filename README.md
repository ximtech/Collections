# Collections

[![tests](https://github.com/ximtech/Collections/actions/workflows/cmake-ci.yml/badge.svg)](https://github.com/ximtech/Collections/actions/workflows/cmake-ci.yml)
[![codecov](https://codecov.io/gh/ximtech/Collections/branch/main/graph/badge.svg?token=YKK6M0TWGO)](https://codecov.io/gh/ximtech/Collections)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/99e9fb7e754f4ffa95ae89b02eb75b08)](https://www.codacy.com/gh/ximtech/Collections/dashboard)

Lightweight implementation of Collections in ANSI C.\
Collection library provides efficient implementations of the most common general purpose programming data structures.

## Data Structures
1. [Vector](#vector)
2. [HashMap](#HashMap)
3. [HashSet](#HashSet)

### Features

- Specifically designed for embedded applications
- Stack and Heap implementations
- Easy to use and add to the project
- Performance optimized
- Low memory consumption

### Add as CPM project dependency

How to add CPM to the project, check the [link](https://github.com/cpm-cmake/CPM.cmake)

```cmake
CPMAddPackage(
        NAME Collections
        GITHUB_REPOSITORY ximtech/Collections
        GIT_TAG origin/main)

target_link_libraries(${PROJECT_NAME} Collections)
```

```cmake
add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})
# For Clion STM32 plugin generated Cmake use 
target_link_libraries(${PROJECT_NAME}.elf Collections)
```

## Vector

### Stack Vector

Fixed size of generic type `Vector`. This implementation not allocate heap memory, but need compile time known capacity

### Single header include

```c
#include "BufferVector.h"
```

### Define Vector type

Provide: 
1. Vector elements stored type
2. Optionally: Vector alias, if not provided then type will be used as prefix name
3. Elements comparator function pointer(for most standard data types it generates automatically)
Example:
```c
CREATE_VECTOR_TYPE(int);   // creates `intVector` type that holds int elements, with default numeric comparator
CREATE_VECTOR_TYPE(uint32_t, u32);   // creates with alias `u32Vector` type and default comparator
CREATE_VECTOR_TYPE(User, user, userAgeComparator); // alias will be used for custom type prefix: userVector
CREATE_VECTOR_TYPE(char*, cStr, strComparator); // `cStrVector`, predefined comparator from `Comparator.h`
```

#### Best practices

Use `typedef` for custom or pointer type
```c
typedef char* str;
CREATE_VECTOR_TYPE(str);    // creates `strVector` type and auto assigns `strComparator()`
```

#### Natural ordering comparator

For ordering string in natural order `strNaturalSortComparator()` can be used
```c
typedef char* str;
CREATE_VECTOR_TYPE(str, str, strNaturalSortComparator);
```

#### Custom comparator creation example

For custom types provide comparator function as in example:

```c
typedef struct User {   // Some custom type
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

int userNameComparator(User one, User two) {    // users will be sorted by 'name'
    return strcmp(one.name, two.name);
}
```

For inline function generation `CREATE_CUSTOM_COMPARATOR` macro can be used \
Provide:

1. Comparator function prefix name, this creates function like: <prefix>Comparator()
2. Your type
3. First and second function parameter name
4. Comparator logic

```c
CREATE_CUSTOM_COMPARATOR(userName, User, one, two, strcmp(one.name, two.name)); // creates `int userNameComparator(User one, User two)`
```


### Method and type naming conventions

1. When only type provided, then methods will be generated like: `<type>Vec...()` and `is<type>Vec...()` with prefix
2. When type alias(name) provided, then methods will be generated like: `<name>Vec...()` and `is<name>Vec...()` with prefix
3. Vector `typedef` naming: `<type>Vector` and for type alias `<name>Vector`


### Base vector creation
When comparator and all needed types provided, then `Vector` can be instantiated
```c
intVector *intVec1 = NEW_VECTOR(int, 4); // empty vector with max length of 4
intVector *intVec2 = NEW_VECTOR_8(int); // shorter version from predefined macro
intVector *intVec3 = NEW_VECTOR_128(int); // bigger, check available defines in `BufferVector.h'

intVector *intVec4 = VECTOR(int, 1, 2, 3, 4, 5); // auto size resolving for 5 elements
intVector *intVec5 = NEW_VECTOR_OF(32, int, int, 1, 2, 3, 4, 5); // vector with capacity of 32 and 5 elements of `int` type

cStrVector *strVec1 = VECTOR_OF(char*, cStr, "a", "b", "c");    // with type name alias
strVector *strVec2 = VECTOR(str, "one", "two", "three");  // shortcut with custom `typedef`

userVector *userVec = VECTOR_OF(user, User, {.name = "name_1", .age = 23}); // custom type init
userVector *userVec2 = VECTOR(user, {"name_1", 23});  // shortcut with custom `typedef`
```

### Add elements

***NOTE***: method `<type_name>VecAdd()` return `true` if element has been added or `false` when vector is full

```c
strVector *strVec = NEW_VECTOR_4(str);
strVecAdd(strVec, "aaa");
strVecAdd(strVec, "bbb");
strVecAdd(strVec, "ccc");
strVecAdd(strVec, "ddd");

assert(strVecAdd(strVec, "eee") == false); // vector is full, return false
```

### Get elements
```c
intVector *intVec = VECTOR(int, 1, 2, 3, 4);
intVecGet(intVec, 0);   // 1
intVecGet(intVec, 1);   // 2
intVecGet(intVec, 2);   // 3
intVecGet(intVec, 3);   // 4

intVecGet(intVec, 4);   // accessing index that is out of bounds, returns default for type '0'
intVecGetOrDefault(intVec, 5, 12345);   // return '12345'
```

### Put element at specific index
```c
intVector *intVec = VECTOR(int, 1, 2, 3, 4);
intVecPut(intVec, 1, 99);   // [1], [99], [3], [4]
```

### Add element at index
```c
intVector *intVec = NEW_VECTOR_OF(8, int, int, 1, 2, 3, 4);
intVecAddAt(intVec, 0, 21); // [21], [1], [2], [3], [4]
intVecAddAt(intVec, 5, 22); // [21], [1], [2], [3], [4], [22]
intVecAddAt(intVec, 2, 23); // [21], [1], [23], [2], [3], [4], [22]
```
### Remove element at index
```c
intVector *intVec = VECTOR(int, 1, 2, 3, 4, 5);
intVecRemoveAt(intVec, 0); // [2], [3], [4], [5]
intVecRemoveAt(intVec, 1); // [2], [4], [5]
intVecRemoveAt(intVec, 2); // [2], [4]
```

### Check that vector is empty or not
```c
charVector *cVec = VECTOR(char, 'a', 'b', 'c');
assert(ischarVecEmpty(cVec) == false);  // not empty
assert(ischarVecNotEmpty(cVec) == true);

charVecClear(cVec);
assert(ischarVecEmpty(cVec) == true);  // empty
assert(ischarVecNotEmpty(cVec) == false);
```

### Vector element count
```c
charVector *cVec = VECTOR(char, 'a', 'b', 'c');
printf("[%d]", charVecSize(cVec)); // [3]
```

### Remove all elements from vector
```c
charVector *cVec = VECTOR(char, 'a', 'b', 'c');
charVecClear(cVec);
printf("[%d]", charVecSize(cVec)); // [0]
```

### Add all elements from one vector to other
**NOTE:** Ensure that destination vector have enough space for all elements from source vector
```c
charVector *cVec = NEW_VECTOR_OF(8, char, char, 'a', 'b', 'c');
charVector *cVec2 = VECTOR(char, 'd', 'e', 'f');
assert(charVecAddAll(cVec, cVec2) == true); // cVec -> [a], [b], [c], [d], [e], [f], if lack of capacity then returns false
```

### Add elements to vector from array
```c
int array[] = {1, 2, 3, 4, 5};
intVector *intVec = NEW_VECTOR_8(int);
intVecFromArray(intVec, array, 5);  // intVec -> [1], [2], [3], [4], [5]
assert(intVecSize(intVec) == 5);
```

### Find element index in vector
```c
cStrVector *strVec = NEW_VECTOR_4(cStr, char*);
cStrVecAdd(strVec, "aaa");
cStrVecAdd(strVec, "bbb");
cStrVecAdd(strVec, "ccc");
cStrVecAdd(strVec, "ddd");

assert(cStrVecIndexOf(strVec, "bbb") == 1);
assert(cStrVecIndexOf(strVec, "eee") == -1); // not found 
```

### Check that element is in vector
```c
cStrVector *strVec = VECTOR_OF(char*, cStr, "a", "b", "c", "d");
assert(cStrVecContains(strVec, "c") == true);
assert(cStrVecContains(strVec, "e") == false);
```

### Reverse elements in vector
```c
cStrVector *strVec = VECTOR_OF(char*, cStr, "a", "b", "c", "d");
cStrVecReverse(strVec); // [d], [c], [b], [a]
```

### Sort elements
```c
cStrVector *strVec = VECTOR_OF(char*, cStr, "b", "a", "d", "c");
cStrVecSort(strVec); // [a], [b], [c], [d]
```

### Check for two vector equality
```c
charVector *cVec = VECTOR(char, 'a', 'b', 'c');
charVector *cVec2 = VECTOR(char, 'a', 'b', 'c');
assert(ischarVecEquals(cVec, cVec2) == true);
```

### Remove duplicated values
```c
cStrVector *strVec = VECTOR_OF(char*, cStr, "a", "b", "c", "a", "a", "a", "c");
cStrVecRemoveDup(strVec); // [a], [b], [c]
```

### Vector Union
```c
cStrVector *strVec = NEW_VECTOR_OF(12, char*, cStr, "1", "2", "3", "4", "5", "6");
cStrVector *strVec2 = VECTOR_OF(char*, cStr, "2", "3", "6", "7", "8", "9");
cStrVecUnion(strVec, strVec2);  // strVec -> [1], [2], [3], [4], [5], [6], [7], [8], [9]
```

**Shortcut:** Copy all elements to empty vector from other
```c
cStrVector *strVec = cStrVecUnion(NEW_VECTOR_16(char*, cStr), strVec2);
```

### Vector Intersection
```c
cStrVector *strVec = NEW_VECTOR_OF(12, char*, cStr, "A", "B", "C", "D", "E", "F");
cStrVector *strVec2 = VECTOR_OF(char*, cStr, "B", "D", "F", "G", "H", "K");
cStrVecIntersect(strVec, strVec2);  // strVec -> [B], [D], [F]
```

### Vector Subtraction
```c
cStrVector *strVec = NEW_VECTOR_OF(12, char*, cStr, "1", "2", "3", "4", "5", "6");
cStrVector *strVec2 = VECTOR_OF(char*, cStr, "2", "3", "6", "7", "8", "9");
cStrVecSubtract(strVec, strVec2);  // strVec -> [1], [4], [5]
```

### Vector Disjunction
```c
cStrVector *strVec = NEW_VECTOR_OF(12, char*, cStr, "1", "2", "3", "4", "5", "6");
cStrVector *strVec2 = VECTOR_OF(char*, cStr, "2", "3", "6", "7", "8", "9");
cStrVecDisjunction(strVec, strVec2);  // strVec -> [1], [4], [5], [7], [8], [9]
```

### Heap Vector

This version uses generic type of `void*` and uses heap memory allocation increasing inner array size

Example of usage:

```C
Vector vector = getVectorInstance(4);   // create instance
vectorAdd(vector, 1);   // add some elements
vectorAdd(vector, 2);
vectorAdd(vector, 3);

printf("Size: %d\n", getVectorSize(vector)); // 3
for (int i = 0; i < getVectorSize(vector); i++) {
    printf("%d ", vectorGet(vector, i)); // 1 2 3
}
printf("\n");

vectorAdd(vector, 5);   // add more elements
vectorAdd(vector, 6);
printf("Added size: %d\n", getVectorSize(vector)); // size is increased to 5

vectorAddAt(vector, 3, 4);  // insert in the middle
printf("New size: %d\n", getVectorSize(vector)); // 6
for (int i = 0; i < getVectorSize(vector); i++) {
    printf("%d ", vectorGet(vector, i)); // 1 2 3 4 5 6
}
printf("\n");

vectorRemoveAt(vector, 2);  // remove some element
for (int i = 0; i < getVectorSize(vector); i++) {
    printf("%d ", vectorGet(vector, i)); // 1 2 4 5 6
}
printf("\n");

vectorPut(vector, 0, 0);    // override value
for (int i = 0; i < getVectorSize(vector); i++) {
    printf("%d ", vectorGet(vector, i)); // 0 2 4 5 6
}
printf("\n");

vectorClear(vector);
printf("Is Vector has elements: %s\n", isVectorNotEmpty(vector) ? "Yes" : "No");    // No
printf("Is Vector empty: %s\n", isVectorEmpty(vector) ? "Yes" : "No");              // Yes

vectorDelete(vector);
```

## HashMap

Stores the data in (Key, Value) pairs, and can be accessible by an index of another type (e.g. an `char*`). 
While insert the duplicate key, it will replace the element of the corresponding key.

### Stack HashMap

Fixed size of generic type `Map`. This implementation not allocate heap memory, but need compile time known capacity

### Single header include

```c
#include "BufferHashMap.h"
```

### Define HashMap type

Provide:
1. Map key type
2. Map value type
3. Optionally: Map Key/Value alias, if not provided then type will be used as prefix name
4. Key comparator and hashCode function pointer(for most standard data types it generates automatically)
   Example:
```c
CREATE_HASH_MAP_TYPE(int, int);   // creates `int_intMap` type that stores `int` key type and `int` values, with default numeric key comparator and hashCode
CREATE_HASH_MAP_TYPE(int8_t, float, i8);    // creates `i8_floatMap` with key name alias and default key comparator/hashCode
CREATE_HASH_MAP_TYPE(char*, int, str, strComparator, strHashCode);  // custom comparator and hashCode functions
CREATE_HASH_MAP_TYPE(char*, char*, cStr, cStr, strComparator, strHashCode);   // full customization: `cStr_cStrMap` with aliases for key and values and custom key comparator and hashCode
```

#### Best practices

C strings(`char*`) as Hash Map keys is widely used, so for shortcut declare `typedef` with name `str`
```c
typedef char* str;
CREATE_HASH_MAP_TYPE(str, str);    // creates `str_strMap` type and auto assigns predefined `strComparator()` and `strHashCode()`
```

#### Custom type as a key
```c
typedef struct User {   // Some custom type
    char *name;
    int age;
} User;

CREATE_CUSTOM_COMPARATOR(user, User, one, two, strComparator(one.name, two.name)); // creates inline comparator `userComparator()` function, used for user identity
CREATE_CUSTOM_HASH_CODE(user, User, user, strHashCode(user.name));    // inline hash code function creation: `userHashCode()`

typedef User user;
CREATE_HASH_MAP_TYPE(user, int);    // auto assigns previous declared comparator and hash code functions
```

### Method and type naming conventions

1. When only type provided, then methods will be generated like: `<key_type>_<value_type>Map...()` and `is_<key_type>_<value_type>Map...()` with prefix
2. When type alias(name) provided, then methods will be generated like: `<key_name>_<value_name>Map...()` and `is_<key_name>_<value_name>Map...()` with prefix
3. Map `typedef` naming: `<key_type>_<value_type>Map` and for type alias `<key_name>_<value_name>Map`


### Base Map creation

When comparator/hashCode and all needed types provided, then `Map` can be instantiated
```c
int_intMap *intMap1 = NEW_HASH_MAP(int, int, 4); // empty Map with max length of 4
str_strMap *srtMap2 = NEW_HASH_MAP_8(str, str); // shorter version from predefined macro
str_strMap *srtMap3 = NEW_HASH_MAP_128(str, str); // bigger, check available defines in `BufferHashMap.h'

str_strMap *strMap5 = HASH_MAP_OF(str, str, {"k1", "v1"}, {"k2", "v2"}, {"k3", "v3"}); // auto size resolving for 3 pairs
i8_floatMap *i8Map = NEW_HASH_MAP_OF(5, i8, float, {1, 1.1}, {2, 2.2}, {3, 3.3});  // map with capacity for 5 key/value pairs(will be aligned to (5 + 1) * 2) and values
cStr_cStrMap *strMap4 = NEW_HASH_MAP_OF(3, cStr, cStr, {"k1", "v1"}, {"k2", "v2"}, {"k3", "v3"}); // key/value types with aliases

user_intMap *usrMap = HASH_MAP_OF(user, int, {{ "first", 23 }, 123 }, {{ "second", 18 }, 456 });   // custom type as a key and `int` as value
```

### Add elements
```c
str_strMap *strMap = NEW_HASH_MAP_4(str, str);
str_strMapAdd(strMap, "key_1", "value_1");
str_strMapAdd(strMap, "key_2", "value_2");
str_strMapAdd(strMap, "key_3", "value_3");
str_strMapAdd(strMap, "key_3", "value_33"); // same key overwrite value
str_strMapAdd(strMap, "key_4", "value_4");

assert(str_strMapAdd(strMap, "key_5", "value_5") == false); // `false` when out of capacity
```

### Get elements
```c
i8_floatMap *i8Map = NEW_HASH_MAP_OF(23, i8, float, { 1, 1.1 }, { 2, 2.2 }, { 3, 3.3 });
i8_floatMapGet(i8Map, 1);   // 1.1
i8_floatMapGet(i8Map, 2);   // 2.2
i8_floatMapGet(i8Map, 3);   // 3.3

assert(i8_floatMapGet(i8Map, 4) == 0.0); // return default value when key not found. For int -> 0, float -> 0.0, char* -> NULL
assert(i8_floatMapGetOrDefault(i8Map, 4, -1.0) == -1.0); // returns provided default value when key is not found
```

### Remove element
```c
str_strMap *strMap = HASH_MAP_OF(str, str, { "k1", "v1" }, { "k2", "v2" }, { "k3", "v3" });
str value = str_strMapRemove(strMap, "k2");
assert(value == "v2");
```

### Add all elements from other map

**NOTE:** make sure that destination map have enough capacity to store all values
```c
str_strMap *strMap1 = HASH_MAP_OF(str, str, { "k1", "v1" }, { "k2", "v2" }, { "k3", "v3" });
str_strMap *strMap2 = NEW_HASH_MAP_OF(12, str, str, {"k4", "v4"}, {"k5", "v5"});
str_strMapAddAll(strMap1, strMap2); // strMap2 -> [k2]:[v2], [k5]:[v5], [k1]:[v1], [k3]:[v3], [k4]:[v4]
```

### Remove all elements
```c
str_strMap *strMap = HASH_MAP_OF(str, str, { "k1", "v1" }, { "k2", "v2" }, { "k3", "v3" });
assert(str_strMapSize(strMap) == 3);
str_strMapClear(strMap);
assert(str_strMapSize(strMap) == 0);
```

### Elements count
```c
int_charMap *charMap = NEW_HASH_MAP_OF(12, int, char, { 1, 'a' }, { 2, 'b' }, { 3, 'c' });
printf("Map size: [%d]\n", int_charMapSize(charMap));   // Map size: [3]
```

### Check that Map is empty or not
```c
int_charMap *charMap = HASH_MAP_OF(int, char, { 1, 'a' }, { 2, 'b' }, { 3, 'c' });
assert(is_int_charMapEmpty(charMap) == false);
assert(is_int_charMapNotEmpty(charMap) == true);

int_charMapClear(charMap);
assert(is_int_charMapEmpty(charMap) == true);
assert(is_int_charMapNotEmpty(charMap) == false);
```

### Check that Map contains key
```c
str_strMap *strMap = HASH_MAP_OF(str, str, { "k1", "v1" }, { "k2", "v2" }, { "k3", "v3" });
assert(str_strMapContains(strMap, "k1") == true);
assert(str_strMapContains(strMap, "k4") == false);
```

### Iterate Map keys and values
```c
int_charMap *charMap = HASH_MAP_OF(int, char, { 1, 'a' }, { 2, 'b' }, { 3, 'c' });
int_charMapIterator iter = int_charMapIter(charMap);
while (int_charMapHasNext(&iter)) {
  printf("Key: [%d], Value: [%c]\n", iter.key, iter.value);
}
```

### Heap HashMap

This version uses generic type of `void*` for values and `char*` for keys. Uses heap memory allocation increasing inner array size

Example of usage:
```C
HashMap hashMap = getHashMapInstance(4);    // create first Map
hashMapPut(hashMap, "1", "one");
hashMapPut(hashMap, "2", "two");
hashMapPut(hashMap, "3", "three");  // add some values

printf("Map size: %d\n", getHashMapSize(hashMap));  // 3
printf("Single value: %s\n", hashMapGet(hashMap, "2")); // two

HashMapIterator iterator = getHashMapIterator(hashMap); // loop all keys and values
while (hashMapHasNext(&iterator)) {
    printf("Key: [%s], Value: [%s]\n", iterator.key, iterator.value);
}

hashMapRemove(hashMap, "1");    // element with key "1" will be removed from Map
printf("New size: %d\n", getHashMapSize(hashMap));  // 2

HashMap hashMap2 = getHashMapInstance(4);   // create second Map
hashMapPut(hashMap2, "4", "four");
hashMapPut(hashMap2, "5", "five");

hashMapAddAll(hashMap, hashMap2);   // merge two maps
iterator = getHashMapIterator(hashMap2);
while (hashMapHasNext(&iterator)) {
    printf("Key: [%s], Value: [%s]\n", iterator.key, iterator.value);
}
hashMapDelete(hashMap);
hashMapDelete(hashMap2);
```

## HashSet

Same as `HashMap`, but only stores unique values as keys in `HashMap`.
No guarantee is made as to the iteration order of the set which means that the implementation does not guarantee the constant order of elements over time.

### Stack HashSet

Fixed size of generic type `HashSet`. This implementation not allocate heap memory, but need compile time known capacity

### Single header include

```c
#include "BufferHashSet.h"
```

### Define HashSet type

Provide:
1. Set key type
3. Optionally: Set value alias, if not provided then type will be used as prefix name
4. Value comparator and hashCode function pointer(for most standard data types it generates automatically)
   Example:
```c
CREATE_HASH_SET_TYPE(int);   // creates `intHashSet` type that stores `int` values, with default numeric key comparator and hashCode
CREATE_HASH_SET_TYPE(int8_t, i8);    // creates `i8HashSet` with value name alias and default comparator/hashCode
CREATE_HASH_SET_TYPE(char*, str, strComparator, strHashCode);  // custom comparator and hashCode functions
```

#### Best practices

Use `typedef` for shortcut configuration. Strings of `char*` is widely used so, there is a predefined functions
```c
typedef char* str;
CREATE_HASH_SET_TYPE(str);    // creates `strHashSet` type and auto assigns predefined `strComparator()` and `strHashCode()`
```

#### Custom type as Set value
```c
typedef struct User {   // Some custom type
    char *name;
    int age;
} User;

CREATE_CUSTOM_COMPARATOR(user, User, one, two, strComparator(one.name, two.name)); // creates inline comparator `userComparator()` function, used for user identity
CREATE_CUSTOM_HASH_CODE(user, User, user, strHashCode(user.name));    // inline hash code function creation: `userHashCode()`

typedef User user;
CREATE_HASH_SET_TYPE(user); // auto assigns previous declared comparator and hash code functions
```

### Method and type naming conventions

1. When only type provided, then methods will be generated like: `<value_type>Set...()` and `is<value_type>Set...()` with prefix
2. When type alias(name) provided, then methods will be generated like: `<value_name>Set...()` and `is<value_name>Set...()` with prefix
3. HashSet `typedef` naming: `<type>HashSet` and for type alias `<name>HashSet`

### Base Set creation

When comparator/hashCode and all needed types provided, then `HashSet` can be instantiated
```c
strHashSet *strSet = NEW_HASH_SET(str, 4); // empty Set with max length of 4
intHashSet *intSet = NEW_HASH_SET_8(int); // shorter version from predefined macro
i8HashSet *i8Set = NEW_HASH_SET_128(i8); // bigger, check available defines in `BufferHashSet.h'

floatHashSet *floatSet = HASH_SET_OF(float, {1.1}, {2.2}, {3.3}); // auto size resolving for 3 elements
strHashSet *strSet1 = HASH_SET_OF(str, {"one"}, {"two"}, {"three"}, {"four"}, {"one"}, {"one"});   // duplicates will be removed
strHashSet *strSet2 = NEW_HASH_SET_OF(5, str, {"v1"}, {"v2"}, {"v3"});  // Set with capacity for 5 values. The capacity will be aligned to (5 + 1) * 2 = 12
userHashSet *userSet = HASH_SET_OF(user, {"name_1", 23}, {"name_2", 13}); // custom type set
```

### Add elements
```c
strHashSet *strSet = NEW_HASH_SET_4(str);
strSetAdd(strSet, "value_1");
strSetAdd(strSet, "value_2");
strSetAdd(strSet, "value_3");
strSetAdd(strSet, "value_3"); // same value overwrite
strSetAdd(strSet, "value_4");

assert(strSetAdd(strSet, "value_5") == false); // `false` when out of capacity
```

### Remove element
```c
strHashSet *strSet = HASH_SET_OF(str, { "v1" }, { "v2" }, { "v3" });
assert(strSetRemove(strMap, "k2") == true);
```

### Add all elements from other Set

**NOTE:** make sure that destination HashSet have enough capacity to store all values
```c
strHashSet *strSet1 = HASH_SET_OF(str, { "v1" }, { "v2" }, { "v3" });
strHashSet *strSet2 = NEW_HASH_SET_OF(12, str, {"v4"}, {"v5"});
strSetAddAll(strSet1, strSet2); // strSet2 -> [v2], [v5], [v1], [v3], [v4]
```

### Remove all elements
```c
strHashSet *strSet = HASH_SET_OF(str, { "v1" }, { "v2" }, { "v3" });
assert(strSetSize(strSet) == 3);
strSetClear(strSet);
assert(strSetSize(strSet) == 0);
```

### Elements count
```c
charHashSet *charSet = NEW_HASH_SET_OF(12, char, { 'a' }, { 'b' }, { 'c' });
printf("Set size: [%d]\n", charSetSize(charSet));   // Set size: [3]
```

### Check that Set is empty or not
```c
charHashSet *charSet = HASH_SET_OF(char, { 'a' }, { 'b' }, { 'c' });
assert(ischarSetEmpty(charSet) == false);
assert(ischarSetNotEmpty(charSet) == true);
charSetClear(charSet);
assert(ischarSetEmpty(charSet) == true);
assert(ischarSetNotEmpty(charSet) == false);
```

### Check that Set contains value
```c
strHashSet *strSet = HASH_SET_OF(str, { "v1" }, { "v2" }, { "v3" });
assert(strSetContains(strSet, "v1") == true);
assert(strSetContains(strSet, "v4") == false);
```

### Check that Set contains all values from other Set
```c
strHashSet *strSet1 = HASH_SET_OF(str, { "v1" }, { "v2" }, { "v3" });
strHashSet *strSet2 = HASH_SET_OF(str, { "v1" }, { "v3" });
assert(strSetContainsAll(strSet1, strSet2) == true);
```

### Iterate Set values
```c
charHashSet *charSet = HASH_SET_OF(char, { 'a' }, { 'b' }, { 'c' });
charSetIterator iter = charSetIter(charSet);
while (charSetHasNext(&iter)) {
  printf("Value: [%c]\n", iter.value);
}
```
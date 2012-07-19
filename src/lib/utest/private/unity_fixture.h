//- Copyright (c) 2010 James Grenning and Contributed to Unity Project
/* ==========================================
    Unity Project - A Test Framework for C
    Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
    [Released under MIT License. Please refer to license.txt for details]
========================================== */

#ifndef UNITY_FIXTURE_H_
#define UNITY_FIXTURE_H_

#include "./utest/private/unity.h"
#include "./utest/private/unity_internals.h"
#include "./utest/private/unity_fixture_internals.h"

void run_utest_main(void (*runAllTests)());

#define TEST_GROUP(group)\
    int TEST_GROUP_##group = 0

#define TEST_SETUP(group) void TEST_##group##_SETUP()

#define TEST_TEAR_DOWN(group) void TEST_##group##_TEAR_DOWN()


#define TEST(group, name) \
    void TEST_##group##_##name##_();\
    void TEST_##group##_##name##_run()\
    {\
        UnityTestRunner(TEST_##group##_SETUP,\
             TEST_##group##_##name##_,\
            TEST_##group##_TEAR_DOWN,\
            "TEST(" #group ", " #name ")",\
            #group, #name,\
            __FILE__, __LINE__);\
    }\
    void  TEST_##group##_##name##_()

#define IGNORE_TEST(group, name) \
    void TEST_##group##_##name##_();\
    void TEST_##group##_##name##_run()\
    {\
        UnityIgnoreTest();\
    }\
    void  TEST_##group##_##name##_()

#define DECLARE_TEST_CASE(group, name) \
    void TEST_##group##_##name##_run()

#define RUN_TEST_CASE(group, name) \
        DECLARE_TEST_CASE(group, name);\
    TEST_##group##_##name##_run();

//This goes at the bottom of each test file or in a separate c file
#define TEST_GROUP_RUNNER(group)\
    void TEST_##group##_GROUP_RUNNER_runAll();\
    void TEST_##group##_GROUP_RUNNER()\
    {\
        TEST_##group##_GROUP_RUNNER_runAll();\
    }\
    void TEST_##group##_GROUP_RUNNER_runAll()

//Call this from main
#define RUN_TEST_GROUP(group)\
    void TEST_##group##_GROUP_RUNNER();\
    TEST_##group##_GROUP_RUNNER();

void UnityMalloc_MakeMallocFailAfterCount(int count);

#endif /* UNITY_FIXTURE_H_ */

//- Copyright (c) 2010 James Grenning and Contributed to Unity Project
/* ==========================================
    Unity Project - A Test Framework for C
    Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
    [Released under MIT License. Please refer to license.txt for details]
========================================== */

#include "./utest/private/unity_fixture.h"
#include "./utest/private/unity_internals.h"
#include "./utest/private/unity.h"
#include <string.h>

UNITY_FIXTURE_T UnityFixture;

//If you decide to use the function pointer approach.
int (*outputChar)(int) = putchar;

int verbose = 0;

void setUp(void)    { /*does nothing*/ }
void tearDown(void) { /*does nothing*/ }

void announceTestRun(int runNumber)
{
    UnityPrint("Unity test run ");
    UnityPrintNumber(runNumber+1);
    UnityPrint(" of ");
    UnityPrintNumber(UnityFixture.RepeatCount);
    UNITY_PRINT_EOL;
}

int UnityMain(void (*runAllTests)())
{
    UnityBegin();
    runAllTests();
    UNITY_PRINT_EOL;
	UnityEnd();

 //   return UnityFailureCount();
}

static int selected(const char * filter, const char * name)
{
    if (filter == 0)
        return 1;
    return strstr(name, filter) ? 1 : 0;
}

static int testSelected(const char* test)
{
    return selected(UnityFixture.NameFilter, test);
}

static int groupSelected(const char* group)
{
    return selected(UnityFixture.GroupFilter, group);
}

void UnityTestRunner(unityfunction* setup,
        unityfunction* testBody,
        unityfunction* teardown,
        const char * printableName,
        const char * group,
        const char * name,
        const char * file, int line)
{
    if (testSelected(name) && groupSelected(group))
    {
        Unity.CurrentTestFailed = 0;
        Unity.TestFile = file;
        Unity.CurrentTestName = printableName;
        Unity.CurrentTestLineNumber = line;

        UnityPrint(printableName);
		UNITY_PRINT_EOL;

        Unity.NumberOfTests++;
        setup();
        testBody();
        teardown();
    }
}

void UnityIgnoreTest()
{
    Unity.NumberOfTests++;
    Unity.CurrentTestIgnored = 1;
    UNITY_OUTPUT_CHAR('!');
}
/* AUTOGENERATED FILE. DO NOT EDIT. */
#ifndef _MOCK_BUTTON_H
#define _MOCK_BUTTON_H

#include "unity.h"
#include "Button.h"

/* Ignore the following warnings, since we are copying code */
#if defined(__GNUC__) && !defined(__ICC) && !defined(__TMS470__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && (__GNUC_MINOR__ > 6 || (__GNUC_MINOR__ == 6 && __GNUC_PATCHLEVEL__ > 0)))
#pragma GCC diagnostic push
#endif
#if !defined(__clang__)
#pragma GCC diagnostic ignored "-Wpragmas"
#endif
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wduplicate-decl-specifier"
#endif

void mock_Button_Init(void);
void mock_Button_Destroy(void);
void mock_Button_Verify(void);




#define readPhysicalButton_IgnoreAndReturn(cmock_retval) readPhysicalButton_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void readPhysicalButton_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, int cmock_to_return);
#define readPhysicalButton_StopIgnore() readPhysicalButton_CMockStopIgnore()
void readPhysicalButton_CMockStopIgnore(void);
#define readPhysicalButton_ExpectAndReturn(cmock_retval) readPhysicalButton_CMockExpectAndReturn(__LINE__, cmock_retval)
void readPhysicalButton_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, int cmock_to_return);
typedef int (* CMOCK_readPhysicalButton_CALLBACK)(int cmock_num_calls);
void readPhysicalButton_AddCallback(CMOCK_readPhysicalButton_CALLBACK Callback);
void readPhysicalButton_Stub(CMOCK_readPhysicalButton_CALLBACK Callback);
#define readPhysicalButton_StubWithCallback readPhysicalButton_Stub

#if defined(__GNUC__) && !defined(__ICC) && !defined(__TMS470__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && (__GNUC_MINOR__ > 6 || (__GNUC_MINOR__ == 6 && __GNUC_PATCHLEVEL__ > 0)))
#pragma GCC diagnostic pop
#endif
#endif

#endif
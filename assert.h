#ifndef ASSERTION_H
#define ASSERTION_H
#include "abort.h"
#include <stdio.h>
#ifndef ASSERTION_RANGES_DEFINED
#define ASSERTION_RANGES_DEFINED
// maximum buffer size for assertions status array
#define MAX_ASSERTIONS_BUFFER_SIZE 1024
#endif // ASSERTION_RANGES_DEFINED
static int assertion_do_stdout_ = 1; // assertion do print to stdout
// assertion do print to stdout setter
#define set_assertion_do_stdout_(value) assertion_do_stdout_ = value
// assertion do print to stdout getter
#define get_assertion_do_stdout_() assertion_do_stdout_
// assertion status constants
enum { ASSERTION_NOTRUN = -1,  ASSERTION_FAILED = 0,  ASSERTION_PASSED = 1 };
// assertion status variable
static int assertion_last_status_ = ASSERTION_NOTRUN;
// assertion status getter
#define get_last_assertion_status_() assertion_last_status_
// assertion status setter
#define set_last_assertion_status_(value) assertion_last_status_ = value
// assertion status is failed
#define assertion_failed_(status) (status == ASSERTION_FAILED)
// assertion status is passed
#define assertion_passed_(status) (status == ASSERTION_PASSED)
// assertion status is not run
#define assertion_notrun_(status) (status == ASSERTION_NOTRUN)
// assertion status is failed
#define last_assertion_failed_() (assertion_last_status_ == ASSERTION_FAILED)
// assertion status is passed
#define last_assertion_passed_() (assertion_last_status_ == ASSERTION_PASSED)
// assertion status is not run
#define last_assertion_notrun_() (assertion_last_status_ == ASSERTION_NOTRUN)
// auto abortion if failed assertion
static int assertion_auto_abort_  = 0;
// auto abort setter
#define set_assertion_auto_abort_(value) assertion_auto_abort_ = value
// auto abort getter
#define get_assertion_auto_abort_() assertion_auto_abort_
// assertion status structure
struct S_AssertionStatus {
    int status;
    const char* expression;
    const char* file;
    unsigned long line;
};
static struct S_AssertionStatus assertion_statuses_[MAX_ASSERTIONS_BUFFER_SIZE]; // assertions status array
static int assertion_count_ = 0; // assertions count (max is MAX_ASSERTIONS_BUFFER_SIZE)
static int assertion_failed_count_ = 0; // failed assertions count
static int assertion_passed_count_ = 0; // passed assertions count
// abort if assertion passed variable is $op to $constant
#define assertion_abort_if_(op, constant) \
    if (assertion_last_status_ op constant) force_abort_();
// abort if last assertion failed
#define assertion_abort_if_failed_() abort_if_(==, ASSERTION_FAILED)
// abort if last assertion passed
#define assertion_abort_if_passed_() abort_if_(==, ASSERTION_PASSED)
// abort if last assertion notrun
#define assertion_abort_if_notrun_() abort_if_(==, ASSERTION_NOTRUN)
// abort if last assertion is not failed
#define assertion_abort_if_not_failed_() abort_if_(!=, ASSERTION_FAILED)
// abort if last assertion is not passed
#define assertion_abort_if_not_passed_() abort_if_(!=, ASSERTION_PASSED)
// abort if last assertion is not notrun
#define assertion_abort_if_not_notrun_() abort_if_(!=, ASSERTION_NOTRUN)
// assert if expression is true (opposite of assert_if_not_)
#define assert_if_(expr) \
    if (expr) { \
        assertion_last_status_ = ASSERTION_FAILED; \
        assertion_failed_count_++; \
        if (assertion_do_stdout_ || assertion_auto_abort_) { \
            fprintf(stderr, "Assertion failed: %s, file %s, line %d\n", #expr, __FILE__, __LINE__); \
        } \
        if (assertion_auto_abort_) { \
            abort_(); \
        } \
    } else { \
        assertion_last_status_ = ASSERTION_PASSED; \
        assertion_passed_count_++; \
    } \
    if (assertion_count_ < MAX_ASSERTIONS_BUFFER_SIZE) { \
        assertion_statuses_[assertion_count_++].status = assertion_last_status_; \
        assertion_statuses_[assertion_count_].expression = #expr; \
        assertion_statuses_[assertion_count_].file = __FILE__; \
        assertion_statuses_[assertion_count_].line = __LINE__; \
    } else { \
        fprintf(stderr, "Assertion buffer overflow at file %s, line %d\n", __FILE__, __LINE__); \
        force_abort_(); \
    }
// assert if expression is false (opposite of assert_if_)
#define assert_if_not_(expr) assert_if_(!(expr)) \
// default assert (assert if expression is false) (opposite of assert_if_)
#define assert_(expr) assert_if_not_(expr)
#endif // ASSERTION_H
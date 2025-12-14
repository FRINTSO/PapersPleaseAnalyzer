/*
 * test.h - Minimal test assertions, no framework bullshit
 *
 * Each test is a standalone executable. Returns 0 on success, non-zero on failure.
 * That's it. No magic, no dependency injection, no test discovery.
 */

#ifndef PAPLEASE_TEST_H
#define PAPLEASE_TEST_H

#include <cstdio>
#include <cstdlib>
#include <cstring>

#define TEST_ASSERT(cond) do { \
    if (!(cond)) { \
        fprintf(stderr, "FAIL: %s:%d: %s\n", __FILE__, __LINE__, #cond); \
        return 1; \
    } \
} while(0)

#define TEST_ASSERT_EQ(a, b) do { \
    auto _a = (a); \
    auto _b = (b); \
    if (_a != _b) { \
        fprintf(stderr, "FAIL: %s:%d: %s != %s\n", __FILE__, __LINE__, #a, #b); \
        return 1; \
    } \
} while(0)

#define TEST_ASSERT_NE(a, b) do { \
    auto _a = (a); \
    auto _b = (b); \
    if (_a == _b) { \
        fprintf(stderr, "FAIL: %s:%d: %s == %s (expected different)\n", __FILE__, __LINE__, #a, #b); \
        return 1; \
    } \
} while(0)

#define TEST_ASSERT_STR_EQ(a, b) do { \
    const char* _a = (a); \
    const char* _b = (b); \
    if (strcmp(_a, _b) != 0) { \
        fprintf(stderr, "FAIL: %s:%d: \"%s\" != \"%s\"\n", __FILE__, __LINE__, _a, _b); \
        return 1; \
    } \
} while(0)

#define TEST_ASSERT_GT(a, b) do { \
    auto _a = (a); \
    auto _b = (b); \
    if (!(_a > _b)) { \
        fprintf(stderr, "FAIL: %s:%d: %s not > %s\n", __FILE__, __LINE__, #a, #b); \
        return 1; \
    } \
} while(0)

#define TEST_ASSERT_GE(a, b) do { \
    auto _a = (a); \
    auto _b = (b); \
    if (!(_a >= _b)) { \
        fprintf(stderr, "FAIL: %s:%d: %s not >= %s\n", __FILE__, __LINE__, #a, #b); \
        return 1; \
    } \
} while(0)

#define TEST_ASSERT_LT(a, b) do { \
    auto _a = (a); \
    auto _b = (b); \
    if (!(_a < _b)) { \
        fprintf(stderr, "FAIL: %s:%d: %s not < %s\n", __FILE__, __LINE__, #a, #b); \
        return 1; \
    } \
} while(0)

#define TEST_ASSERT_LE(a, b) do { \
    auto _a = (a); \
    auto _b = (b); \
    if (!(_a <= _b)) { \
        fprintf(stderr, "FAIL: %s:%d: %s not <= %s\n", __FILE__, __LINE__, #a, #b); \
        return 1; \
    } \
} while(0)

/* For tests that need to fail explicitly */
#define TEST_FAIL(msg) do { \
    fprintf(stderr, "FAIL: %s:%d: %s\n", __FILE__, __LINE__, msg); \
    return 1; \
} while(0)

/*
 * Logging for verbose test output.
 * Set TEST_VERBOSE=1 environment variable to enable.
 * Use: TEST_LOG("Processing %d items", count);
 */
#define TEST_LOG(...) do { \
    if (getenv("TEST_VERBOSE")) { \
        printf("  [LOG] "); \
        printf(__VA_ARGS__); \
        printf("\n"); \
        fflush(stdout); \
    } \
} while(0)

/* Log without the [LOG] prefix, for custom formatting */
#define TEST_PRINT(...) do { \
    if (getenv("TEST_VERBOSE")) { \
        printf(__VA_ARGS__); \
        fflush(stdout); \
    } \
} while(0)

#endif /* PAPLEASE_TEST_H */


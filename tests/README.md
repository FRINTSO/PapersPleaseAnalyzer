# Tests

Framework-less test suite. Each test is a standalone executable that returns 0 on success, non-zero on failure. Built with CMake, run with CTest.

## Structure

```
tests/
├── include/test.h         # Minimal assertion macros
├── geometry/              # Rectangle tests
├── colorspace/            # Color conversion tests
├── ocr/                   # OCR algorithm tests
├── gamescreen/            # Game screen handling tests
├── vision/                # Integration tests with real images
├── inspector/             # Inspector integration tests
├── data/                  # Test data (screenshots, game_sim/)
├── CMakeLists.txt         # Test build definitions
├── setup_test_data.sh     # Script to copy test images
└── README.md              # This file
```

## Building

From the project root:

```bash
cd build && cmake .. && make
```

To skip building tests:

```bash
cmake .. -DPAPLEASE_BUILD_TESTS=OFF
```

## Setting Up Test Data

Before running integration tests, copy some game screenshots:

```bash
chmod +x tests/setup_test_data.sh
./tests/setup_test_data.sh
```

## Running Tests

```bash
cd build

ctest                        # run all tests
ctest --output-on-failure    # show stderr on failure
ctest -R charset             # run only charset tests
ctest -L integration         # run integration tests (require screenshots)
ctest -R test_rectangle      # run rectangle tests
```

## Writing New Tests

1. Create `tests/<category>/test_<name>.cpp`
2. Include `"test.h"` for assertions
3. Write a `main()` that returns 0 on success
4. Add one line to `tests/CMakeLists.txt`:
   ```cmake
   paplease_test(test_<name> <category>/test_<name>.cpp)
   ```

Example:

```cpp
#include "test.h"

int main()
{
    int x = 42;
    TEST_ASSERT_EQ(x, 42);
    TEST_ASSERT(x > 0);
    return 0;
}
```

## Available Assertions

- `TEST_ASSERT(cond)` - Fail if condition is false
- `TEST_ASSERT_EQ(a, b)` - Fail if a != b
- `TEST_ASSERT_NE(a, b)` - Fail if a == b
- `TEST_ASSERT_GT(a, b)` - Fail if a not > b
- `TEST_ASSERT_GE(a, b)` - Fail if a not >= b
- `TEST_ASSERT_LT(a, b)` - Fail if a not < b
- `TEST_ASSERT_LE(a, b)` - Fail if a not <= b
- `TEST_ASSERT_STR_EQ(a, b)` - Fail if strings differ
- `TEST_FAIL(msg)` - Unconditional failure

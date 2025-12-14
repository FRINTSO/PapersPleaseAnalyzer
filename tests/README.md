# Tests

Framework-less test suite. Each test is a standalone executable that returns 0 on success, non-zero on failure.

## Structure

```
tests/
├── include/test.h         # Minimal assertion macros
├── geometry/              # Rectangle tests
├── colorspace/            # Color conversion tests
├── ocr/                   # OCR algorithm tests
├── gamescreen/            # Game screen handling tests
├── vision/                # Integration tests with real images
├── data/                  # Test data (screenshots)
├── premake5.lua           # Build config
├── setup_test_data.sh     # Script to copy test images
└── README.md              # This file
```

## Building

From the project root:

```bash
# Generate makefiles
./vendor/premake/bin/premake5 gmake2

# Build tests (and everything else)
make config=debug
```

## Setting Up Test Data

Before running vision tests, copy some game screenshots:

```bash
chmod +x tests/setup_test_data.sh
./tests/setup_test_data.sh
```

This copies images from `before_resturcture/core/assets/images/` to `tests/data/screenshots/`.

## Running Tests

```bash
# Run all tests
./run_tests.sh

# Run with Release config
./run_tests.sh Release

# Run individual test
./bin/linux-x86_64/Debug/tests/test_rectangle_empty
```

## Writing New Tests

1. Create `tests/<category>/test_<name>.cpp`
2. Include `"test.h"` for assertions
3. Write a `main()` that returns 0 on success
4. Add to `tests/premake5.lua` using `test_project()`

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


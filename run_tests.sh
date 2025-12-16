#!/bin/bash
#
# run_tests.sh - Run all test executables
#
# Returns 0 if all tests pass, non-zero if any test fails.
# Each test is a standalone executable that returns 0 on success.
#
# Usage: ./run_tests.sh [options] [config]
#	-v, --verbose	 Show test output even for passing tests
#	config			 Debug or Release (default: Debug)
#
# NOTE: Run with bash, not sh: ./run_tests.sh or bash run_tests.sh
#

set -e

# Parse arguments
VERBOSE=0
CONFIG="Debug"

for arg in "$@"; do
	case "$arg" in
		-v|--verbose)
			VERBOSE=1
			export TEST_VERBOSE=1
			;;
		Debug|Release)
			CONFIG="$arg"
			;;
		-h|--help)
			echo "Usage: $0 [-v|--verbose] [Debug|Release]"
			echo "	-v, --verbose	 Show test output even for passing tests"
			exit 0
			;;
	esac
done

# Get project root and cd there so relative paths in tests work
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

# Set library path for OpenCV
export LD_LIBRARY_PATH="${SCRIPT_DIR}/vendor/opencv-install/lib:${LD_LIBRARY_PATH}"

# Detect OS and architecture for bin path
case "$(uname -s)" in
	Linux*)		PLATFORM="linux-x86_64";;
	Darwin*)	PLATFORM="macosx-x86_64";;
	CYGWIN*|MINGW*|MSYS*) PLATFORM="windows-x86_64";;
	*)			echo "Unknown platform: $(uname -s)"; exit 1;;
esac

# Test binary directory
TEST_DIR="bin/${PLATFORM}/${CONFIG}/tests"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NC='\033[0m' # No Color

# Track results
PASSED=0
FAILED=0
SKIPPED=0

echo "========================================"
echo "Running tests (${CONFIG})"
echo "Test directory: ${TEST_DIR}"
echo "========================================"
echo ""

# Check if test directory exists
if [ ! -d "$TEST_DIR" ]; then
	echo "${RED}Error: Test directory not found: ${TEST_DIR}${NC}"
	echo "Run 'make' or 'premake5 gmake2 && make' first to build tests."
	exit 1
fi

# Find and run all test executables
for test_exe in "$TEST_DIR"/test_*; do
	if [ ! -f "$test_exe" ] || [ ! -x "$test_exe" ]; then
		continue
	fi
	
	test_name=$(basename "$test_exe")
	printf "%-40s " "$test_name"
	
	# Run test and capture output
	set +e
	output=$("$test_exe" 2>&1)
	result=$?
	set -e

	if [ $result -eq 0 ]; then
		# Check if test was skipped
		if echo "$output" | grep -q "SKIP:"; then
			printf "${YELLOW}SKIP${NC}\n"
			SKIPPED=$((SKIPPED + 1))
			# Show skip reason
			echo "$output" | grep "SKIP:" | sed 's/^/	 /'
		else
			printf "${GREEN}PASS${NC}\n"
			PASSED=$((PASSED + 1))
			# Show output in verbose mode
			if [ $VERBOSE -eq 1 ] && [ -n "$output" ]; then
				echo "$output" | sed 's/^/	  /'
			fi
		fi
	else
		printf "${RED}FAIL${NC}\n"
		FAILED=$((FAILED + 1))
		# Show failure output
		echo "$output" | sed 's/^/	  /'
	fi
done

echo ""
echo "========================================"
echo "Results:"
printf "  ${GREEN}Passed:${NC}	%d\n" "$PASSED"
printf "  ${RED}Failed:${NC}  %d\n" "$FAILED"
printf "  ${YELLOW}Skipped:${NC} %d\n" "$SKIPPED"
echo "========================================"

# Exit with failure if any test failed
if [ $FAILED -gt 0 ]; then
	exit 1
fi

exit 0

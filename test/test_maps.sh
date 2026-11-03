#!/bin/bash

BINARY="./build/so_long"
if [ ! -f "$BINARY" ]; then
    echo "Error: Binary '$BINARY' not found. Run 'make' first."
    exit 1
fi

passed=0
failed=0

# Usage: run_test <map_file> <expected_exit_code> <test_description>
run_test() {
    local map_path="$1"
    local expected="$2"
    local description="$3"

    # Run your binary with --check flag in headless mode
    $BINARY --check "$map_path" > /dev/null 2>&1
    local actual=$?

    if [ $actual -eq $expected ]; then
        echo "  [PASS] $description (Got exit code $actual)"
        ((passed++))
    else
        echo "  [FAIL] $description"
        echo "         Expected exit code: $expected, Got: $actual"
        ((failed++))
    fi
}

echo "[*] Testing valid maps..."
run_test "assets/maps/valid/small_map1.ber" 0 "Standard valid map"
run_test "assets/maps/valid/small_map2.ber" 0 "Standard valid map"
run_test "assets/maps/valid/medium_map1.ber" 0 "Standard valid map"
run_test "assets/maps/valid/medium_map2.ber" 0 "Standard valid map"

echo "[*] Testing invalid maps..."
run_test "assets/maps/invalid/map_invalid_extension" 1 "Map must have a '.ber' extension"
run_test "assets/maps/invalid/map_not_rectangular.ber" 4 "Map is not rectangular"
run_test "assets/maps/invalid/map_empty_line1.ber" 4 "Map contains an empty line"
run_test "assets/maps/invalid/map_empty_line2.ber" 4 "Map contains an empty line"
run_test "assets/maps/invalid/map_empty_line3.ber" 4 "Map contains an empty line"
run_test "assets/maps/invalid/map_missing_wall.ber" 5 "Map is missing a delimiting wall"
run_test "assets/maps/invalid/map_extra_parameter.ber" 6 "Map contains invalid characters"
run_test "assets/maps/invalid/map_extra_P.ber" 7 "Map has an extra player"
run_test "assets/maps/invalid/map_missing_E.ber" 7 "Map is missing an exit"
run_test "assets/maps/invalid/map_missing_P.ber" 7 "Map is missing a player"
echo "Results: $passed passed, $failed failed."

if [ $failed -gt 0 ]; then
    exit 1
else
    exit 0
fi

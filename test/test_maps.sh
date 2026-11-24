#!/bin/bash

BINARY="./build/so_long"
if [ ! -f "$BINARY" ]; then
    echo "Error: Binary '$BINARY' not found. Build the project first."
    exit 1
fi

passed=0
failed=0

check_map() {
    local map_path="$1"
    local expected_code="$2"
    local map_name
    map_name=$(basename "$map_path")

    local output
    output=$(TEST_MAP_ONLY=1 "$BINARY" "$map_path" 2>&1)
    local actual_code=$?

    if [ "$actual_code" -eq "$expected_code" ]; then
        echo "  [PASS] $map_name"
        ((passed++))
    else
        echo "  [FAIL] $map_name (expected $expected_code, got $actual_code)"
        [ -n "$output" ] && echo "         $output"
        ((failed++))
    fi
}

echo "[*] Testing valid maps..."
for map in test/maps/valid/*.ber; do
    check_map "$map" 0
done

echo "[*] Testing invalid maps..."
for map in test/maps/invalid/*; do
    check_map "$map" 1
done

echo ""
echo "Results: $passed passed, $failed failed."
[ "$failed" -eq 0 ]

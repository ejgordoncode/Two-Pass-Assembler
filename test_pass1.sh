#!/bin/bash

# Test script for pass1.cpp
# Usage: ./test_pass1.sh

echo "=========================================="
echo "Testing pass1.cpp"
echo "=========================================="
echo ""

# Compile first
echo "Step 1: Compiling..."
make clean
make
if [ $? -ne 0 ]; then
    echo "ERROR: Compilation failed"
    exit 1
fi
echo "✓ Compilation successful"
echo ""

# Test 1: Basic functionality
echo "Test 1: Basic program (test_prog.sic)"
if [ -f test_prog.sic ]; then
    ./asx test_prog.sic
    if [ -f test_prog.st ]; then
        echo "✓ test_prog.st created"
        echo "Contents:"
        cat test_prog.st
        echo ""
    else
        echo "✗ test_prog.st NOT created"
    fi
else
    echo "⚠ test_prog.sic not found, skipping"
fi
echo ""

# Test 2: Check if executable exists
echo "Test 2: Executable check"
if [ -f asx ]; then
    echo "✓ asx executable exists"
else
    echo "✗ asx executable NOT found"
fi
echo ""

# Test 3: No arguments
echo "Test 3: No arguments (should error)"
./asx 2>&1 | grep -q "no input present"
if [ $? -eq 0 ]; then
    echo "✓ Correctly handles no arguments"
else
    echo "✗ Does not handle no arguments correctly"
fi
echo ""

echo "=========================================="
echo "Test completed"
echo "=========================================="


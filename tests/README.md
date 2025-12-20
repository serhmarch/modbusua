# modbusua Unit Tests

## Purpose

This directory contains unit tests for the `modbusua` project, written with GoogleTest/GoogleMock.
The tests cover the core, devices, ports, OPC UA layer, and helper modules.

## Prerequisites
- CMake 3.13+
- C++17 compiler
- Source dependencies available and built along with the project (ModbusLib, open62541)

> Note: The root CMakeLists.txt integrates CTest and auto‑registers tests via `gtest_discover_tests`.

## Option A — Run via CTest (recommended)

### Windows (MSVC + Presets)
1) Configure and build tests (Debug):
```powershell
cmake --preset Win64-MSVC-Tests
cmake --build --preset Win64-MSVC-Debug-Tests
```

2) Run all tests:
```powershell
ctest --preset Win64-Tests-Debug
```
3) Run a subset (filter by test/fixture name):
```powershell
ctest --preset Win64-Tests-Debug -R CnFileInfoTest
```
4) List registered tests:
```powershell
ctest --preset Win64-Tests-Debug -N
```

### Linux (GCC/Clang + Presets)
1) Configure and build tests (Debug):
```bash
cmake --preset Linux-Debug-Tests
cmake --build --preset Linux-Debug-Tests
```
2) Run all tests:
```bash
ctest --preset Linux-Tests-Debug
```
3) Run a subset:
```bash
ctest --preset Linux-Tests-Debug -R CnFileInfoTest
```
4) List tests:
```bash
ctest --preset Linux-Tests-Debug -N
```

## Option B — Run the test executable directly

The test executable is generated as `testmodbusua`.

### Common GoogleTest command‑line options
Frequently used flags to control test execution:

- `--gtest_list_tests`: list all tests/fixtures without running.
- `--gtest_filter=PATTERN`: run only selected tests. Supports `*`, multiple patterns with `:`, and exclusions with `-`.
  - Examples: `CnFileInfoTest.*`, `CnDirTest.*:CnFileTest.*`, `* -CnDirTest.flaky_case`.
- `--gtest_repeat=N`: repeat execution N times (useful for finding flaky tests). Combine with `--gtest_break_on_failure`.
- `--gtest_break_on_failure`: stop at the first failure.
- `--gtest_shuffle`: shuffle test order; optionally `--gtest_random_seed=SEED`.
- `--gtest_output=xml[:FILE]`: save results in JUnit XML for CI. If `FILE` is omitted, a default is created in the current directory.
- `--gtest_color=yes|no|auto`: control color output.
- `--gtest_also_run_disabled_tests`: include tests marked as disabled.
- `--gtest_brief`: shorter output (less detail for passing tests).

### Windows
- Default path (Debug):
```powershell
$env:USERPROFILE\tmp\cmake\modbusua\bin\Win64-Tests\Debug\testmodbusua.exe
```
- Example runs:
```powershell
# All tests
your\path\to\testmodbusua.exe

# GoogleTest filtering
your\path\to\testmodbusua.exe --gtest_filter="CnFileInfoTest.*"

your\path\to\testmodbusua.exe --gtest_filter="CnDirTest.absoluteFilePath_test"

# List tests (no run)
your\path\to\testmodbusua.exe --gtest_list_tests

# Repeat and stop on first failure
your\path\to\testmodbusua.exe --gtest_repeat=10 --gtest_break_on_failure

# Shuffle order with fixed seed
your\path\to\testmodbusua.exe --gtest_shuffle --gtest_random_seed=123

# Export JUnit XML (for CI)
your\path\to\testmodbusua.exe --gtest_output=xml:results\gtest-report.xml
```

### Linux
- Typical path (Debug):
```bash
$HOME/tmp/cmake/modbusua/Linux/Debug-Tests/testmodbusua
```
- Example runs:
```bash
# All tests
$HOME/tmp/cmake/modbusua/Linux/Debug-Tests/testmodbusua

# GoogleTest filtering
$HOME/tmp/cmake/modbusua/Linux/Debug-Tests/testmodbusua --gtest_filter="CnFileInfoTest.*"

# List tests (no run)
$HOME/tmp/cmake/modbusua/Linux/Debug-Tests/testmodbusua --gtest_list_tests

# Repeat and stop on first failure
$HOME/tmp/cmake/modbusua/Linux/Debug-Tests/testmodbusua --gtest_repeat=10 --gtest_break_on_failure

# Shuffle order with fixed seed
$HOME/tmp/cmake/modbusua/Linux/Debug-Tests/testmodbusua --gtest_shuffle --gtest_random_seed=123

# Export JUnit XML (for CI)
$HOME/tmp/cmake/modbusua/Linux/Debug-Tests/testmodbusua --gtest_output=xml:results/gtest-report.xml
```

### Quick Start Examples

#### Windows (PowerShell)
```powershell
# 1) A single test (exact Suite.Test name)
$exe = "$env:USERPROFILE\tmp\cmake\modbusua\bin\Win64-Tests\Debug\testmodbusua.exe"
& $exe --gtest_filter="CnDirTest.filePath_test"

# 2) Multiple groups (colon) and exclusions (minus)
& $exe --gtest_filter="CnDirTest.*:CnFileInfoTest.*-*flaky*"

# 3) Repeat 50 times and stop on first failure
& $exe --gtest_repeat=50 --gtest_break_on_failure

# 4) Shuffle with a fixed seed
& $exe --gtest_shuffle --gtest_random_seed=42

# 5) Export JUnit XML report
if (!(Test-Path results)) { New-Item -ItemType Directory results | Out-Null }
& $exe --gtest_output=xml:results\gtest-report.xml
```

#### Linux (bash)
```bash
# 1) A single test (exact Suite.Test name)
exe="$HOME/tmp/cmake/modbusua/Linux/Debug-Tests/testmodbusua"
"$exe" --gtest_filter="CnDirTest.filePath_test"

# 2) Multiple groups and exclusions
"$exe" --gtest_filter="CnDirTest.*:CnFileInfoTest.*-*flaky*"

# 3) Repeat 50 times and stop on first failure
"$exe" --gtest_repeat=50 --gtest_break_on_failure

# 4) Shuffle with a fixed seed
"$exe" --gtest_shuffle --gtest_random_seed=42

# 5) Export JUnit XML report
mkdir -p results
"$exe" --gtest_output=xml:results/gtest-report.xml
```

## Tips and Diagnostics
- To enable building tests manually, use `-DCN_TESTS_ENABLED=ON` (already enabled in presets).
- For more detailed GoogleTest logging, add
`--gtest_repeat=1 --gtest_break_on_failure` or `--gtest_also_run_disabled_tests`.
- List tests inside the executable:
```bash
./testmodbusua --gtest_list_tests
```
- Some filesystem tests operate relative to the current working directory. Fixtures create temporary directories and, when needed, temporarily change the working directory during test execution.

## Directory Structure
```
./tests
├── core/      # Core tests (strings, time, FS, etc.)
├── device/    # Device tests and their elements
├── port/      # Port tests
├── ua/        # OPC UA layer tests
├── include/   # Helper headers for tests
├── CMakeLists.txt
└── README.md  # This file
```

## Typical Scenarios
- Full rebuild of tests:
```powershell
# Windows
cmake --preset Win64-MSVC-Tests
cmake --build --preset Win64-MSVC-Debug-Tests
ctest --preset Win64-Tests-Debug --output-on-failure
```
```bash
# Linux
cmake --preset Linux-Debug-Tests
cmake --build --preset Linux-Debug-Tests
ctest --preset Linux-Tests-Debug --output-on-failure
```

---
For more details on building and running, see the root README.md.
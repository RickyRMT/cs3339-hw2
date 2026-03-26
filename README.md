# CS3339 HW2 – Floating-Point Overflow Checker

## Author

Ricky Mosqueda-Torres (A05220370)

---

## Description

This program checks if a floating-point loop may experience overflow due to precision loss in IEEE 754 single-precision arithmetic.

It takes two inputs:

* `loop_bound`
* `loop_counter`

It prints their bit representations and determines whether overflow can occur. If so, it outputs the minimum overflow threshold.

---

## Build

```bash
g++ -std=c++17 -o fp_overflow_checker HW2.cpp
```

---

## Run

```bash
./fp_overflow_checker <loop_bound> <loop_counter>
```

Example:

```bash
./fp_overflow_checker 1e+08 1.0
```

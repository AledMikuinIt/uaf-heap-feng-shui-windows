# Use-After-Free & Heap Feng Shui (Minimal Demo)

## Disclaimer
This project is for educational purposes only.
It does **not** provide a real-world exploit and is **not**
intended to be weaponized.

It demonstrates the core concepts behind:
- Use-After-Free (UAF)
- Basic heap shaping (often referred to as "mini heap feng shui")

---

## Description
This repository demonstrates a controlled Use-After-Free on the
Windows heap.

By carefully ordering allocations and frees of objects with the
same size, a freed heap chunk is deterministically reused,
allowing controlled data to overwrite a previously freed object.

The demo shows how a function pointer can be hijacked via a UAF
when heap reuse occurs.

---

## Compilation
Compile with GCC / MinGW:

```bash
gcc -O0 -g -fno-omit-frame-pointer main.c -o main.exe
```

## Expected Result
Heap behavior may vary depending on the allocator and system configuration.
However, on typical Windows environments, the following output is commonly observed:

Initial Addr: 00007FF6FBEE1450

After Free Addr: 4141414141414141

After Free Name: Hello

This shows that the freed heap chunk was reused and that the function pointer
was overwritten via a Use-After-Free condition.

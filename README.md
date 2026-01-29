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
same size, a freed heap chunk is likely reused,
allowing controlled data to overwrite a previously freed object.

The demo shows how a function pointer can be hijacked via a UAF
when heap reuse occurs.
The function pointer overwrite is intentional and used purely
to make the UAF effect observable.

## Scope & Limitations

This demo:
- demonstrates a controlled Use-After-Free scenario
- shows basic heap reuse behavior

This demo does NOT:
- represent a real-world vulnerability
- bypass modern heap mitigations
- exploit allocator internals

---

## Compilation
Compile with GCC / MinGW:

```bash
gcc main.c -o main.exe
```

## Expected Result
Heap behavior may vary depending on the allocator and system configuration.
However, on typical Windows environments, the following output is commonly observed:

Current: 000001BFBE0196D0 | Next: 000001BFBE0193F0 | Callback: 00007FF6D5081450

Hello

Current: 000001BFBE0193F0 | Next: 0000000000000000 | Callback: 00007FF6D508146E

UAF

This shows that the freed heap chunk is likely reused and that the function pointer
was overwritten via a Use-After-Free condition.

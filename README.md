# Use-After-Free: Function Pointer Hijack via Heap Reuse (Minimal Demo)

## Disclaimer
This project is for educational purposes only.
It does **not** provide a real-world exploit and is **not**
intended to be weaponized.

## Description

This repository demonstrates a controlled **Use-After-Free (UAF)** where a freed heap chunk gets reused, allowing a function pointer stored in the original object to be hijacked.

The technique combines two simple ideas:
1. **Heap spraying**: allocate multiple identical objects (`spray[10]`), all the same size, then free them all without nulling the pointers (a UAF condition is created on purpose here, for demonstration).
2. **Free-list reuse**: allocate a second batch of objects (`evil[10]`) of the *same size* right after. Because allocators commonly reuse recently-freed chunks of matching size (often in LIFO order), some of the new "evil" objects are likely to land exactly where the old "spray" objects used to be.

Since the dangling `spray` pointers are never nulled, walking the (freed) `spray` linked list and calling `->callback()` on each node can end up executing `uaf()` the callback written by the *new* "evil" allocation, instead of the original `legit()` function. That's the observable proof that heap reuse occurred and the UAF was effective.

> **Note on naming**: this is a *heap spray + free-list reuse* UAF demo, not "heap feng shui" in the stricter sense. Real heap feng shui involves precisely engineering chunk adjacency, sizes, and allocator metadata (e.g. to control coalescing or trigger a targeted overflow), this demo relies on statistical reuse across many same-size allocations, which is a simpler (and less reliable) technique.

## Scope & Limitations

This demo:
- demonstrates a controlled Use-After-Free scenario
- shows basic heap chunk reuse behavior across same-size allocations
- hijacks a function pointer as a simple, observable proof of exploitation

This demo does **not**:
- represent a real-world vulnerability
- bypass modern heap mitigations (e.g. safe unlinking, hardened free-lists, CFI/CFG)
- exploit allocator internals in a targeted way (no control over chunk adjacency, metadata, or coalescing)
- guarantee reuse, success depends on allocator behavior, which varies by platform/allocator/build configuration, and isn't deterministic

## Compilation

```bash
gcc main.c -o main.exe
```

## Expected Result

Heap reuse behavior depends on the allocator and system configuration, so results aren't guaranteed, but a successful run typically looks like this:

```
Current: 000001BFBE0196D0 | Next: 000001BFBE0193F0 | Callback: 00007FF6D5081450
Hello
Current: 000001BFBE0193F0 | Next: 0000000000000000 | Callback: 00007FF6D508146E
UAF
```

The second node's `Callback` address differs from `legit()` and printing `UAF` instead of `Hello` confirms the freed chunk was reused by an `evil` object, and its callback (`uaf()`) got called through the dangling `spray` pointer.

## What this demonstrates

- Understanding of heap allocator reuse behavior and how a UAF becomes exploitable once a freed chunk is reallocated with attacker-controlled content.
- Practical use of a function pointer as an observable "canary" to prove memory corruption occurred, without needing a debugger to confirm it.
- Awareness of the difference between *demonstrating* a bug class and *reliably exploiting* it, this project is explicit about only doing the former.

## Next steps

- Explore more deterministic heap grooming (controlling chunk adjacency/order more precisely) to move from probabilistic spraying toward real heap feng shui.
- Test behavior across different allocators (glibc malloc, Windows heap/LFH) to compare reuse patterns.

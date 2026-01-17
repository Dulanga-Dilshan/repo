# repo
A powerful folder/file pattern generator and renamer written in C, supporting custom ranges, zero-padding, and batch renaming.


## Overview

`repo` is a low-level, pattern-driven file and folder name generator and renamer
written in **C**. It is designed to be explicit, deterministic, and easy to reason
about at the code level.

The project focuses on:

- Manual parsing (no regex libraries)
- Explicit state machines
- Predictable iteration order
- Minimal abstraction overhead
- Full control over memory and data structures

This is **not** a convenience wrapper around shell tools. It is a standalone
implementation intended for learning, control, and extensibility.

---

## Core Concepts

1. Pattern Tokens  
2. Range Definitions  
3. Token-to-Range Mapping  
4. Iteration Engine  
5. Output / Rename Layer  

Each stage is isolated to keep logic readable and debuggable.

---

## Pattern Parsing

Patterns are parsed character-by-character into an internal token list.

Supported tokens:

| Token | Meaning |
|------|--------|
| `?`  | Numeric digit placeholder |
| `<`  | Lowercase character placeholder |
| `>`  | Uppercase character placeholder |

Literal characters are preserved as-is.

### Example

```
Pattern: img_<>_??
```

Parsed as:

```
LITERAL('i')
LITERAL('m')
LITERAL('g')
LITERAL('_')
TOKEN(LOWER)
TOKEN(UPPER)
LITERAL('_')
TOKEN(DIGIT)
TOKEN(DIGIT)
```

No regex or format strings are used. Parsing is deterministic and single-pass.

---

## Range Parsing

Ranges are parsed separately from patterns.

### Syntax

```
(char_type){start,end[,increment]}
```

Each range is converted into a structured representation containing:

- type
- start value
- end value
- step (default = 1)
- current value (iterator state)

Validation ensures:

- Each token has a matching range
- Range types match token types
- Bounds are valid (e.g. `a–z`, `0–9`)

---

## Token–Range Binding

After parsing, each pattern token is bound to a corresponding range object.

This avoids string-based lookups during iteration and enables:

- Fast generation
- Clear ownership of state
- Easy extension (new token types)

The binding step fails early if the configuration is invalid.

---

## Iteration Engine

The iteration engine behaves like a multi-digit counter:

- The rightmost token advances first
- On overflow, it resets and carries left
- Termination occurs when the leftmost token overflows

This is implemented **without recursion**.

### Pseudocode

```
advance rightmost range
if overflow:
    reset range
    carry to previous range
```

This guarantees:

- Predictable output order
- No skipped combinations
- O(n) work per generated name

---

## Zero Padding

When enabled (`-z` flag), numeric tokens are rendered with padding based on the
maximum width required by their range.

Padding width is calculated once during initialization and reused during output.

---

## Rename Mode

Rename mode (`-re`):

- Enumerates existing files
- Applies generated names sequentially
- Supports filtering by extension or type
- Avoids overwriting by ordering operations safely

No shell expansion or external tools are used.

---

## File Filtering

Filtering occurs **before** rename or generation.

Supported filters:

- Extension list (`-x`)
- Image files
- Video files
- Audio files
- Document files

Filtering is implemented using simple extension matching for portability.

---

## Data Structures

- Custom linked list for tokens
- Range structures with embedded iterator state
- Cached traversal pointers for fast sequential access
- No dynamic resizing arrays at runtime

Memory ownership is explicit and centralized.

---

## Error Handling

- Fail fast on invalid input
- Clear error messages
- No silent fallbacks
- No undefined behavior by design

Every parsing stage validates input before proceeding.

---

## Build & Debugging

### Build

```bash
gcc -Wall -Wextra -O2 *.c -o repo
```

### Debug

```bash
gcc -Wall -Wextra -g *.c -o repo
```

### Recommended Tools

- valgrind
- gdb
- strace (Linux)

---

## Extension Ideas

- Recursive directory generation
- Dry-run mode
- Custom token definitions
- Unicode support
- JSON-based pattern definitions

---

## Design Philosophy

- Explicit > implicit
- Control > convenience
- Predictability > magic
- Readability > cleverness

`repo` is intentionally low-level.  
The goal is not fewer lines of code, but **fewer surprises**.

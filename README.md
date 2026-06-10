# 2D Graphics Editor in C

A menu-driven terminal graphics editor built in C using plain `printf/scanf`.  
Draws shapes on a 40×80 character canvas using `_` (background) and `*` (drawing).

---

## Features

- Draw **Circle**, **Rectangle**, **Line**, and **Triangle**
- **Add**, **Delete**, and **Modify** objects by ID
- Display the canvas anytime
- Works on **any compiler** — OnlineGDB, MinGW, GCC, Turbo C

---

## How to Compile & Run

### Online (OnlineGDB)
1. Go to [onlinegdb.com](https://www.onlinegdb.com)
2. Set language to **C**
3. Paste the code → click **Run**

### Windows (MinGW / GCC)
```bash
gcc graphics_editor.c -o graphics_editor -lm
./graphics_editor
```

### Linux / Mac
```bash
gcc graphics_editor.c -o graphics_editor -lm
./graphics_editor
```

---

## Menu Controls

| Key | Action              |
|-----|---------------------|
| `1` | Add a new object    |
| `2` | Delete object by ID |
| `3` | Modify object by ID |
| `4` | Display canvas      |
| `5` | Clear canvas        |
| `6` | List all objects    |
| `0` | Quit                |

---

## Shape Input Guide

| Shape     | Inputs required                                    |
|-----------|----------------------------------------------------|
| Circle    | Centre col, centre row, radius                     |
| Rectangle | Top-left (col, row) → bottom-right (col, row)      |
| Line      | Start (col, row) → end (col, row)                  |
| Triangle  | Three vertices, each as (col, row)                 |

---

## Project Structure

```
.
├── graphics_editor.c   # All source code (single file)
└── README.md           # This file
```

---

## Algorithms Used

- **Bresenham's Line Algorithm** — lines, rectangle edges, triangle edges
- **Midpoint Circle Algorithm** — circle outlines

---

## Canvas Details

- Size: **40 rows × 80 columns**
- Background character: `_` (underscore)
- Drawing character: `*` (asterisk)
- Stored as a 2D char array: `canvas[40][80]`

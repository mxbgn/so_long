# so_long

A 2D top-down tile-based dungeon game written in C, inspired by the classic `so_long` project specification. Navigate the map, collect all items and reach the exit.

Built with a **256-case bitmask autotiling engine**, frame-animated sprites, and automated headless/CI testing. See [ARCHITECTURE.md](docs/ARCHITECTURE.md) for technical design details.

---

## Building

### Prerequisites
- C compiler (`gcc` or `clang`)
- `CMake` (3.16+) and `make` / `ninja`

### Compile
```bash
cmake -B build
cmake --build build
```

---

## Usage

Run the game by passing a `.ber` map file:
```bash
# Play a map
./build/so_long <path/to/map.ber>
```

The goal of the game is to collect all collectibles on a map before walking to the exit.

---

## Controls

| Key | Action |
| :--- | :--- |
| `W` / `Up` | Move North |
| `A` / `Left` | Move West |
| `S` / `Down` | Move South |
| `D` / `Right` | Move East |
| `ESC` | Quit |

---

## Map Format (`.ber`)

Maps must be rectangular, surrounded by walls (`1`), and contain:
- `1` — Wall
- `0` — Floor
- `P` — Player (1)
- `E` — Exit (1)
- `C` — Collectible

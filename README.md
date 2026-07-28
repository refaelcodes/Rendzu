# Rendzu (Five-in-a-Row) — 40×40 board + performance-focused AI

Rendzu is a classic **Five-in-a-Row** (Gomoku-style) strategy game: players alternate placing stones, and the first to build an unbroken line of **five** wins (horizontal / vertical / diagonal).
Unlike common boards (often **15×15**), this project uses a **40×40** board — which makes naive brute-force search unrealistic and forces the AI to be efficient.

## Why this project matters
This repo demonstrates real-world engineering skills:
- **Game AI**: minimax-family decision search + evaluation function
- **Search-space control** on a large board (40×40)
- **Heuristics & scoring**: fast pattern-based weighting of candidate moves
- **Practical tooling**: save/load, replay navigation, example positions

## Features
- Player vs Computer (**Red** = you, **Blue** = computer)
- Red always starts
- Difficulty levels (AI strength)
- Save / Load games
- Replay navigation (Prev / Next / Last)
- Example positions (Red win / Blue win)
- HTML docs:
  - Marketing page (GitHub Pages): `docs/index.html`
  - Full help: `docs/Rendzu-Help.html`

## AI approach (high level)
The AI follows a **minimax-family** approach:
- build a limited-depth game tree
- evaluate positions at leaf nodes
- propagate scores upward (maximizing vs minimizing logic)

The key to making this workable on **40×40** is controlling the branching factor.

## Performance notes (why it doesn’t “explode” on 40×40)
A 40×40 board has **1600** cells. If you try “all empty cells”, the branching factor becomes massive.
To stay fast, the engine uses **candidate filtering + weighted selection + early stop rules**:

### 1) Local candidate generation (radius 2–4)
Instead of considering every empty cell, the engine only scans empty cells **near existing stones**:
- radius depends on difficulty (typically **2–4 cells**)
- this focuses computation on tactically relevant zones

### 2) Weighted scoring for every candidate move
For each candidate cell, a **weight** is computed by rule-based heuristics (patterns/threats/blocks).
Then the engine keeps only the **top-weighted** moves.

### 3) Candidate count depends on difficulty
Difficulty controls how many candidates are kept:
- easy: fewer candidates (faster, more “human mistakes”)
- hard: more candidates (stronger, slower)

### 4) Controlled depth + “best-so-far” fallback
Depending on difficulty, there are criteria for when to stop deeper exploration and take the
current **best-weight** option rather than expanding further.

### 5) Time budget (timer cutoff)
A strict time budget is enforced:
- if search exceeds the allowed time, it stops
- the engine immediately plays the **best move found so far** (highest score/weight)

> Result: the AI behaves like a practical engine, not a theoretical brute-force solver:
it searches where it matters, limits options, and guarantees responsiveness.

## Download & Run
### Option A — Download the latest build (recommended)
Use GitHub Releases:
`https://github.com/refaelcodes/Rendzu/releases/latest`

### Option B — Build from source
- Requirements: Windows + Visual Studio (adjust if needed)
- Open solution/project
- Build Debug/Release
- Run the produced executable

## MIT License

## Author
Rafael M. — Software Developer

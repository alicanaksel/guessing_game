# 🎲 Guessing Game in C

A simple **Guess the Number** game written in C.  
The program generates a random number within a user-defined range, and you try to guess it.  
It also logs every game session (win/lose, attempts, time, etc.) into a `game_log.txt` file.

---

## ✨ Features
- User chooses the **minimum** and **maximum** numbers.
- Optional **attempt limit** (or unlimited tries).
- Helpful hints: `"Higher!"` or `"Lower!"`.
- Result logging with:
  - Timestamp
  - Range
  - Target number
  - Attempts used
  - Win/Lose status
  - Elapsed time (seconds)

---

## 📦 Requirements
- A C compiler (`gcc` or `clang`).
- Standard C11 libraries (stdio, stdlib, time, ctype, string, stdbool).

---

## 🔧 Compilation
```bash
# Using gcc
gcc -std=c11 -Wall -Wextra -O2 guess.c -o guess

# Or using clang
clang -std=c11 -Wall -Wextra -O2 guess.c -o guess
```

---

## ▶️ Usage
```bash
./guess
```

**Sample Run**
```
Enter minimum: 1
Enter maximum: 100
Max attempts (0 = unlimited): 5
Your guess: 50
Higher!
Your guess: 75
Lower!
Your guess: 63
You win! Attempts: 3, Time: 7.14 s
Play again? (y/n): n
```

---

## 📝 Log File Example (`game_log.txt`)
```
2025-08-30 14:12:05, 1-100, 63, 3, WIN, 7.14
2025-08-30 14:15:44, 1-50, 22, 5, LOSE, 12.02
```

---

## 📂 Project Structure
```
guessing_game/
├── guess.c         # Source code
├── game_log.txt    # Game results (auto-created)
├── algorithm.txt   # Algorithm diagram (ASCII flowchart of program logic)
└── README.md       # Documentation
```

---

## 🚀 Future Improvements
- Difficulty presets (Easy: 1–50, Medium: 1–100, Hard: 1–1000).
- High score tracking.
- Colored terminal output (WIN/LOSE messages).
- Configurable log file path.

---

Developed as a small practice project in C to reinforce:
- Pointers
- File I/O
- Input validation
- Modular programming


## 👤 Author
Alican Aksel
alicanaksel4@gmail.com

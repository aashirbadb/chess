https://www.talkchess.com/forum3/viewtopic.php?t=47318

### 1. avoid illegal en passant capture

```
perft 6 = 824064
8/5bk1/8/2Pp4/8/1K6/8/8 w - d6 0 1
8/8/1k6/8/2pP4/8/5BK1/8 b - d3 0 1
```

### 2. en passant capture checks opponent

```
perft 6 = 1440467
8/8/1k6/2b5/2pP4/8/5K2/8 b - d3 0 1
8/5k2/8/2Pp4/2B5/1K6/8/8 w - d6 0 1
```

### 3. short castling gives check

```
perft 6 = 661072
5k2/8/8/8/8/8/8/4K2R w K - 0 1
4k2r/8/8/8/8/8/8/5K2 b k - 0 1
```

### 4. long castling gives check

```
perft 6 = 803711
3k4/8/8/8/8/8/8/R3K3 w Q - 0 1
r3k3/8/8/8/8/8/8/3K4 b q - 0 1
```

### 5. castling (including losing cr due to capture rook)

```
perft 4 = 1274206
r3k2r/1b4bq/8/8/8/8/7B/R3K2R w KQkq - 0 1
r3k2r/7b/8/8/8/8/1B4BQ/R3K2R b KQkq - 0 1
```

### 6. castling prevented

```
perft 4 = 1720476
r3k2r/8/3Q4/8/8/5q2/8/R3K2R b KQkq - 0 1
r3k2r/8/5Q2/8/8/3q4/8/R3K2R w KQkq - 0 1
```

### 7. promote out of check

```
perft 6 = 3821001
2K2r2/4P3/8/8/8/8/8/3k4 w - - 0 1
3K4/8/8/8/8/8/4p3/2k2R2 b - - 0 1
```

### 8. discovered check

```
perft 5 = 1004658
8/8/1P2K3/8/2n5/1q6/8/5k2 b - - 0 1
5K2/8/1Q6/2N5/8/1p2k3/8/8 w - - 0 1
```

### 9. promote to give check

```
perft 6 = 217342
4k3/1P6/8/8/8/8/K7/8 w - - 0 1
8/k7/8/8/8/8/1p6/4K3 b - - 0 1
```

### 10. underpromote to check

```
perft 6 = 92683
8/P1k5/K7/8/8/8/8/8 w - - 0 1
8/8/8/8/8/k7/p1K5/8 b - - 0 1
```

### 11. self stalemate

```
perft 6 = 2217
K1k5/8/P7/8/8/8/8/8 w - - 0 1
8/8/8/8/8/p7/8/k1K5 b - - 0 1
```

### 12. stalemate/checkmate

```
perft 7 = 567584
8/k1P5/8/1K6/8/8/8/8 w - - 0 1
8/8/8/8/1k6/8/K1p5/8 b - - 0 1
```

### 13. double check

```
perft 4 = 23527
8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 1
8/5k2/8/5N2/5Q2/2K5/8/8 w - - 0 1
```
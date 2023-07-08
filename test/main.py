import chess.pgn

pgn = open("test/lichess_games.pgn", encoding="utf-8")
positions = []

for i in range(1000):
    game = chess.pgn.read_game(pgn)
    board = game.board()
    moves = game.mainline_moves()
    for move in moves:
        board.push(move)
        positions.append(board.fen())

def remove_copies(posns):
    res = []
    [res.append(x) for x in posns if x not in res]
    return res

with open("test/lichess_games.txt", "w") as f:
    posns = remove_copies(positions)
    for position in posns:
        f.write(position + "\n")
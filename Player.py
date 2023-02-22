# -*- coding: utf-8 -*-
"""
Some hard coded things:
    +1 is 'x'
    -1 is 'o'
    0 is blank space at the board
    
Created on Sun Jan 29 22:13:37 2023

@author: zahil
"""
import numpy as np
from Board import Board

# "Virtuall Class"
class Player():
    def __init__(self,symbol,typ):
        self.symbol = symbol
        self.typ = typ
        if self.typ == 2:
            print('Created an unbeatable MinMax player')
    
    def SymbolStr(self):
        if self.symbol == 1:
            return 'x'
        elif self.symbol == -1:
            return 'o'
        elif self.symbol == 0:
            return '_'
        else:
            return 'Error'


class HumanPlayer(Player):
    def __init__(self,symbol):
        self.symbol = symbol
        self.typ = 0
            
    def GetAction(self,board):
        action = int(input("choose square (0-9):"))
        return action


class RandomComputer(Player):
    def __init__(self,symbol):
        self.symbol = symbol
        self.typ = 1
            
    def GetAction(self,board):
        action = np.random.choice(board.valid_actions())
        return action

    
class MiniMaxPlayer(Player):
    def __init__(self,symbol):
        self.symbol = symbol
        self.typ = 2
        print('Created an un-beatable MinMax player for ' + self.SymbolStr() )
            
    def GetAction(self,board):
        action = self.FindBestCase(board,self.symbol)
        return action
    
    def FindBestCase(self,board_copy,val):
        valid_moves = board_copy.valid_actions()
        if len(valid_moves)==1:
            return valid_moves[0]
        max_score = -1000
        action = valid_moves[0]
        for act in valid_moves:
            new_board = Board(Board = board_copy.Board)
            new_board.step(act,val)
            score = minimax(new_board,[act,val],depth=0,isMax=False)
            #print(score)
            if score > max_score:
                max_score = score
                action = act
        return action
    
# TODO: this must be written in a simple way.. and combined with FindBestCase   
def minimax(board,last_move,depth,isMax):
    last_act,last_val = last_move[0],last_move[1]
    if board.IsWon(last_act,last_val):
        if isMax:
            return -10 + depth 
        else:
            return 10 - depth
    else:
        val = last_val * -1 # flip players
        
    valid_moves = board.valid_actions()
    
    if len(valid_moves) == 0:
        return 0 # a draw
    
    action = valid_moves[0]
    max_score = -1000
    min_score = 1000

    for act in valid_moves:
        new_board = Board(Board = board.Board)
        new_board.step(act,val)
        score = minimax(new_board,[act,val],depth=depth+1,isMax=not(isMax))
        
        if isMax:   
            if score > max_score:
                max_score = score
                action = act
        else:
            if score < min_score:
                min_score = score
                action = act
    if isMax: 
        return max_score-depth
    else:
        return min_score+depth
    
###############################################################################
# For debug only
if __name__ == "__main__":
    board = Board(size = 3, Board = np.array([1,1,0,-1,-1,0,0,0,0]))
    p1 = MiniMaxPlayer(1)
    p2 = MiniMaxPlayer(-1)
    board.PrintBoard()
    action = p1.GetAction(board)
    print('Best action for X:',action)
    action = p2.GetAction(board)
    print('Best action for O:',action)
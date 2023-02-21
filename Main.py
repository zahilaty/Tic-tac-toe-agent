# -*- coding: utf-8 -*-
"""
Created on Mon Feb 13 15:41:31 2023
A simple wrpper to all classes so human can play against computer
Some hard coded things:
    +1 is 'x'
    -1 is 'o'
    0 is blank space at the board
Please note that both MinMax player and board support any size, but:
    A. The priniting is not nice for size != 3
    B. The MinMax player will run for hours..
Note that RL is very specific: only for 3X3 and only for 'x' or 'o' (TODO: maybe
                                                                     we can inverse?)
@author: zahil
"""
from Board import Board
from Agent import Agent
from Player import HumanPlayer,RandomComputer,MiniMaxPlayer

if __name__ == "__main__":
    board = Board(size = 3)
    
    p1 = HumanPlayer(symbol=1)
    p2 = Agent(-1)
    p2.loadTrainer('SharedDict.pkl')
    
    if p1.typ == 0:
        board.PrintBoard()
    
    while True:
        # x (val = 1) plays first. 
        action = p1.GetAction(board)
        board.step(action,p1.symbol)
        board.PrintBoard()
        if board.terminated:
            break
        action = p2.GetAction(board)
        board.step(action,p2.symbol)
        board.PrintBoard()
        if board.terminated:
            break
    
#X: 4 8 2 6\5
#O: 8 2 6 7
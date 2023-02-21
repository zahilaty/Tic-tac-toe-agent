# -*- coding: utf-8 -*-
"""
Created on Mon Feb 13 12:33:42 2023
Train two agents
If you want one of them to be stupid or just to add more states to Q
then set p1_exp_prob to 0, of play with the p1_decay_factor
@author: zahil
"""

from Board import Board
from Agent import Agent
from tqdm import tqdm

p1 = Agent(1)
#p1.loadTrainer('Xplayer.pkl')
p2 = Agent(-1)
#p2.loadTrainer('Oplayer.pkl')
p1.loadTrainer('SharedDict.pkl')

NumOfIter = 100000
p1_decay_factor = 0.9999
p2_decay_factor = 0.9999
p1_exp_prob = 1
p2_exp_prob = 1

for k in tqdm(range(NumOfIter)):
    board = Board(size = 3)
    board.reset()#just in case
    p1_exp_prob = p1_exp_prob*p1_decay_factor
    p2_exp_prob = p2_exp_prob*p2_decay_factor
    while True:
        # x (val = 1) plays first. 
        action = p1.GetAction(board,p1_exp_prob)
        p1.UpdateQ(board, action)
        board.step(action,p1.symbol)
        #board.PrintBoard()
        if board.terminated:
            break
        action = p2.GetAction(board,p2_exp_prob)
        p2.UpdateQ(board, action)
        board.step(action,p2.symbol)
        #board.PrintBoard()
        if board.terminated:
            break
    #print('Results of game number ' + str(k) + ':')
    #board.PrintBoard()
    
# p1.saveTrainer('Xplayer.pkl')
# p2.saveTrainer('Oplayer.pkl')
p1.saveTrainer('SharedDict.pkl')
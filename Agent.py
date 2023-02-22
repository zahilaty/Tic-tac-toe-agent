# -*- coding: utf-8 -*-
"""
Some hard coded things:
    +1 is 'x'
    -1 is 'o'
    0 is blank space at the board
    
TODO:
    1) Test for boards which are not 3X3                                                                   we can inverse?)

@author: zahil
"""
import numpy as np
import random
from Board import Board
from Player import Player
import _pickle as cPickle

class Agent(Player): 
    
    def __init__(self,symbol):
        self.symbol = symbol # 1 or -1
        self.opponent_val = -1*symbol
        self.typ = 3
        # This is a static class variable - very usefull to represnet the shared dict
        self.__class__.Q = {} 
    
    def EncodeStateActionPair(self,state_key,action):
        key = state_key + str(action) + str(self.symbol)
        return key
    
    def GetQVal(self,key):
        if key in self.__class__.Q:
            return self.__class__.Q[key]
        else:
            #print('found new state:',key)
            self.__class__.Q[key] = 0
            return 0
        
    def GetAction(self,board,exploration_probability=0):
        valid_moves = board.valid_actions()
        state_key = board.EncodeState()

        if random.random() < exploration_probability:
            action = np.random.choice(valid_moves)
            key = self.EncodeStateActionPair(state_key,action)
            qVal = self.GetQVal(key) # needed only for the purpose of expanding the dict
            return action
        
        else:
            bestQVal = -1
            action = valid_moves[0]
            
            for act in valid_moves:
                key = self.EncodeStateActionPair(state_key,act)
                qVal = self.GetQVal(key)
                if qVal>bestQVal:
                    bestQVal = qVal
                    action = act
            # The final key (created with choosen action):
            # key = self.EncodeStateActionPair(state_key,action)
            return action

    def UpdateQ(self,board,action,alpha = 0.5,gamma = 0.9):
        # The current move key:
        state_key = board.EncodeState()
        key = self.EncodeStateActionPair(state_key,action)
        
        # make a step
        new_board = Board(size = board.size,Board = board.Board)
        new_board.step(action,self.symbol)
        
        # Check current reward:
        reward = self.GetReward(new_board,action, self.symbol)
        
        # The future best key (no exploration):
        next_best_Qval = self.GetNextBestQval(new_board)
        
        self.__class__.Q[key] = self.__class__.Q[key]*(1-alpha) + \
            alpha*(reward + gamma*next_best_Qval)
    
    def GetReward(self,board,lin_ind,val):
        if board.IsWon(lin_ind,val):
            return 1 #Note that we can only win after *our* step.
            # In other repositories we will find something like this:
            # if self.symbol == val:
            #     return 1
            # if self.symbol == -val:
            #     return -1
        elif self.checkWinPossible(board):
            return -1
        else:
            return 0
        
    def checkWinPossible(self,board):
        valid_moves = board.valid_actions()
        opponent_val = self.opponent_val
        if len(valid_moves)>0:
            for act in valid_moves:
                new_board = Board(size = board.size,Board = board.Board)
                new_board.step(act,opponent_val)
                if new_board.IsWon(act,opponent_val):
                    return True # we have missed a chance tp block the opponent wining
        else:
            return False # a draw
    
    def GetNextBestQval(self,new_board):
        new_board_valid_actions = new_board.valid_actions()
        opponent_val = self.opponent_val
        if len(new_board_valid_actions)>1: # one turn foy my opponent, one for me
            opponent_action = np.random.choice(new_board_valid_actions)
            new_board.step(opponent_action, opponent_val)
            new_act = self.GetAction(new_board,exploration_probability=0)
            new_key = self.EncodeStateActionPair(new_board.EncodeState(),new_act)
            next_best_Qval = self.GetQVal(new_key)
        else:
            next_best_Qval = 0 # the game has ended
        return next_best_Qval
    
    def saveTrainer(self, save_path):
        dict = self.__class__.Q
        with open(save_path, "wb") as f:
            cPickle.dump(dict, f)
    
    def loadTrainer(self, save_path):
        with open(save_path, "rb") as f:
            dict = cPickle.load(f)
        self.__class__.Q = dict
        print('Loaded dict with number of keys = ',len(self.__class__.Q.keys()))
        
###############################################################################
# For debug only
if __name__ == "__main__":
    agent = Agent(1)
    print(agent.SymbolStr())           
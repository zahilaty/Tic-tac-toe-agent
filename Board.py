# -*- coding: utf-8 -*-
"""
Some hard coded things:
    +1 is 'x'
    -1 is 'o'
    0 is blank space at the board

# TODO: render board and other gui stuff
    
@author: zahil
"""
import numpy as np
import copy

class Board():
    def __init__(self,size = 3,Board=None):
        self.size = size
        if Board is None:
            self.Board = np.zeros((size**2))
        else:
            self.Board = copy.deepcopy(Board)
        self.terminated = False
    
    def reset(self):
        self.Board = np.zeros((self.size**2))

    def n_actions(self):
        # Note that not all steps are valid all the times!
        # hence a rule based approch will be used
        return self.size**2
    
    def n_observations(self):
        #This is not exactly the number of states as defined it thier example
        return self.size**2
    
    def valid_actions(self):
        # 0 is square which no one has marked yet
        return np.where(self.Board == 0)[0]
    
    def random_init(self): #Only for debug
        self.Board = np.array([[1, -1, 1],[-1, 1, -1],[1, -1, 1]])
        self.Board = self.Board.reshape(-1,) 
    
    def PrintBoard(self):
        print('\n')
        BoardAs2D = self.BoardAs2D()
        for m in range(self.size):
            string = '  '
            for n in range(self.size):
                if BoardAs2D[m,n] == 0:
                    string += '_'
                if BoardAs2D[m,n] == 1:
                    string += 'x'
                if BoardAs2D[m,n] == -1:
                    string += 'o'
                if n<self.size-1:
                    string += '  :  '
                else:
                    string += ''
            print(string)
            if  m<self.size-1:
                print('-----------------')
        print('\n')
        # for row in board_str:
        #     print(' '.join(['{:>6}'.format(str(x)) for x in row]))
        #     print('\n')      

    def render(self):
        self.PrintBoard()
    
    def push(self,lin_ind,val):
        assert(val== 1 or val == -1)
        assert(self.Board[lin_ind] == 0)
        self.Board[lin_ind] = val
        
    def step(self,lin_ind,val):
        self.push(lin_ind,val)
        win_flag = self.IsWon(lin_ind,val)
        board_full = len(self.valid_actions())==0
        if win_flag or board_full:
            self.terminated = True
        else:
            self.terminated = False
        if (not win_flag) and board_full:
            draw = False
        return
    
    def BoardAs2D(self):
        return np.reshape(self.Board,(self.size,self.size))
    
    def IsWon(self,lin_ind,val): #after step has been made
        # https://stackoverflow.com/questions/1056316/algorithm-for-determining-tic-tac-toe-game-over
        row = int(lin_ind/self.size)
        col = lin_ind%self.size 
        di = np.diag_indices(self.size)
        di_rev = (di[0],np.flip(di[1]))
        win_flag = False
        if np.all(self.BoardAs2D()[row,:]==val): win_flag = True
        if np.all(self.BoardAs2D()[:,col]==val): win_flag = True
        if np.all(self.BoardAs2D()[di]==val): win_flag = True
        if np.all(self.BoardAs2D()[di_rev]==val): win_flag = True
        return win_flag
    
    def EncodeState(self):
        string = ''
        for element in self.Board:
            if element == 0:
                string += '_'
            if element == 1:
                string += 'x'
            if element == -1:
                string += 'o'
        return string

###############################################################################
# For debug only
if __name__ == "__main__":
    board = Board(size = 3, Board = np.array([1,1,1,-1,-1,-1,0,0,0]))
    board.PrintBoard()
    print(board.EncodeState())
    
# -*- coding: utf-8 -*-
"""
Created on Thu Feb 16 12:23:59 2023
After training, I wanted to anlyse the dictonary values
The problem: the agents does not understand that taking the middle 
square is the best starting move. However, they dont loose

Examples:
1: X and O always start at square 0  

2:
when human X try to win with this sequence: 4 8 2 6\5 (O choose 0)
o is blocking him only by chance, because p2.Q['o___x___x1-1'] ==0 
like all p2.Q['o___x___x?-1'] options!

@author: zahil
"""
from Agent import Agent
p1 = Agent(1)
p1.loadTrainer('SharedDict.pkl')
Q = p1.Q

# non zero elements
red = []
for key in p1.Q:
    if p1.Q[key] != 0:
        #print(p1.Q[key])
        red.append(p1.Q[key])
print(len(red))

# collect odd level only
for key in Q.keys():
    val = Q[key]
    number_of_blank_spaces = key.count('_')
    if  number_of_blank_spaces in [9]:
        if val != 0:
            print(key,val)
            
# collect Even level only (always 0 in old version, and empty in new one)
# for key in Q.keys():
#     val = Q[key]
#     number_of_blank_spaces = key.count('_')
#     if  number_of_blank_spaces in [8]:
#         # if val != 0:
#         print(number_of_blank_spaces,val)

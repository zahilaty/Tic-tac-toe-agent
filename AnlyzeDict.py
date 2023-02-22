# -*- coding: utf-8 -*-
"""
After training, it is a good practice to analyze the dictonary values
(make sure for exmple that the Qval for the middle square has high value)

@author: zahil
"""
from Agent import Agent
p1 = Agent(1)
p1.loadTrainer('SharedDict.pkl')
Q = p1.Q

# find how many non zero elements exist
red = []
for key in p1.Q:
    if p1.Q[key] != 0:
        #print(p1.Q[key])
        red.append(p1.Q[key])
print(len(red))

# Analyze x turns (blank space number = 9,7,5,3,1)
for key in Q.keys():
    val = Q[key]
    number_of_blank_spaces = key.count('_')
    if number_of_blank_spaces in [9]:
        if val != 0:
            print(key,val)
            
# Analyze o turns (blank space number = 8,6,4,2)
# for key in Q.keys():
#     val = Q[key]
#     number_of_blank_spaces = key.count('_')
#     if number_of_blank_spaces in [8]:
#         # if val != 0:
#         print(number_of_blank_spaces,val)

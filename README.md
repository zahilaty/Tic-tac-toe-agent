# Tic-Tac-Toe Reinforcement Learning Agent

Update (5/5/2023) : A new C++ implementation has been added that has improved the speed of both RL agent training 
and the minimax algorithm.

This project implements a tic-tac-toe agent, trained using Q-learning (RL). The agent learns to play tic-tac-toe by playing against another copy of itself.
**Unlike other implementations,** this repository is able to represent both players using a single Q-value dictionary.
This is possible because the state-action pairs for X and O are disjunct, meaning that they don't overlap.
**The ability to represent both players with a single Q-value dictionary is crucial both for efficient weight updating (the Bellman equation), and for improving the gameplay experience by allowing the agent to make better and more informed decisions.**  
In addition to the RL agent, the repository also contains an implementation of the minimax algorithm, which can be used to play against the agent.
Users can choose to play against either the RL agent or the minimax algorithm and see that neither will lose.

# Usage
No GUI is included, only consule printings.  
To train the RL agent, run TrainAgent.py for about ~ 100,000 iteration (games).  
To play against the RL agent or the minimax algorithm, run Main.py (see documentation inside the file).


# Useful links:
https://www.youtube.com/watch?v=trKjYdBASyQ  
https://towardsdatascience.com/how-to-play-tic-tac-toe-using-reinforcement-learning-9604130e56f6

# License
This project is licensed under the MIT License. See the LICENSE file for more information.

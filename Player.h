#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <utility> // for std::pair
#include <tuple> // for tie 

#include "Board.h"

/* Player is virtual Class 
which will be override by HumanPlayer, RandomComputer,Minimax Player and RL Agent.
symbol is int with a value of +1 for x, -1 for o.
typ is 0 for human , 1 for random player, 2 for minmax player and 3 for agent
*/
class Player {
    public:
    	int symbol; 
        int typ; //
        //Player() {}
        Player(int sym, int t) {
            symbol = sym;
            typ = t;
            if(typ == 2) cout << "Created an unbeatable MinMax player" << endl;
        }
        string SymbolStr() {
            if(symbol == 1) return "x";
            else if(symbol == -1) return "o";
            else if(symbol == 0) return "_";
            else return "Error";
        }
};


class HumanPlayer : public Player {
    public:
        HumanPlayer(int sym):Player(sym,0){
		}
        int GetAction(Board board) {
        	// TODO: I pass board altough it is not needed, because I want a unified call to GetAction. What to do?
        	vector<int> valid_actions = board.GetValidActions();
        	cout << "Choose a square index: ";
			for (auto act : valid_actions) {
			cout << act << "  ";
			}
			cout<<endl;
            int action;
            cin >> action;
            return action;
        }
};


class RandomComputer : public Player {
    public:
        RandomComputer(int sym):Player(sym,1) {
        }
        int GetAction(Board board) {
        	vector<int> valid_actions = board.GetValidActions();
            int index = rand() % valid_actions.size(); // a random number in the range[0,number of valid actions -1]
            return valid_actions[index];
        }
};


class MiniMaxPlayer : public Player {
    public:
        MiniMaxPlayer(int sym) : Player(sym,2){
        }
        
        int GetAction(Board board) {
        	vector<int> last_move = {-1,-1*symbol}; // The last move was the opponent move (therefore the *-1) and the last move was irrelevant
			int action,score;
			tie(action,score) = minimax(board, last_move, 0, true);
            return action;
        }
        
    private:
    	int MaxDepth = 10; //Set a limit to the tree depth
    	
        pair<int, int> minimax(Board board, vector<int> last_move, int depth, bool isMax) {
			
			/* 
			This is a recursive function that performs a DFS.
			 First, let's make sure that we have not reached one of the stopping criteria:
			1) Someone has won (return the win/lose reward).
			2) There is draw (return zero reward).
			3) We have reached a depth  limit (return some default reward).
			*/
			
			// If the depth is 0 than the "last act" is not relevant and therefore passed as -1 to this function (from GetAction)
			int last_act = last_move[0], last_val = last_move[1];
			
			// Check if someone has won. This is relevant only if we are at depth > 0 (otherwise we dont have a "last_act")
			if (depth>0) {
				
            	if(board.IsWon(last_act, last_val)) {
            		// action is -1: irrelevant at the end of the game
                	if(isMax) return make_pair(-1, -MaxDepth + depth);
                	else return make_pair(-1, MaxDepth - depth);
            	}
			}
				          
            // Get valid moves (actions in the RL terminology)
            vector<int> valid_moves = board.GetValidActions();
            
            // If no valid actions left, and we already checked that there was no win, that this is a draw! We return action -1 and score 0
			if(valid_moves.size() == 0) return make_pair(-1, 0);

			// If we reached the max depth - do not make another call (so the user will not wait too long)
			if (depth>MaxDepth){
				return make_pair( valid_moves[0], 0); //return some default action with zero reward 
			}   
			         
			         
			/* Second, let's continue with the DFS (make another step in the recursion)
			For this, we will define:
			1) action - the *current* best action. We need to return this so that GetAction will not have to use another auxillary function.
			2) next__best_action - the best action of the next step.
			3) score - the score we will get after doing a step with "action. 
			Please note that score == reward in RL terminology, and that move == action in RL terminology.
			4) A new board to test each of the possible actions we can make
            */ 
                        
            int val = last_val * -1; // Flip players
            int action = valid_moves[0]; 
            int max_score = -INT_MAX;
            int min_score = INT_MAX;
            
            for(int i=0; i<valid_moves.size(); i++) {
                Board new_board(board);
                new_board.step(valid_moves[i], val);
                int next__best_action,score; // we dont need the next best action, but only the current best, which is called here "action"
                tie(next__best_action,score) = minimax(new_board, {valid_moves[i], val}, depth+1, !isMax);
                if(isMax) {
                    if(score > max_score) {
                        max_score = score;
                        action = valid_moves[i];
					}
				}
				else {
            		if (score < min_score) {
                		min_score = score;
                		action = valid_moves[i];
            		}
            	}
        	}
        	
        	// End of loop - return best [action,score] pair
            if (isMax) {
        		return make_pair(action, max_score - depth);
    		}
			else {
        		return make_pair(action, min_score + depth);
    		}
		}
};
#endif

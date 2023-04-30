#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Board.h"

/* TODO
1) return from board a vector
2) does the define size not creating problems with vector size?
*/

class Player {
    public:
    	int symbol;
        int typ;
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
            int index = rand() % valid_actions.size();
            return valid_actions[index];
        }
};


class MiniMaxPlayer : public Player {
    public:
        MiniMaxPlayer(int sym) : Player(sym,2){
        }
        int FindBestCase(Board board, int val) {
            vector<int> valid_actions = board.GetValidActions();
            if(valid_actions.size() == 1) return valid_actions[0];
            int max_score = -1000;
            int action = valid_actions[0];
            for(int i=0; i<valid_actions.size(); i++) {
                Board new_board(board);
                //cout<<"*************\n";
                //new_board.PrintBoard();
                new_board.step(valid_actions[i], val);
                //new_board.PrintBoard();
                //cout<<"*************\n";
                int score = minimax(new_board, {valid_actions[i], val}, 0, false);
                cout<<score<<endl;
				if(score > max_score) {
                    max_score = score;
                    action = valid_actions[i];
                }
            }
            return action;
        }
        
        int GetAction(Board board) {
            int action = FindBestCase(board, symbol);
            return action;
        }
        
    private:
        int minimax(Board board, vector<int> last_move, int depth, bool isMax) {
        //int minimax(Board board, int last_move[2], int depth, bool isMax) {
            int last_act = last_move[0], last_val = last_move[1];
            if(board.IsWon(last_act, last_val)) {
                if(isMax) return -10 + depth;
                else return 10 - depth;
            }
            //else {
            int val = last_val * -1; // flip players
            
            vector<int> valid_moves = board.GetValidActions();
            if(valid_moves.size() == 0) return 0; // a draw
            int action = valid_moves[0];
            int max_score = -1000;
            int min_score = 1000;
            for(int i=0; i<valid_moves.size(); i++) {
                Board new_board(board);
                new_board.step(valid_moves[i], val);
                int score = minimax(new_board, {valid_moves[i], val}, depth+1, !isMax);
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
            if (isMax) {
        		return max_score - depth;
    		}
			else {
        		return min_score + depth;
    		}
		}
};
#endif

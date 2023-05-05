#ifndef AGENT_H
#define AGENT_H
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <utility> // for std::pair
#include <tuple> // for tie 

#include "Board.h"
#include "Player.h"
using namespace std;

class Agent : public Player {
private:
    int opponent_val;
    static unordered_map<string, float> Q;

public:
    Agent(int sym) : Player(sym,3) {
        opponent_val = -1 * sym;
    }

    string EncodeStateActionPair(string state_key, int action) {
        string key = state_key + to_string(action) + to_string(this->symbol);
        return key;
    }

    float GetQVal(string key) {
        if (Q.find(key) != Q.end()) {
            return Q.at(key);
        }
        else {
            //cout << "found new state: " << key << endl;
            Q[key] = 0;
            return 0;
        }
    }

    int GetAction(Board board, float exploration_probability = 0) {
        vector<int> valid_actions = board.GetValidActions();
        string state_key = board.EncodeState();
		            
        if ((float)rand() / RAND_MAX < exploration_probability) { //sampling number between 0 to 1
        
            int action = valid_actions[rand() % valid_actions.size()]; //sampling action 
            string key = EncodeStateActionPair(state_key, action);
            float qVal = GetQVal(key); // needed only for the purpose of expanding the dict (inside GetQVal)
            return action;
        }
        else {
            float bestQVal = -1;
            int action = valid_actions[0]; //we assume the vector is not empty

            for (int act : valid_actions) {
                string key = EncodeStateActionPair(state_key, act);
                float qVal = GetQVal(key);
                //cout<<key<<" "<<qVal<<endl; //for debug
                if (qVal > bestQVal) {
                    bestQVal = qVal;
                    action = act;
                }
            }
			                    
            // The final key (created with the chosen action):
            // string key = EncodeStateActionPair(state_key, action);
            return action;
        }
    }

    void UpdateQ(Board board, int action, float alpha = 0.5, float gamma = 0.9) {
        // The current move key:
        string state_key = board.EncodeState();
        string key = EncodeStateActionPair(state_key, action);

        // make a step
        Board new_board(board);
        new_board.step(action, this->symbol);

        // Check current reward:
        float reward = GetReward(new_board, action, this->symbol);

        // The future best key (no exploration):
        float next_best_Qval = GetNextBestQval(new_board);

        Q[key] = Q[key] * (1 - alpha) + alpha * (reward + gamma * next_best_Qval);
    }

    float GetReward(Board board, int lin_ind, int val) {
        if (board.IsWon(lin_ind, val)) {
            return 1; //Our action (which is lin_ind) had made us won
        }
        else if (checkWinPossible(board)) {
            return -1; // we have missed a change to block the opponent winning
        }
        else {
            return 0;
        }
    }
              
    bool checkWinPossible(Board board) {
	    vector<int> valid_actions = board.GetValidActions();
	    if (valid_actions.size() > 0) {
	        for (int act : valid_actions) {
	            Board new_board(board);
	            new_board.step(act, opponent_val);
	            if (new_board.IsWon(act, opponent_val)) {
	                return true; // we have missed a chance to block the opponent from winning
	            }
	        }
	        return false; // opponent can't win
	    } 
		else {
	        return false; // it's a draw
	    }
	}

	double GetNextBestQval(Board new_board) {
	    vector<int> new_board_valid_actions = new_board.GetValidActions();
	    if (new_board_valid_actions.size() > 1) { // one turn for my opponent, one for me
	        int opponent_action = new_board_valid_actions[rand() % new_board_valid_actions.size()];
	        new_board.step(opponent_action, opponent_val);
	        int new_act = GetAction(new_board, 0);
	        string new_key = EncodeStateActionPair(new_board.EncodeState(), new_act);
	        double next_best_Qval = GetQVal(new_key);
	        return next_best_Qval;
	    } else {
	        return 0; // the game has ended
	    }
	}

   void saveTrainer(const string& save_path) {
		ofstream outfile(save_path,ios::trunc);
		if (outfile.is_open()) {
        	for (auto const& pair : Q) {
            	outfile << pair.first << " " << pair.second << std::endl;
        	}	
        	outfile.close();
        	cout<<"Saved Q dict with :"<<Q.size()<< " keys"<<endl;
    	}	
	}
	
	void loadTrainer(const std::string& save_path) {
		ifstream file(save_path);
		string line;
		if (file.is_open()) {
			while (getline(file,line)) {
				string key = line.substr(0, line.find(' '));
				float score = stof(line.substr(line.find(' ')+1,line.length()));
				//cout<<key<<" "<<score<<endl;  //for debug
				Q[key] = score;
			}
			file.close();
			cout<<"Loaded Q dict with :"<<Q.size()<< " keys"<<endl;
		}
		
	}
};
unordered_map<string, float> Agent::Q;

#endif

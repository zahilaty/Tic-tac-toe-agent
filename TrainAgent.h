#ifndef TRAINAGENT_H
#define TRAINAGENT_H

#include <iostream>
#include "Agent.h"
#include "Board.h"

int TrainAgent() {
  Agent p1(1); // 1 = x
  Agent p2(-1); // -1 = o
  p1.loadTrainer("SharedDictFromCpp.txt");
  
  int NumOfIter = 200000;
  double p1_decay_factor = 0.9999;
  double p2_decay_factor = 0.9999;
  double p1_exp_prob = 1;
  double p2_exp_prob = 1;

  for (int k = 0; k < NumOfIter; k++) {
    Board board;
    board.reset_board();
    p1_exp_prob = p1_exp_prob * p1_decay_factor;
    p2_exp_prob = p2_exp_prob * p2_decay_factor;

    while (true) {
      int action = p1.GetAction(board, p1_exp_prob);
      p1.UpdateQ(board, action);
      board.step(action, p1.symbol);
      if (board.terminated) {
        break;
      }

      action = p2.GetAction(board, p2_exp_prob);
      p2.UpdateQ(board, action);
      board.step(action, p2.symbol);
      if (board.terminated) {
        break;
      }
    }
    
    if (k%10000 == 0) {
    	cout<<"Finished "<< k << " iterations out of " << NumOfIter << endl;
	}
  }

  p1.saveTrainer("SharedDictFromCpp.txt"); // this saves both x and o weights (the dictionary is shared with static class var)
  return 0;
}
#endif

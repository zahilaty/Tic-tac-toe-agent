#include <iostream>
#include <vector>

#include "Board.h"
#include "Player.h"
//#include "Agent.h"
#include "Tests.h"

using namespace std;


/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char** argv) {
	
	//int test_return_val = MyTest();
	//return test_return_val;
	
if (true) {
	Board board;
    //MiniMaxPlayer p1(1);
    HumanPlayer p1(1);
    //HumanPlayer p2(-1);
    MiniMaxPlayer p2(-1);
    //p2.loadTrainer("SharedDict.pkl");
    if (p1.typ == 0) {
        board.PrintBoard();
    }
    
    while (true) {
        int action = p1.GetAction(board);
        board.step(action, p1.symbol);
        board.PrintBoard();
        if (board.terminated) {
            break;
        }

        action = p2.GetAction(board);
        board.step(action, p2.symbol);
        board.PrintBoard();
        if (board.terminated) {
            break;
        }
    }
    if (p1.typ == 0 or p2.typ == 0) {
    	cout<<"Game has ended"<<endl;
	}
}	
}

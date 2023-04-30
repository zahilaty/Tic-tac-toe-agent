#include <iostream>
#include "Board.h"
#include "Player.h"
using namespace std;

void Prints(Board board) {
	board.PrintBoard();
	
	vector<int> valid_actions = board.GetValidActions();
	for (auto act : valid_actions) {
		cout << "Index " << act << " is a valid action \n";
	}
	
	cout<<"Board state encoding is: " << board.EncodeState()<<endl;
	
	cout<<endl;
}

int MyTest() {
	cout<<" ******** Creating a board, init board: ********** \n";
	Board board;
	board.random_init();
	Prints(board);

	cout<< "******** Reaset Board ******** \n";
	board.reset_board();
	Prints(board);
	
	cout<< "******** Some push and steps to check winning ******** \n";
	board.step(0,1);
	board.step(3,-1);
	board.step(1,1);
	board.step(4,-1);
	cout<<"Is Won == "<<board.IsWon(4,-1)<<endl;
	board.step(2,1);
	cout<<"Is Won == "<<board.IsWon(2,1)<<endl;
	Prints(board);
	
	cout<< "******** Copy constructor check ******** \n";
	Board new_board(board);
    cout<<"OLD Board win,terminated,draw: "<<board.IsWon(2,1)<<board.terminated<<board.draw<<endl;
	cout<<"New Board win,terminated,draw: "<<new_board.IsWon(2,1)<<new_board.terminated<<new_board.draw<<endl;
	Prints(new_board);
	return 0;
}

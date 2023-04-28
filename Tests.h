#include <iostream>
#include "Board.h"
using namespace std;

int MyTest() {
	Board board;
	board.random_init();
	board.PrintBoard();
	board.reset_board();
	for (int i = 0; i < sizeSq; i++) {
		cout << board.GetValidActions()[i];
	}
	board.PrintBoard();
	cout<<board.EncodeState()<<endl;
	board.step(0,1);
	board.step(3,-1);
	board.step(1,1);
	board.step(4,-1);
	cout<<board.EncodeState()<<endl;
	cout<<board.IsWon(1,1)<<endl;
	board.step(2,1);
	cout<<board.IsWon(2,1)<<endl;
	board.PrintBoard();
	cout<<board.EncodeState()<<endl;
	for (int i = 0; i < sizeSq; i++) {
		cout << board.GetValidActions()[i];
	}
	return 0;
}

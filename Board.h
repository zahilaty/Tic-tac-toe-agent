#ifndef BOARD_H
#define BOARD_H
#include <iostream>
#include <cstdlib> // for rand() and srand()
#include <string> //convert char array to string
#include <assert.h>   // for assert in one line..
#include <numeric> //for sum of array in one line..
#include <time.h> //for srand seed
using namespace std;
#define size 3 // I used define istead of this: https://stackoverflow.com/questions/751878/determine-array-size-in-constructor-initializer
#define sizeSq 9

class Board {
public:
    // Constructor
    Board() {
    	reset_board();
	}
	
	// Copy from another board - use the Using copy constructor!
	// https://www.techiedelight.com/how-to-clone-objects-in-cpp/
	/* 
	static void CloneBoard(board old_br, board new_br) {
		copy(new_br.GetBoardArray(), new_br.GetBoardArray() + size, old_br.GetBoardArray())		
	}
	*/
  	int m(int lin_ind) {
  		int m = (int) lin_ind/size;
  		return m;
	}
	int n(int lin_ind) {
		int n = lin_ind%size;
		return n;
	}
    // Reset the board to its initial state
    void reset_board() {    	
    	for (int ind = 0; ind<sizeSq; ind++) {    		
    		board[ind] = 0;
    		boardAs2D[m(ind)][n(ind)] = 0;
    		ValidActions[ind] = 1; 	// 0 is not valid
			terminated = false; // when true, game is ended
			draw = false;
		}
		//cout<<"Done Reset Board\n";
	}
	
    // Randomly initialize the board
    void random_init() {
    	// seed
    	time_t t;
    	srand((unsigned) time(&t));
    	
		for (int ind = 0; ind<sizeSq; ind++) {	
    		int random = rand() % 3 - 1; // the 3 here is not related to "size" but to player1,player2,blank
    		board[ind] = random;
			boardAs2D[m(ind)][n(ind)] = random;	
		}
		//cout<<board[0]<<" "<<board[1]<<" "<<board[2]<<" "<<endl;
	}
	
    // Print the board
    void PrintBoard() {
    cout << '\n';
	    for (int m = 0; m < size; m++) {
	        string str = "  ";
	        for (int n = 0; n < size; n++) {
	            if (boardAs2D[m][n] == 0) {
	                str += '_';
	            }
	            if (boardAs2D[m][n] == 1) {
	                str += 'x';
	            }
	            if (boardAs2D[m][n] == -1) {
	                str += 'o';
	            }
	            if (n < size-1) {
	                str += "  :  ";
	            } else {
	                str += "";
	            }
	        }
	        cout << str << '\n';
	        if (m < size-1) {
	            cout << "-----------------\n";
	        }
	    }
    cout << '\n';
	}

    // Get the valid actions for the current state of the board
    int* GetValidActions() {
		return ValidActions;
	}

    // Push an action onto the board's stack
    void push(int lin_ind, int val) {
    	assert ((val == 1) || (val == -1));
    	assert (board[lin_ind]==0);
    	board[lin_ind] = val;
    	boardAs2D[m(lin_ind)][n(lin_ind)] = val;
	    ValidActions[lin_ind] = 0; // this square is not valid now
	}
	
    // Step (push value + check if win)
    void step(int lin_ind, int val) {
    	push(lin_ind,val);
    	bool win_flag = IsWon(lin_ind,val);
    	
    	// Calculate the sum using std::accumulate, if it is equal to 0, board is full 
    	int sum = accumulate(begin(ValidActions), end(ValidActions), 0);
    	bool board_full = (sum == 0);
		if (win_flag || board_full) {
			terminated = true;
		}
		else {
			terminated = false;
		}
		
		// Do we have a draw (disentlganment with terminated)
		if (!win_flag and board_full) {
			draw = true;
		}
	}

	// IsWon -the key is to check only the relevant index!
	bool IsWon(int lin_ind,int val) {
		int m = (int) lin_ind/size;
		int n = lin_ind%size;
    	
		// Sum of elements in row m (go over col)
    	int row_sum = 0;
		for (int col=0;col<size;col++) {
			row_sum += boardAs2D[m][col];
		}
		if (row_sum==size) {
			return true;
		}
		
		// Sum of elements in col n (go over row)
    	int col_sum = 0;
		for (int row=0;row<size;row++) {
			col_sum += boardAs2D[row][n];
		}
		if (col_sum==size) {
			return true;
		}
		
		// The only case we need to check diagonal and reverse-diagonal
		if (m==n) {
			// Sum of elements in diag
			int diag_sum = 0;
			for (int k=0;k<size;k++) {
				diag_sum += boardAs2D[k][k];
			}
			if (diag_sum==size) {
				return true;
			}
			
			// sum of elements in reverse diagonal
			int rev_diag_sum = 0;
			for (int k=0;k<size;k++) {
				rev_diag_sum += boardAs2D[k][size-1-k];
			}
			if (rev_diag_sum==size) {
				return true;
			}
		}
		
		// All other cases
		return false;
	}
	
	// Encode State for Q learning dictionary
	string EncodeState() {
		char CodeAsChars[size*size];
		for (int ind = 0; ind<sizeSq;ind++) {
    		if (board[ind]==0) {
    			CodeAsChars[ind] = '_';
			}
			else if (board[ind]==1) {
				CodeAsChars[ind] = 'x';
			}
			else if (board[ind]==-1) {
				CodeAsChars[ind] = 'o';
			}
			else {
				cout<< "some problem with the values"<<endl;
			}
		}
		string StateCode(CodeAsChars,sizeSq); 
		return StateCode;
	}
		
private:
	int board[size*size];
	int boardAs2D[size][size]; 
	int ValidActions[sizeSq]; // 0 is not valid
	bool terminated = false; // when true, game is ended
	bool draw = false;
};

#endif

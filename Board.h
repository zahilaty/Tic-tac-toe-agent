#ifndef BOARD_H
#define BOARD_H
#include <iostream>
#include <vector>
#include <cstdlib> // for rand() and srand()
#include <string> //convert char array to string
#include <assert.h>   // for assert in one line..
#include <numeric> //for sum of array in one line..
#include <time.h> //for srand seed
using namespace std;
const int br_size = 3; // I used define instead of this: https://stackoverflow.com/questions/751878/determine-array-size-in-constructor-initializer
const int sizeSq = br_size*br_size;

class Board {
public:
		
    // Constructor
    Board() {
    	reset_board();
	}
	
	// Copy Constructor - instead of "static void CloneBoard(board old_br, board new_br)"
	Board(const Board &old_board) {
		
		//copy(board, board + sizeSq, old_board.board);
		int m;
		int n;
		for (int lin_ind=0;lin_ind<sizeSq;lin_ind++){
			m = M(lin_ind);
			n = N(lin_ind);
			board[lin_ind] = old_board.board[lin_ind];
			boardAs2D[m][n] = old_board.boardAs2D[m][n];
		}
		terminated = old_board.terminated;
		draw = old_board.draw;
	}
	
	// From linear index of 1D board to 2D  	
	int M(int lin_ind) {
  		int m = (int) lin_ind/br_size;
  		return m;
	}
	int N(int lin_ind) {
		int n = lin_ind%br_size;
		return n;
	}
	
    // Reset the board to its initial state
    void reset_board() {    	
    	for (int ind = 0; ind<sizeSq; ind++) {    		
    		board[ind] = 0;
    		boardAs2D[M(ind)][N(ind)] = 0;
			terminated = false; // when true, game is ended
			draw = false;
		}
	}
	
    // Randomly initialize the board
    void random_init() {
    	// seed
    	time_t t;
    	srand((unsigned) time(&t));
    	
		for (int ind = 0; ind<sizeSq; ind++) {	
    		int random = rand() % 3 - 1; // the 3 here is not related to "size" but to player1,player2,blank
    		board[ind] = random;
			boardAs2D[M(ind)][N(ind)] = random;	
		}
	}
	
    // Print the board
    void PrintBoard() {
    cout << '\n';
	    for (int m = 0; m < br_size; m++) {
	        string str = "  ";
	        for (int n = 0; n < br_size; n++) {
	            if (boardAs2D[m][n] == 0) {
	                str += '_';
	            }
	            if (boardAs2D[m][n] == 1) {
	                str += 'x';
	            }
	            if (boardAs2D[m][n] == -1) {
	                str += 'o';
	            }
	            if (n < br_size-1) {
	                str += "  :  ";
	            } else {
	                str += "";
	            }
	        }
	        cout << str << '\n';
	        if (m < br_size-1) {
	            cout << "-----------------\n";
	        }
	    }
    cout << '\n';
	}

    // Get the valid actions for the current state of the board
    vector<int> GetValidActions() {
    	vector<int> valid_actions; //list of indexes
    	for (int i = 0; i < sizeSq; i++) {
            if (board[i] == 0) {
                valid_actions.push_back(i); //pushing the index, not the value!
            }
        }
		return valid_actions;
	}


    // Push an action onto the board's stack
    void push(int lin_ind, int val) {
    	assert ((val == 1) || (val == -1));
    	assert (board[lin_ind]==0);
    	assert((lin_ind>-1 and lin_ind<sizeSq));
    	board[lin_ind] = val;
    	boardAs2D[M(lin_ind)][N(lin_ind)] = val;
	}
	
    // Step (push value + check if win)
    void step(int lin_ind, int val) {
    	push(lin_ind,val);
    	bool win_flag = IsWon(lin_ind,val);
    	
    	// Calculate the sum using std::accumulate, if it is equal to 0, board is full 
    	vector<int> valid_actions = GetValidActions();
    	bool board_full = (valid_actions.size() == 0);
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
		int m = M(lin_ind);
		int n = N(lin_ind);    	
		int SumTOWin = val*br_size; //either +3 or -3
		
		// Sum of elements in row m (go over col)
    	int row_sum = 0;
		for (int col=0;col<br_size;col++) {
			row_sum += boardAs2D[m][col];
		}
		if (row_sum==SumTOWin) {
			return true;
		}
		
		// Sum of elements in col n (go over row)
    	int col_sum = 0;
		for (int row=0;row<br_size;row++) {
			col_sum += boardAs2D[row][n];
		}
		if (col_sum==SumTOWin) {
			return true;
		}
		
		// The only case we need to check diagonal
		if (m==n) {
			// Sum of elements in diag
			int diag_sum = 0;
			for (int k=0;k<br_size;k++) {
				diag_sum += boardAs2D[k][k];
			}
			if (diag_sum==SumTOWin) {
				return true;
			}
		}
		
		// The only case we need to check the reverse-diagonal . Example: m=0,n=2,m+n = 2, size = 3
		if (m+n == (br_size-1)) {
			// sum of elements in reverse diagonal
			int rev_diag_sum = 0;
			for (int k=0;k<br_size;k++) {
				rev_diag_sum += boardAs2D[k][br_size-1-k];
			}
			if (rev_diag_sum==SumTOWin) {
				return true;
			}
		}
		
		// All other cases
		return false;
	}
	
	// Encode State for Q learning dictionary
	string EncodeState() {
		char CodeAsChars[br_size*br_size];
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
	
	bool terminated = false; // when true, game is ended
	bool draw = false;
	
private:
	int board[br_size*br_size];
	int boardAs2D[br_size][br_size]; 

};

#endif

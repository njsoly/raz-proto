/**************************************************************
 **************      keypress_control.cpp       ***************
 * this is source for a program that will take 				
 * single-character keystroke commands until the quit 
 * character 'q' (or 'Q' if CAPS) is entered.
 */

 #include <iostream>
 //#include "raz_follower_defs.h"
 
 using namespace std;
 
 int main(){
 
	cout << "hello, you" << endl;

	bool quit = false;
	cout << "enter commands; 'q' to quit.\n";
	char c;
	while(!quit){
		// not right yet.  still waits for ENTER.
		c = getchar(); 
		
		if(c == 'q' || c == 'Q'){
			quit = true;
		}
		cout << "\rreceived command: '" << c << "'" << flush;
	}
	cout << "\ngoodbye." << endl;
	return 0;
 }
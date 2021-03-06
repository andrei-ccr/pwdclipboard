#include <iostream>
#include <windows.h>

#include "keylisten.h"
#include "passwords.h"

using namespace std;

short consoleMode = 0;
short pwdFile = 0;

int main(int argc, char **argv) {
    //Parse arguments
    if(argc>=2) {
        if(!strcmp(argv[1], "-c")) {
            consoleMode = 1;
        }
		else if(!strcmp(argv[1], "-f")) {
			if(argc<3) {
				cout<<"Specify the path to a new password file. \n";
				cout<<"Usage: " <<argv[0]<<" -f <path>\n";
				return 1;
			} else {
				pwdFile = 1;
			}
		}
    }
	
	struct lFlags lf;
	HWND wnd = FindWindowA("ConsoleWindowClass", NULL);
	BOOL wndRes = 0;

	if(pwdFile)
		InitPwds(argv[2]);
	else
		InitPwds("p.txt");

    if(!consoleMode) {
        wndRes = ShowWindow(wnd, SW_HIDE); //Returns non-zero
    }

    //Listen for key input
    do {
		resetFlags(lf);
		
        cout<<"Listening for activation key combination ..."<<endl;
        cout<<endl;
        cout<<"The following key combinations are valid:"<<endl;
        cout<<"\tCTRL + SHIFT + K  --  Allows password selection from list 0-9"<<endl;
        cout<<"\tCTRL + SHIFT + T  --  Show/Hide console"<<endl;
        cout<<"\tCTRL + SHIFT + H  --  Exits the program"<<endl;

        kListener(lf);

        if(lf.exitNow) break; //Check if the exitNow flag has been set
        if(lf.listenAgain) continue; //If the listenAgain flag is set, the loop should be restarted.
		if(lf.toggleVisibility) {
			if(wndRes != 0)
				wndRes = ShowWindow(wnd, SW_SHOW);
			else
				wndRes = ShowWindow(wnd, SW_HIDE);
			
			continue;
		}

        cout<<"Choose a password:"<<endl;
        ListPwds();
        selListener();
    } while(!lf.exitNow);

    return 0;
}

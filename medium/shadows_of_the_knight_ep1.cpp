#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;


void middle(int area[], int* x, int* y){
    *x = (area[0] + area[2])/2;
    *y = (area[1] + area[3])/2;
}


void assignArea(int area[], int ul_X, int ul_Y, int br_X, int br_Y){
    area[0] = ul_X;
    area[1] = ul_Y;
    area[2] = br_X;
    area[3] = br_Y;
}


int main()
{
    int W; // width of the building.
    int H; // height of the building.
    cin >> W >> H; cin.ignore();
    int N; // maximum number of turns before game over.
    cin >> N; cin.ignore();
    int X0;
    int Y0;
    cin >> X0 >> Y0; cin.ignore();
    
    int searchArea [4] = {0,0,W-1,H-1}; // [Upperleft X,Y, Bottomright X,Y]

    // game loop
    while (1) {
        string bombDir; // the direction of the bombs from batman's current location (U, UR, R, DR, D, DL, L or UL)
        cin >> bombDir; cin.ignore();
        
        if (bombDir == "U"){
            assignArea(searchArea, X0, searchArea[1], X0, Y0 - 1);
        }
        else if (bombDir == "UR"){
            assignArea(searchArea, X0 + 1, searchArea[1], searchArea[2], Y0 - 1);
        }
        else if (bombDir == "R"){
            assignArea(searchArea, X0 + 1, Y0, searchArea[2], Y0);
        }
        else if (bombDir == "DR"){
            assignArea(searchArea, X0 + 1, Y0 + 1, searchArea[2], searchArea[3]);
        }
        else if (bombDir == "D"){
            assignArea(searchArea, X0, Y0 + 1, X0, searchArea[3]);
        }
        else if (bombDir == "DL"){
            assignArea(searchArea, searchArea[0], Y0 + 1, X0 - 1, searchArea[3]);
        }
        else if (bombDir == "L"){
            assignArea(searchArea, searchArea[0], Y0, X0 - 1, Y0);
        }
        else if (bombDir == "UL"){
            assignArea(searchArea, searchArea[0], searchArea[1], X0 - 1, Y0 - 1);
        }
        
        cerr << searchArea[0] << ", " << searchArea[1] << ", " << searchArea[2] << ", " << searchArea[3] << endl;
        
        int Xnew;
        int Ynew;
        
        middle(searchArea, &Xnew, &Ynew);
        
        X0 = Xnew;
        Y0 = Ynew;

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;
        
        // the location of the next window Batman should jump to.
        cout << to_string(Xnew) + " " + to_string(Ynew) << endl;
    }
}
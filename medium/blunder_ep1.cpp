#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstddef> 
#include <array>

using namespace std;


enum Direction {SOUTH, EAST, NORTH, WEST};
static const string DirStrings[] = {"SOUTH", "EAST", "NORTH", "WEST"};


struct State{
    int x = 0, y = 0; // Position
    int dir = SOUTH; // Direction
    int nextDir; // Used by direction modifiers
    bool breaker = false; // true when Blunder is in breaker mode
    bool inverse = false; // true when Blunders circuits are inversed
    bool alive = true;
};


bool isStateEqual(State s1, State s2, vector<string> map1, vector<string> map2) {
    if (s1.x == s2.x && s1.y == s2.y && s1.breaker == s2.breaker 
      && s1.inverse == s2.inverse && s1.dir == s2.dir){
        for (int i = 0; i < map1.size(); i++){
            if (map1[i] != map2[i])
                return false;
        }
        return true;
    } else {
        return false;
    }
}


bool goInDir(State &blunder, vector<string> &map, int tp[]){
    int dx, dy;
    switch(blunder.dir) {
        case SOUTH:
            dx = 0;
            dy = 1;
            break;
        case EAST:
            dx = 1;
            dy = 0;
            break;
        case NORTH:
            dx = 0;
            dy = -1;
            break;
        case WEST:
            dx = -1;
            dy = 0;
            break;
    }
    
    blunder.nextDir = blunder.dir;
    
    char c = map[blunder.y+dy].at(blunder.x+dx);
    if (c == '#') {
        return false;
    }
    else if (c == 'X') {
        if (blunder.breaker) {
            map[blunder.y+dy].replace(blunder.x+dx, 1, " ");
            cerr << "Broke something" << endl;
        } else {
            return false;
        }
    }
    else if (c == '$') {
        blunder.alive = false;
    }
    else if (c == 'I') {
        blunder.inverse = !blunder.inverse;
        cerr << "Inverse mode = " << blunder.inverse << endl;
    }
    else if (c == 'B') {
        blunder.breaker = !blunder.breaker;
        cerr << "Breaker mode = " << blunder.breaker << endl;
    }
    else if (c == 'T') {
        if (blunder.x+dx == tp[0] && blunder.y+dy == tp[1]) {
            blunder.x = tp[2];
            blunder.y = tp[3];
        }
        else {
            blunder.x = tp[0];
            blunder.y = tp[1];
        }
        cerr << "Teleporting" << endl;
        return true;
    }
    else if (c == 'S' || c == 'E' || c == 'N' || c == 'W') {
        for (int i = 0; i < 4; i++) {
            if (DirStrings[i][0] == c) {
                blunder.nextDir = i;
                break;
            }
        }
    }

    blunder.x += dx;
    blunder.y += dy;
    return true;
}


int main()
{
    State blunder;
    int L;
    int C;
    cin >> L >> C; cin.ignore();
    vector<string> map;
    int tp[] = {0,0,0,0}; // {x1,y1,x2,y2} position of the teleporters
    vector<State> prevStates;
    vector<vector<string>> prevMaps;

    // Startup
    for (int i = 0; i < L; i++) {
        string row;
        getline(cin, row);
        map.push_back(row);
        cerr << row << endl;
        
        size_t startx;
        if (blunder.x == 0) {
            startx = row.find('@');
            if (startx != string::npos) {
                blunder.x = startx;
                blunder.y = i;
            }
        }
        
        startx = row.find('T');
        if (startx != string::npos) {
            if (tp[0] == 0) {
                tp[0] = startx;
                tp[1] = i;
                startx = row.find('T', startx+1);
                if (startx != string::npos) {
                    tp[2] = startx;
                    tp[3] = i;
                }
            }
            else if (tp[2] == 0) {
                tp[2] = startx;
                tp[3] = i;
            }
        }
    }
    
    // Game loop
    while (blunder.alive) {
        
        if (!goInDir(blunder, map, tp)) {
            if (blunder.inverse) {
                for (int i = 3; i > -1; i--) {
                    blunder.dir = i;
                    if (goInDir(blunder, map, tp)) {
                        break;
                    }
                }
            }
            else {
                for (int i = 0; i < 4; i++) {
                    blunder.dir = i;
                    if (goInDir(blunder, map, tp)) {
                        break;
                    }
                }
            }
        }
        
        for (int i = 0; i < prevStates.size(); i++){
            if (isStateEqual(prevStates[i], blunder, prevMaps[i], map) ) {
                cout << "LOOP" << endl;
                return 0;
            }
        }
        
        prevStates.push_back(blunder);
        prevMaps.push_back(map);
        cerr << "Blunder is at: {" << blunder.x << ", " << blunder.y << "} after going " << DirStrings[blunder.dir] << endl;
        blunder.dir = blunder.nextDir;
    }

    for (int i = 0; i < prevStates.size(); i++) {
        cout << DirStrings[prevStates[i].dir] << endl;
    }
}
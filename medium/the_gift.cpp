#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;


int main()
{
    int N; // the number of participants
    cin >> N; cin.ignore();
    int C; // the price of the gift
    cin >> C; cin.ignore();
    int totalBudget = 0;
    int budgets[N];
    for (int i = 0; i < N; i++) {
        int B;
        cin >> B; cin.ignore();
        budgets[i] = B;
        totalBudget += B;
    }
    
    
    if (totalBudget < C) {
        cout << "IMPOSSIBLE" << endl;
    }
    
    else {
        sort(budgets, budgets + N);
        bool flag = false; // 
        for (int i = 0; i < N; i++) {
            if(budgets[i] < C/(N-i)) {
                cout << budgets[i] << endl;
                C -= budgets[i];
            }
            else {
                int pay = C/(N-i);
                cout << pay << endl;
                C -= pay;
            }
        }
    }

    // Write an action using cout. DON'T FORGET THE "<< endl"
    // To debug: cerr << "Debug messages..." << endl;
}
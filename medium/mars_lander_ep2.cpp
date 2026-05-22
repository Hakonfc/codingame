#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

// Constants used by P regulators
const int Kp_hs1 = -1;
const int Kp_hs2 = 1;
const double Kp_vs = 1;
const double Kp_ang = 0.2;

// Gravity on Mars
const double g = 3.711;


void clip(int &variable, int min, int max){
    if (variable < min){
        variable = min;
    }
    else if (variable > max){
        variable = max;
    }
}


double getVSpeed(int VS, int power, int angle){
    return VS + (-g + power * cos(angle));
}

double getHSpeed(int HS, int power, int angle){
    return HS - power * sin(angle);
}


int main()
{
    int surfaceN; // the number of points used to draw the surface of Mars.
    cin >> surfaceN; cin.ignore();
    int surfaceX[surfaceN];
    int surfaceY[surfaceN];
    int landingX, landingY; 
    int middleX = 0;
    int middleY = 0;
    int prev = 0;
    bool found = false;
    for (int i = 0; i < surfaceN; i++) {
        int landX; // X coordinate of a surface point. (0 to 6999)
        int landY; // Y coordinate of a surface point. By linking all the points together in a sequential fashion, you form the surface of Mars.
        cin >> landX >> landY; cin.ignore();
        surfaceX[i] = landX;
        surfaceY[i] = landY;
        if (surfaceY[i] == prev){
            landingX = (surfaceX[i] + surfaceX[i-1])/2;
            landingY = landY;
            found = true;
        }
        else if (!found){
            prev = landY;
        }
    }
    
    cerr << "landingX/Y: " << landingX << " / " << landingY << endl;
    
    // game loop
    int t = 0;
    while (1) {
        t++;
        
        int X;
        int Y;
        int hSpeed; // the horizontal speed (in m/s), can be negative.
        int vSpeed; // the vertical speed (in m/s), can be negative.
        int fuel; // the quantity of remaining fuel in liters.
        int rotate; // the rotation angle in degrees (-90 to 90).
        int power; // the thrust power (0 to 4).
        cin >> X >> Y >> hSpeed >> vSpeed >> fuel >> rotate >> power; cin.ignore();

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;
        
        // Calculate a middle point to aim towards at the beginning of the flight
        // This is sufficient to route around the terrain challenges in this task
        if (t == 1){
            int biggest_dist = -200;
            double a = static_cast<double>(landingY - Y)/(landingX - X);
            cerr << "a: " << a << endl;
            for (int i = 0; i < surfaceN; i++){
                if ((landingX < surfaceX[i] && surfaceX[i] < X) || (landingX > surfaceX[i] && surfaceX[i] > X)){
                    int y_func = floor(a*(surfaceX[i] - X) + Y);
                    int dist = surfaceY[i] - y_func;
                    if (y_func < (surfaceY[i] + 100) && dist > biggest_dist){
                        middleX = surfaceX[i];
                        middleY = surfaceY[i] + 100;
                        biggest_dist = dist;
                    }
                    cerr << "X / Y: " << surfaceX[i] << " / " << surfaceY[i] << endl;
                    cerr << "y_func: " << y_func << endl;
                    cerr << "dist / biggest: " << dist << " / " << biggest_dist << endl;
                    
                }
            }
            cerr << "middleX/Y: " << middleX << " / " << middleY << endl;
        }
        
        // Reset middle point to start aiming towards the landing area
        if ((X-50 < middleX && middleX < X+50) && (Y-50 < middleY && middleY < Y+50)){
            middleX = 0;
        }
        
        //cerr << "middleX/Y: " << middleX << " / " << middleY << endl;
        
        // Calculated wanted direction

        int newPower, newAngle, errorX, errorY;
        
        if (middleX == 0){
            errorX = landingX - X;
            errorY = landingY - Y;
        }
        else {
            errorX = middleX - X;
            errorY = middleY - Y;
        }
        
        cerr << "errorX , errorY: " << errorX << " , " << errorY << endl;
        
        double dx, dy; // Wanted change in position
        
        if (2 * abs(errorX) > abs(errorY)){
            dx = 19.0 * errorX/abs(errorX);
            dy = dx * (static_cast<double>(errorY)/errorX);
        } else {
            dy = 37.0 * errorY/abs(errorY);
            dx = dy * (static_cast<double>(errorX)/errorY);
        }
        cerr << "dx , dy: " << dx << " , " << dy << endl;
        
        
        // Estimate time to landing
        double t_landing;
        double nextVS = getVSpeed(vSpeed, power, rotate);
        //double nextHS = getHSpeed(hSpeed, power, rotate);
        if (landingX - 200 < X < landingX + 200){
            t_landing = static_cast<double>(errorY)/nextVS;
        } else {
            double abs_dist = sqrt(pow(errorX,2) + pow(errorY,2));
            double abs_speed = sqrt(pow(hSpeed,2) + pow(vSpeed,2));
            double t_landing = abs_dist/abs_speed;
        }
        cerr << "t_landing: " << t_landing << endl;
        cerr << "nextVS: " << nextVS << endl;
        
        // Calculate angle and power for next step
        
        int error_hSpeed = static_cast<int>(dx - hSpeed);
        int error_vSpeed = static_cast<int>(dy - vSpeed);
        cerr << "error_hSpeed / error_vSpeed: " << error_hSpeed << " / " << error_vSpeed << endl;
        int angleMax = ceil(t_landing-1)*10 - 10;
        clip(angleMax, 0, 40);

        int k = error_hSpeed * error_vSpeed * Kp_ang;
        cerr <<  error_hSpeed * Kp_hs1 << ", " << k << endl;
        clip(k, 0, error_hSpeed * Kp_hs1);

        newAngle = error_hSpeed * Kp_hs1 + k; // P-regulator for angle
        clip(newAngle, -angleMax, angleMax);
        newPower = abs(error_hSpeed) * Kp_hs2 + ceil((error_vSpeed+3) * Kp_vs); // P-regulator for power
        clip(newPower, 0, 4);
        
        if (fuel < 5){
            newAngle = 0;
        }

        // rotate power. rotate is the desired rotation angle. power is the desired thrust power.
        cout << to_string(newAngle) + " " + to_string(newPower) << endl;
    }
}

#include <iostream>
#include <vector>
#include <cmath>
#include <unistd.h>
#define X 100
#define Y 25
#define CENTDIST 0.4
#define MAXCENT 10
using namespace std;

int nc = 0;

struct Point;

struct Cent{
    int x;
    int y;
    vector<Point*> nearPoints;
    Cent(char A[][Y],int x_,int y_){
        x = x_;
        y = y_; 
        A[x_][y_] = '@';
    }
    void setOnMat(char A[][Y],int x_,int y_){
        A[x][y] = ' ';
        A[x_][y_] = '@';
        x = x_;
        y = y_;
    }
};

struct Point{
    int x;
    int y;
    Cent* nearCent;
    Point(){
        x = 0;
        y = 0;
        nearCent = NULL;
    }
    Point(int x_,int y_){
        x = x_;
        y = y_; 
    }
    Point(char A[][Y],int x_,int y_){
        x = x_;
        y = y_; 
        nearCent = NULL;
        A[x_][y_] = '*';
    }
    void setNear(Cent* c){
        nearCent = c;
    }
};

char M[X][Y];
vector<Cent> cent;
vector<Point> points;

void resetMat(char A[][Y]){
    for(int i = 0; i < Y; i++){
        for(int j = 0; j < X; j++){
            A[j][i] = ' ';
        }
    }
}

void printMat(char A[][Y]){
    for(int j = 0; j < X; j++){
            cout<<"-";
    }
    cout<<"\n";
    for(int i = 0; i < Y; i++){
        cout<<"|";
        for(int j = 0; j < X; j++){
            cout<<A[j][i];
        }
        cout<<"|\n";
    }
    for(int j = 0; j < X; j++){
            cout<<"-";
    }
}

void insertPoints(char A[][Y],int nPoints,int agg){
    for(int j = 0 ; j < agg ; j++){
        int agg_x = rand()%X-21;
        int agg_y = rand()%Y-11;
        for(int i = 0; i < nPoints; i++){
            //generate random X and Y
            int x = (rand() % 20) + agg_x;
            int y = (rand() % 10) + agg_y;
            points.push_back(Point(A,x,y));
        }
    }
}

void insertCentroids(char A[][Y]){
    if(cent.size() == 0){
        int x = rand() % X;
        int y = rand() % Y;
        cent.push_back(Cent(A,x,y));
    }else{  
        bool valid = false;
        while(!valid){
            int x = rand() % X;
            int y = rand() % Y;
            for(auto i : cent){
                if((x < i.x - X*CENTDIST || x > i.x + X*CENTDIST) && (y < i.y - Y*CENTDIST || y > i.y + Y*CENTDIST)){
                    cent.push_back(Cent(A,x,y));
                    valid = true;
                    nc++;
                }
            }
        }
    }
}

void assignPoints(char A[][Y]){
    for(int j=0; j < points.size(); j++){
        int min_dist = X*Y;
        Cent* tempCent = NULL;
        for(int i=0; i < cent.size(); i++){
            int dist = sqrt(pow(cent[i].x - points[j].x,2) + pow(cent[i].y - points[j].y,2));
            if(dist < min_dist){
                min_dist = dist;
                tempCent = &cent[i];
            }
        }
        points[j].setNear(tempCent);
        tempCent->nearPoints.push_back(&points[j]);
    }
}

Point getCenter(Cent c){
    Point cp;
    int sum[2] = {0,0};
    for(int j = 0; j < c.nearPoints.size(); j++){
        sum[0] += c.nearPoints[j]->x;
        sum[1] += c.nearPoints[j]->y;
    }
    cp.x = sum[0]/c.nearPoints.size();
    cp.y = sum[1]/c.nearPoints.size();
    return cp;
}

void moveCent(char A[][Y]){
    for(int i=0; i < cent.size(); i++){
        Point p = getCenter(cent[i]);

        int move_x = 0;
        int move_y = 0;
        if(p.x < cent[i].x){
            move_x = -1;
        }else if(p.x > cent[i].x){
            move_x = 1;
        }
        if(p.y < cent[i].y){
            move_y = -1;
        }else if(p.y > cent[i].y){
            move_y = 1;
        }
        cent[i].setOnMat(A,cent[i].x + move_x,cent[i].y + move_y);
        cout<<"mosso di: "<<move_x<<" "<<move_y<<endl;
    }
}

void ClearScreen()
    {
    int n;
    for (n = 0; n < 10; n++)
      printf( "\n\n\n\n\n\n\n\n\n\n" );
    }

int main(){


    srand (time(NULL));
    //set all matrix to default value -> ' '
    resetMat(M);

    //set number of points to be randomly insert into the matrix
    int nPoints = 50;
    int nAgg = 3;
    insertPoints(M,nPoints,nAgg);

    //insert centroids
    insertCentroids(M);
    insertCentroids(M);
    insertCentroids(M);


    while(true){
        assignPoints(M);
        moveCent(M);
        ClearScreen();
        printMat(M);
        sleep(1);

    }
}


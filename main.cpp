#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iomanip>
#include <stack>
#include <cstring>

class EightPuzzle{
private:
    int game[5][5], temp[5][5];
    int border = -1, space = 0;
    int up = 0, down = 1, left = 2, right = 3;
    int goal[5][5] = {{-1, -1, -1, -1, -1},
                      {-1,  1,  2,  3, -1},
                      {-1,  8,  0,  4, -1},
                      {-1,  7,  6,  5, -1},
                      {-1, -1, -1, -1, -1}};
    int initial_x, initial_y, temp_x, temp_y;
    int stop = -1;
    std::stack<int> nodes;
    int node = 0;
    int tree[];

public:
    EightPuzzle(int random_num){
        srand(random_num);
        make_game();
        found_space();
        print_initial_space();
        print_game(game);
        copy_array();
    }
    void make_game(){
    	int exist[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    	int inside[9];
        int num = 0;
        int x;
        /*
         * 隨機產生0~8的數列 並確保未重複 存入inside[]
         */
        while(num < 9){
            x = rand() % 9;
            if (exist[x] == 0){
                inside[num++] = x;
                exist[x] = 1;
            }
        }
        /*
         * 將上面產生的數列存入game[][]裡面 並加入border
         */
        num = 0;

        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                if (i == 0 || i == 4 || j == 0 || j == 4)
                    game[i][j] = border;
                else
                    game[i][j] = inside[num++];
            }
        }
    }
    void found_space(){
        for (int i = 1; i < 4; ++i) {
            for (int j = 1; j < 4; ++j) {
                if(game[i][j] == space){
                    initial_x = i;
                    initial_y = j;
                }
            }
        }
    }
    void print_game(int target[][5]){
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                if( !is_border(target[i][j]) )
                    std::cout << std::setw(2) << target[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
    void print_initial_space(){
        std::cout << "(" << initial_x << ", " << initial_y << ")" << std::endl;
    }
    void print_space(){
        std::cout << "(" << temp_x << ", " << temp_y << ")" << std::endl;
    }
    void print_tree(){
        for (int i = 0; i < node; ++i) {
            std::cout << tree[i] << " ";
        }
    }

    bool is_goal(int current[][5]){
        if (memcmp(current, goal, sizeof (game)) == 0){
            return true;
        }
        return false;
    }
    bool is_border(int target){
        if (target == border){
            return true;
        }
        return false;
    }
    bool is_pre_step(){
    	
	}
    void get_route(){

    }

    void copy_array(){
        memcpy(temp, game, sizeof (game));
    }
    bool moving(int x, int y, int dir){
        int temp_num;
        int new_x, new_y;
        switch (dir) {
            case 0:
                // up x-1
                if (is_border(temp[x - 1][y])){
                    return false;
                }
                new_x = x - 1;
                new_y = y;
                break;
            case 1:
                // down x+1
                if (is_border(temp[x+1][y])){
                    return false;
                }
                new_x = x + 1;
                new_y = y;
                break;
            case 2:
                // left y-1
                if (is_border(temp[x][y - 1])){
                    return false;
                }
                new_x = x;
                new_y = y - 1;
                break;
            case 3:
                // left y+1
                if (is_border(temp[x][y + 1])){
                    return false;
                }
                new_x = x;
                new_y = y + 1;
                break;
            default:
                break;
        }

        temp_num = temp[x][y];
        temp[x][y] = temp[new_x][new_y];
        temp[new_x][new_y] = temp_num;
        temp_x = new_x;
        temp_y = new_y;

        return true;
    }
    void start_solve(){
        for (int dir = 0; dir < 4; ++dir) {
            copy_array();
            if (moving(initial_x, initial_y, dir)){
                if(is_goal(temp))
                    tree[node++] = 4;
                else
                    tree[node++] = dir;
                std::cout << dir << std::endl;
                print_game(temp);
                print_space();
            }
            else
                tree[node++] = stop;
        }

    }
    void solving_game( ){


    }

    
};

int main() {
    srand( time(NULL) );

    EightPuzzle puzzle1(1);
    //puzzle1.print_game();
    puzzle1.start_solve();
    puzzle1.print_tree();
    //puzzle1.print_initial_space();


    return 0;
}

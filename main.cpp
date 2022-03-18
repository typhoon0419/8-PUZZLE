#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iomanip>
#include <stack>
#include <cstring>

int total_win = 0;
class EightPuzzle{
private:
    int initial_game[5][5]={-1};
    int temp_arr[5][5]; // original arr & current xy
    int border = -1, space = 0;
    int goal[5][5] = {{-1, -1, -1, -1, -1},
                      {-1,  1,  2,  3, -1},
                      {-1,  4,  5,  6, -1},
                      {-1,  7,  8,  0, -1},
                      {-1, -1, -1, -1, -1}};
    int initial_x, initial_y, temp_x, temp_y; // original xy & current xy
    int count_steps = 0;
    int stop = -1;

    int parent_node = 0, current_node = 1;
    int future_node;
    bool moving_success, is_stop=false;
    std::stack<int> steps;
    int tree[250000][2]; // [dir, parent current_node]


public:

    EightPuzzle(int random_num){
        srand(random_num);
        std::cout << "initial puzzle:";
        make_game_that_can_solved();
        tree[0][0]=-1;
        tree[0][1]=-1;

        future_node = 5;
        copy_array();
        start_solve();
        solving_game();
    }

    void repeat(int a){
        total_win = 0;
        for (int i = 0; i < 1000; ++i) {
            is_stop=false;
            parent_node = 0;
            current_node = 1;
            future_node = 5;
            tree[0][0]=-1;
            tree[0][1]=-1;
            count_steps = 0;

            srand(i);
            if (a == 1)
                make_game_that_can_solved();
            else if (a == 2)
                make_game_in_random();


            copy_array();
            start_solve();
            solving_game();
        }
    }

    void make_game_in_random(){
    	int exist[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    	int inside[9];
        int num = 0;
        int x;

        while(num < 9){ // 隨機產生0~8的數列 並確保未重複 存入inside[]
            x = rand() % 9;
            if (exist[x] == 0){
                inside[num++] = x;
                exist[x] = 1;
            }
        }

        num = 0;
        for (int i = 0; i < 5; ++i) { //將上面產生的數列存入game[][]裡面 並加入border
            for (int j = 0; j < 5; ++j) {
                if (i == 0 || i == 4 || j == 0 || j == 4)
                    initial_game[i][j] = border;
                else
                    initial_game[i][j] = inside[num++];
            }
        }
        print_game(initial_game);
        found_space();
        print_initial_space();
    }
    void make_game_that_can_solved(){
        int board[9]={0};
        int i,j,n;
        int zero;
        int size = 9;
        int side = 3;
        int number = 50;
        srand(time(NULL));
        for(i=1;i<9;i++)
        {
            board[i-1]=i;
        }
        board[size-1]=0;    zero=size-1;

        /*
        for(i=0;i<size;i++)
        {
            printf("%d",board[i]);
        }
        */

        int x,y,tmp;
        x=zero/side;    y=zero%side;
        for(i=0;i<number;i++)
        {
            n=rand()%4;
            switch(n)
            {
                case 0:
                    x--;
                    if(x<0) {x++; continue;}
                    tmp = board[(x+1)*side+y];
                    board[(x+1)*side+y] = board[x*side+y];
                    board[x*side+y] = tmp;
                    break;
                case 1:
                    y--;
                    if(y<0) {y++; continue;}
                    tmp = board[x*side+(y+1)];
                    board[x*side+(y+1)] = board[x*side+y];
                    board[x*side+y] = tmp;
                    break;
                case 2:
                    x++;
                    if(x>side-1) {x--; continue;}
                    tmp = board[(x-1)*side+y];
                    board[(x-1)*side+y] = board[x*side+y];
                    board[x*side+y] = tmp;
                    break;
                case 3:
                    y++;
                    if(y>side-1) {y--; continue;}
                    tmp = board[x*side+(y-1)];
                    board[x*side+(y-1)] = board[x*side+y];
                    board[x*side+y] = tmp;
                    break;
            }
        }
        i = 0;
        for (int k = 0; k < 5; ++k) {
            for (int l = 0; l < 5; ++l) {
                if (k == 0 || k == 4 || l == 0 || l == 4)
                    initial_game[k][l] = border;
                else
                    initial_game [k][l] = board[i++];
            }
        }
        print_game(initial_game);
        found_space();
        print_initial_space();
    }
    void found_space(){
        for (int i = 1; i < 4; ++i) {
            for (int j = 1; j < 4; ++j) {
                if(initial_game[i][j] == space){
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
        std::cout << "(" << temp_x << ", " << temp_y << ")" << std::endl << std::endl;
    }
    void print_solved(){
        int i=1;
        copy_array();
        while (!steps.empty()){
            std::cout << "The " << i++ << " step" << std::endl;
            moving(temp_x, temp_y, steps.top());
            if (moving_success){
                print_game(temp_arr);
                std::cout << "step: " << steps.top() << std::endl;
                print_space();
            }
            steps.pop();
        }
        return;
    }


    bool is_goal(int current[][5]){
        if (memcmp(current, goal, sizeof (initial_game)) == 0)
            return true;
        return false;
    }
    bool is_border(int target){
        if (target == border)
            return true;
        return false;
    }
    void add_route(){
        future_node = current_node;
        for (int dir = 0; dir < 4; ++dir) {
            if (tree[parent_node][0] == stop){
                is_stop = true;
                return;
            }
            else if (tree[parent_node][0] == 0 && dir == 1)
                continue;
            else if (tree[parent_node][0] == 1 && dir == 0)
                continue;
            else if (tree[parent_node][0] == 2 && dir == 3)
                continue;
            else if (tree[parent_node][0] == 3 && dir == 2)
                continue;
            tree[future_node][0] = dir;
            tree[future_node][1] = parent_node;
            //std::cout << "future_node: " << future_node << std::endl;
            future_node++;
        }
    }
    void get_steps(int index){
        if (tree[index][1] == -1)
            return;

        steps.push(tree[index][0]);
        get_steps(tree[index][1]);
        count_steps++;
    }

    void copy_array(){
        memcpy(temp_arr, initial_game, sizeof (initial_game));
        temp_x = initial_x;
        temp_y = initial_y;
    }
    void moving(int x, int y, int dir){
        if (dir==stop){
            moving_success = false;
            return;
        }
        int temp_num;
        int new_x, new_y;
        switch (dir) {
            case 0:
                // up x-1
                if (is_border(temp_arr[x - 1][y])){
                    moving_success = false;
                    return;
                }
                new_x = x - 1;
                new_y = y;
                break;
            case 1:
                // down x+1
                if (is_border(temp_arr[x + 1][y])){
                    moving_success = false;
                    return;
                }
                new_x = x + 1;
                new_y = y;
                break;
            case 2:
                // left y-1
                if (is_border(temp_arr[x][y - 1])){
                    moving_success = false;
                    return;
                }
                new_x = x;
                new_y = y - 1;
                break;
            case 3:
                // left y+1
                if (is_border(temp_arr[x][y + 1])){
                    moving_success = false;
                    return;
                }
                new_x = x;
                new_y = y + 1;
                break;
            default:
                break;
        }

        temp_num = temp_arr[x][y];
        temp_arr[x][y] = temp_arr[new_x][new_y];
        temp_arr[new_x][new_y] = temp_num;
        temp_x = new_x;
        temp_y = new_y;


        moving_success = true;
        return;
    }
    void start_solve(){
        for (int dir = 0; dir < 4; ++dir) {
            copy_array();
            moving(temp_x, temp_y, dir);

            if (moving_success){
                if(is_goal(temp_arr))
                    tree[current_node][0] = 4;
                else
                    tree[current_node][0] = dir;
            }
            else
                tree[current_node][0] = stop;
            tree[current_node++][1] = parent_node;
        }
        return;
    }
    void go_steps(){
        while(true){
            if(steps.empty())
                break;

            moving(temp_x,temp_y,steps.top());
            if (!moving_success)
                tree[current_node][0] = stop;

            steps.pop();
        }
    }
    void solving_game( ){
        while (true){
            parent_node++;
            add_route();
            if (is_stop == true){
                is_stop = false;
                continue;
            }

            for (int i = 1; i < 4; ++i) {
                copy_array();
                if (count_steps > 20){
                    std::cout << "TOO many steps" << std::endl;
                    return;
                }
                if (future_node > 200000){
                    std::cout << "The tree is too big." << std::endl;
                    return;
                }
                count_steps = 0;
                get_steps(current_node);

                //std::cout << "parent_node: " << parent_node << " " << tree[parent_node][0] << " "  << tree[parent_node][1] << " " << std::endl;
                //std::cout << "current_node: " << current_node << " " << tree[current_node][0] << " "  << tree[current_node][1] << " " << std::endl;

                go_steps();

                if (is_goal(temp_arr)){
                    total_win ++ ;
                    std::cout << "The game is solved. Now start print the steps to solved the problem." << std::endl;
                    get_steps(current_node);
                    print_solved();
                    return;
                }
                current_node++;
            }
        }
    }


};

int main() {
    srand( time(NULL) );

    char a;
    int choice;

    EightPuzzle puzzle1(1);

    std::cout << "Would you want to see the next 1000 solves? Y/N" << std::endl;

    std::cin >> a;



    if(a == 'Y'||a=='y'){
        std::cout << "Please choose a create game method." << std::endl;
        std::cout << "1 or odd for make_game_that_can_solved()" << std::endl;
        std::cout << "2 or even for make_game_in_random()" << std::endl;

        std::cin >> choice;
        puzzle1.repeat(a%2);



        if (a%2 == 0)
            std::cout << "Using make_game_in_random() to create initial game board." << std::endl;
        else
            std::cout << "Using make_game_that_can_solved() to create initial game board." << std::endl;
        std::cout << total_win << " question is solved" << std::endl;

        std::cout << "Percentage of solved: " << float(total_win/1000*100) << "%" << std::endl;
    }


    return 0;
}

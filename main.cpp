#include <stdio.h>
#include <stdio.h>
#include <vector>
#include <queue>
#include <iostream>

void load_parameters(std::vector<std::vector<char>> &map, int &row_count, int &col_count, int &worker_pos, int &box_pos, int &place_pos)
{
    scanf("%d %d\n", &row_count, &col_count);
    map = std::vector<std::vector<char>>(row_count, std::vector<char>(col_count));
    for(int i = 0; i < row_count; i++)
    {
        for(int j = 0; j < col_count; j++)
        {
            char c = getchar();
            map[i][j] = c;
            if (c == '-')
                worker_pos = i * col_count + j;
            else if (c == 'O')
                box_pos = i * col_count + j;
            else if (c == '*')
                place_pos = i * col_count + j;
        }
        getchar();
    }
}

void print_parameters(std::vector<std::vector<char>> &map, int row_count, int col_count, int worker_pos, int box_pos, int place_pos)
{
    printf("\n");
    for(int i = 0; i < row_count; i++)
    {
        for(int j = 0; j < col_count; j++)
            printf("%c", map[i][j]);
        printf("\n");
    }
    printf("worker_pos: %d\n", worker_pos);
    printf("box_pos: %d\n", box_pos);
    printf("place_pos: %d\n", place_pos);
}

std::vector<char> bfs(std::vector<std::vector<char>> &map, int row_count, int col_count, int worker_pos, int box_pos, int place_pos)
{
    int size = row_count * col_count;
    std::vector<std::vector<char>> states(size, std::vector<char>(size, '0'));
    states[worker_pos][box_pos] = 'S';
    std::queue<std::pair<int, int>> queue;

    queue.push(std::pair<int, int>(worker_pos, box_pos));

    while(!queue.empty())
    {
        std::pair<int, int> current = queue.front();
        queue.pop();
        int worker_row = current.first/col_count;
        int worker_col = current.first%col_count;
        int box_row = current.second/col_count;
        int box_col = current.second%col_count;


        if(current.second == place_pos)
        {
            std::vector<char> solution;
            std::pair<int, int> start(worker_pos, box_pos);

            while(current != start)
            {
                char direct = states[current.first][current.second];
                solution.push_back(direct);
                switch(direct)
                {
                    case 'U':
                        current.second += col_count;
                    case 'u':
                        current.first += col_count;
                        break;
                    case 'D':
                        current.second -= col_count;
                    case 'd':
                        current.first -= col_count;
                        break;
                    case 'L':
                        current.second += 1;
                    case 'l':
                        current.first += 1;
                        break;
                    case 'R':
                        current.second -= 1;
                    case 'r':
                        current.first -= 1;
                        break;
                }
            }
            return solution;
        }

        //go up
        if(worker_row > 0 &&                                           //There is a place above
            current.first - col_count != current.second &&             //there is no box there      
            map[worker_row - 1][worker_col] != '#' &&                  //there is no wall there
            states[current.first - col_count][current.second] == '0')   //hasn't been there before
        {
            states[current.first - col_count][current.second] = 'u';
            queue.push(std::pair<int, int>(current.first-col_count, current.second));
        }
        //go down
        if(worker_row < row_count - 1 &&                                           //There is a place above
            current.first + col_count != current.second &&             //there is no box there
            map[worker_row + 1][worker_col] != '#' &&                  //there is no wall there
            states[current.first + col_count][current.second] == '0')   //hasn't been there before
        {
            states[current.first + col_count][current.second] = 'd';
            queue.push(std::pair<int, int>(current.first+col_count, current.second));
        }
        //go left
        if(worker_col > 0 &&                                           //There is a place above
            current.first - 1 != current.second &&             //there is no box there
            map[worker_row][worker_col-1] != '#' &&                  //there is no wall there
            states[current.first-1][current.second] == '0')   //hasn't been there before
        {
            states[current.first-1][current.second] = 'l';
            queue.push(std::pair<int, int>(current.first-1, current.second));
        }
        //go right
        if(worker_col < col_count - 1 &&                                         //There is a place above
            current.first + 1 != current.second &&             //there is no box there
            map[worker_row][worker_col + 1] != '#' &&                  //there is no wall there
            states[current.first + 1][current.second] == '0')   //hasn't been there before
        {
            states[current.first + 1][current.second] = 'r';
            queue.push(std::pair<int, int>(current.first + 1, current.second));
        }


        //go up dragging box
        if(worker_row > 0 &&                                           //There is a place above
            current.first + col_count == current.second &&             //there is a box behind      
            map[worker_row - 1][worker_col] != '#' &&                  //there is no wall there
            states[current.first - col_count][current.second - col_count] == '0')   //hasn't been there before
        {
            states[current.first - col_count][current.second - col_count] = 'U';
            queue.push(std::pair<int, int>(current.first-col_count, current.second-col_count));
        }
        //go down dragging box
        if(worker_row < row_count - 1 &&                                           //There is a place above
            current.first - col_count == current.second &&             //there is a box behind
            map[worker_row + 1][worker_col] != '#' &&                  //there is no wall there
            states[current.first + col_count][current.second + col_count] == '0')   //hasn't been there before
        {
            states[current.first + col_count][current.second + col_count] = 'D';
            queue.push(std::pair<int, int>(current.first+col_count, current.second + col_count));
        }
        //go left dragging box
        if(worker_col > 0 &&                                           //There is a place above
            current.first + 1 == current.second &&             //there is a box behind
            map[worker_row][worker_col-1] != '#' &&                  //there is no wall there
            states[current.first - 1][current.second - 1] == '0')   //hasn't been there before
        {
            states[current.first - 1][current.second - 1] = 'L';
            queue.push(std::pair<int, int>(current.first - 1, current.second - 1));
        }
        //go right dragging box
        if(worker_col < col_count - 1 &&                                         //There is a place above
            current.first - 1 == current.second &&             //there is a box behind
            map[worker_row][worker_col + 1] != '#' &&                  //there is no wall there
            states[current.first + 1][current.second + 1] == '0')   //hasn't been there before
        {
            states[current.first + 1][current.second + 1] = 'R';
            queue.push(std::pair<int, int>(current.first + 1, current.second + 1));
        }    
    }
    return std::vector<char>(0);
}


int main()
{
    std::vector<std::vector<char>> map;
    int row_count;
    int col_count;
    int worker_pos;
    int box_pos;
    int place_pos;

    load_parameters(map, row_count, col_count, worker_pos, box_pos, place_pos);
    //print_parameters(map, row_count, col_count, worker_pos, box_pos, place_pos);

    std::vector<char> solution = bfs(map, row_count, col_count, worker_pos, box_pos, place_pos);
    for(int i = solution.size()-1; i >= 0; i--)
    {
        printf("%c", solution[i]);
    }
    
}

/*
Example input:

9 8
########
#####  #
#      #
# ###  #
# O  - #
# ###  #
#   *  #
#####  #
########
*/


/*
########   0
#####  #   8
#      #   16
# ###  #   24
# O  - #   32
# ###  #   40
#   *  #   48
#####  #   56
########   64
*/

/*
########   0
#####  #   8
#      #   16
# ### -#   24
#    O #   32
# ###  #   40
#   *  #   48
#####  #   56
########   64
*/

/*
29 29
##### #####       #######    
#-  ###   #########  #  #####
# #   O #            #      #
#     #   # ##       #   #  #
# ### ##       #  #     #  ##
#  #   #   #      # #      # 
#  #   #       ####   # ## # 
## ##  ###   # #####    ## # 
#         # #     ##       # 
#  #  #           ##  #    # 
#        #   ####### #   # ##
# #  #   #   #    #         #
#        ##### #  #  #  ##  #
#  #  ##     #  ##       ####
#  #  ##        ##  #  # ### 
########   ###  ##       ####
#  #   ##   ########  # #   #
#           ##     #  #     #
##    #   #### #   #  ###  ##
#  ####  #      # ##       ##
#        # #  #        #    #
## ####           #  #   #  #
#   #        ## *##        ##
# #     #  # ##   #   # #  # 
#     #      ##   #        # 
## #   # #  #   #####    # # 
#    #      #     #    #   # 
#  #   #  ###     #        # 
########### #################

*/
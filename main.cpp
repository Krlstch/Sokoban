#include <stdio.h>
#include <stdio.h>
#include <vector>
#include <queue>
#include <iostream>
#include <unordered_map>

int state_size;

class State
{
public:
    uint16_t* values;

    State(uint16_t number, std::vector<uint16_t> vec)
    {
        values = new uint16_t[state_size];
        values[0] = number;
        for(int i = 1; i < state_size; i++)
            values[i] = vec[i-1];
    }

    State(uint16_t* array)
    {
        values = new uint16_t[state_size];
        for(int i = 0; i < state_size; i++)
            values[i] = array[i];
    }

    State(const State &other)
    {
        values = new uint16_t[state_size];
        for(int i = 0; i < state_size; i++)
            values[i] = other.values[i];
    }

    ~State()
    {
        delete [] values;
    }

    void replace(uint16_t old_val, uint16_t new_val)
    {
        for(int i = 1; i < state_size; i++)
        {
            if(values[i] == old_val)
            {
                if(new_val > old_val)
                {
                    while(i < state_size-1)
                    {
                        if(values[i+1] < new_val)
                            values[i] = values[i+1];
                        else
                        {
                            values[i] = new_val;
                            break;
                        }
                        i++;   
                    }
                    if(i == state_size-1 && values[i-1] != new_val)
                        values[i] = new_val;
                }
                else
                {
                    while(i > 1)
                    {
                        if(values[i-1] > new_val)
                            values[i] = values[i-1];
                        else
                        {
                            values[i] = new_val;
                            break;
                        }
                        i--;   
                    }
                    if(i == 1 && values[i+1] != new_val)
                        values[i] = new_val;

                }
                break;
            }
        }
    }
    
    struct HashFunction
    {
        size_t operator()(const State& state) const
        {
            size_t hash = 1;
            for(int i = 0; i < state_size; i++)
                hash *= state.values[i];
            return hash;
        }
    };
};

std::ostream& operator<<(std::ostream &ost, const State &state)
{
    ost << "[";
    for(int i = 0; i < state_size - 1; i++)
        ost << state.values[i] << ", ";
    ost << state.values[state_size-1] << "]";
    return ost;
}

bool operator==(const State &state1, const State &state2)
{
    for(int i = 0; i < state_size; i++)
        if(state1.values[i] != state2.values[i])
            return false;
    return true;
}

bool operator!=(const State &state1, const State &state2)
{
    return !(state1 == state2);
}

bool are_box_in_same_place(State& state1, State& state2)
{
    for(int i = 1; i < state_size; i++)
        if(state1.values[i] != state2.values[i])
            return false;
    return true;
}

bool is_empty_place(uint16_t position, const State &state)
{
    for(int i = 1; i < state_size; i++)
        if(position == state.values[i])
            return false;
    return true;
}

void load_parameters(std::vector<std::vector<char>> &map, int &row_count, int &col_count, int &worker_pos, std::vector<uint16_t> &box_pos, std::vector<uint16_t> &place_pos)
{
    scanf("%d %d\n", &row_count, &col_count);
    map = std::vector<std::vector<char>>(row_count, std::vector<char>(col_count));
    for(int i = 0; i < row_count; i++)
    {
        for(int j = 0; j < col_count; j++)
        {
            char c = getchar();
            //printf("(%c)", c);
            map[i][j] = c;
            if (c == '-')
                worker_pos = i * col_count + j;
            else if (c == 'O')
                box_pos.push_back(i * col_count + j);
            else if (c == '*')
                place_pos.push_back(i * col_count + j);
        }
        //printf("\\n\n");
        getchar();
    }
    state_size = box_pos.size() + 1;
}

void print_parameters(std::vector<std::vector<char>> &map, int row_count, int col_count, int worker_pos, std::vector<uint16_t> box_pos, std::vector<uint16_t> place_pos)
{
    printf("\n");
    printf("row_count: %d, col_count: %d\n", row_count, col_count);
    for(int i = 0; i < row_count; i++)
    {
        for(int j = 0; j < col_count; j++)
            printf("%c", map[i][j]);
        printf("\n");
    }
    printf("worker_pos: %d\n", worker_pos);
    printf("box_pos: ");
    for(int j = 0; j < box_pos.size(); j++)
        printf("%d, ", box_pos[j]);
    printf("\n");
    printf("place_pos:");
    for(int j = 0; j < place_pos.size(); j++)
        printf("%d, ", place_pos[j]);
    printf("\n");
}

std::vector<char> bfs(std::vector<std::vector<char>> &map, int row_count, int col_count, int worker_pos, std::vector<uint16_t> box_pos, std::vector<uint16_t> place_pos)
{
    int size = row_count * col_count;
    std::unordered_map<State, char, State::HashFunction> states;
    std::queue<State> queue;

    State start(worker_pos, box_pos);
    State end(worker_pos, place_pos);
    queue.push(start);

    //std::cout<< "end: " << end << std::endl;


    while(!queue.empty())
    {
        State current = queue.front();
        queue.pop();

        //std::cout << "Current: " << current;
        //std::cout << std::endl;   
        //In right place
        if(are_box_in_same_place(current, end))
        {
            //printf("Found solution\n");
            std::vector<char> solution;

            while(current != start)
            {
                char direct = states[current];
                solution.push_back(direct);
                switch(direct)
                {
                    case 'U':
                        current.replace(current.values[0] + col_count, current.values[0] + 2*col_count);
                    case 'u':
                        current.values[0] += col_count; 
                        break;
                    case 'D':
                        current.replace(current.values[0] - col_count, current.values[0] - 2*col_count);
                    case 'd':
                        current.values[0] -= col_count;
                        break;
                    case 'L':
                        current.replace(current.values[0] + 1, current.values[0] + 2);
                    case 'l':
                        current.values[0] += 1;
                        break;
                    case 'R':
                        current.replace(current.values[0] - 1, current.values[0] - 2);
                    case 'r':
                        current.values[0] -= 1;
                        break;
                }
            }
            return solution;
        }

        int worker_row = current.values[0] / col_count;
        int worker_col = current.values[0] % col_count;
        
        //go up
        if(worker_row > 0 &&                                           //There is a place above
            is_empty_place(current.values[0] - col_count, current) &&             //there is no box there      
            map[worker_row - 1][worker_col] != '#')               //there is no wall there
        {
            State new_state(current);
            new_state.values[0] -= col_count;
            //std::cout << current << " -> " << new_state << " u; ";
            if(states.find(new_state) == states.end())                  //hasn't been there before
            {
                //std::cout << " acc";
                states[new_state] = 'u';
                queue.push(new_state);
            }
            //std::cout << std::endl;
        }

        //go down
        if(worker_row < row_count - 1  &&                                           //There is a place below
            is_empty_place(current.values[0] + col_count, current) &&             //there is no box there      
            map[worker_row + 1][worker_col] != '#')                  //there is no wall there
        {
            State new_state(current);
            new_state.values[0] += col_count;
            //std::cout << current << " -> " << new_state << " d; ";
            if(states.find(new_state) == states.end())                  //hasn't been there before
            {
                //std::cout << " acc";
                states[new_state] = 'd';
                queue.push(new_state);
            }
            //std::cout << std::endl;
        }

        //go left
        if(worker_col > 0 &&                                           //There is a place to the left
            is_empty_place(current.values[0] - 1, current) &&             //there is no box there      
            map[worker_row][worker_col - 1] != '#')                  //there is no wall there
        {
            State new_state(current);
            new_state.values[0] -= 1;
            //std::cout << current << " -> " << new_state << " l; ";
            if(states.find(new_state) == states.end())                  //hasn't been there before
            {
                //std::cout << " acc";
                states[new_state] = 'l';
                queue.push(new_state);
            }
            //std::cout << std::endl;
        }

        //go right
        if(worker_col < col_count - 1 &&                                           //There is a place to the right
            is_empty_place(current.values[0] + 1, current) &&             //there is no box there      
            map[worker_row][worker_col + 1] != '#')                  //there is no wall there
        {
            State new_state(current);
            new_state.values[0] += 1;
            //std::cout << current << " -> " << new_state << " r; ";
            if(states.find(new_state) == states.end())                  //hasn't been there before
            {
                //std::cout << " acc";
                states[new_state] = 'r';
                queue.push(new_state);
            }
            //std::cout << std::endl;
        }

        //go up dragging box
        if(worker_row > 0 &&                                           //There is a place above
            is_empty_place(current.values[0] - col_count, current) &&             //there is no box there      
            map[worker_row - 1][worker_col] != '#' &&                  //there is no wall there
            !is_empty_place(current.values[0] + col_count, current))     //there is a box behind
        {
            State new_state(current);
            new_state.values[0] -= col_count;
            new_state.replace(current.values[0] + col_count, current.values[0]);
            //std::cout << current << " -> " << new_state << " U; ";
            if(states.find(new_state) == states.end())                  //hasn't been there before
            {
                //std::cout << " acc";
                states[new_state] = 'U';
                queue.push(new_state);
            }
            //std::cout << std::endl;
        }

        //go down dragging box
        if(worker_row < row_count - 1  &&                                           //There is a place below
            is_empty_place(current.values[0] + col_count, current) &&             //there is no box there      
            map[worker_row + 1][worker_col] != '#' &&                  //there is no wall there
            !is_empty_place(current.values[0] - col_count, current))     //there is a box behind
        {
            State new_state(current);
            new_state.values[0] += col_count;
            new_state.replace(current.values[0] - col_count, current.values[0]);
            //std::cout << current << " -> " << new_state << " D; ";
            if(states.find(new_state) == states.end())                  //hasn't been there before
            {
                //std::cout << " acc";
                states[new_state] = 'D';
                queue.push(new_state);
            }
            //std::cout << std::endl;
        }

        //go left dragging box
        if(worker_col > 0 &&                                           //There is a place to the left
            is_empty_place(current.values[0] - 1, current) &&             //there is no box there      
            map[worker_row][worker_col - 1] != '#' &&                  //there is no wall there
            !is_empty_place(current.values[0] + 1, current))    //there is a box behind
        {
            State new_state(current);
            new_state.values[0] -= 1;
            new_state.replace(current.values[0] + 1, current.values[0]);
            //std::cout << current << " -> " << new_state << " L; ";
            if(states.find(new_state) == states.end())                  //hasn't been there before
            {
                //std::cout << " acc";
                states[new_state] = 'L';
                queue.push(new_state);
            }
            //std::cout << std::endl;
        }

        //go right dragging box
        if(worker_row < col_count - 1 &&                                           //There is a place to the right
            is_empty_place(current.values[0] + 1, current) &&             //there is no box there      
            map[worker_row][worker_col + 1] != '#' &&                  //there is no wall there
            !is_empty_place(current.values[0] - 1, current))     //there is a box behind
        {
            State new_state(current);
            new_state.values[0] += 1;
            new_state.replace(current.values[0] - 1, current.values[0]);
            //std::cout << current << " -> " << new_state << " R; ";
            if(states.find(new_state) == states.end())                  //hasn't been there before
            {
                //std::cout << " acc";
                states[new_state] = 'R';
                queue.push(new_state);
            }
            //std::cout << std::endl;
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
    std::vector<uint16_t> box_pos;
    std::vector<uint16_t> place_pos;

    load_parameters(map, row_count, col_count, worker_pos, box_pos, place_pos);
    //print_parameters(map, row_count, col_count, worker_pos, box_pos, place_pos);


    std::vector<char> solution = bfs(map, row_count, col_count, worker_pos, box_pos, place_pos);
    if(solution.size() == 0)
        printf("Solution not found\n");
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
# *  * #
# ###  #
# OOO- #
# ###  #
# *    #
####   #
########
*/

/*
9 8
########
#####  #
#  *   #
# ###  #
# -  O #
# ###  #
#      #
#####  #
########
*/

/*
11 11
      #### 
####### -# 
#     *  # 
#   *## *# 
##*#OOO# # 
 #       # 
 # #O O# ##
 #   # #* #
 #        #
 #  #######
 ####       
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

/*
128 128
O                                                                                                                               
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                                
                                                                                                                               *
                                                                                                                               -
                                                                                                                               */
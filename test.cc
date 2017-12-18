#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <cstdio>
#include <utility>
#include <map>
#include <cstdlib>

int x, y, z;

class State {
    public:
        std::vector<int> grid;
        State *parent;
        int depth;
        State(std::vector<int> g, State *p, int d): grid(g), parent(p), depth(d){}

        bool operator<(const State& a) const {
            return this->depth < a.depth;
        }
        bool operator<(int b) const {
            return this->depth < b;
        }
};

int findZero(std::vector<int> v){
    for(int i = 0; i < v.size(); i++){
        if(v[i] == 0) return i;
    }
    return -1;
}

std::set< std::vector<int> > visited;

bool isVisited(std::vector<int> v){
    return visited.count(v);
}

bool isGoalState(State s, std::vector<int> goal){
    return s.grid == goal;
}

int manhattanHeuristic(std::vector<int> grid, std::vector<int> goal){
    std::map<int, int> goalMap;

    for(int i = 0; i < goal.size(); i++){
        goalMap[goal[i]] = i;
    }

    int heuristic = 0;
    for(int i = 0; i < grid.size(); i++){
        if(grid[i] == 0){
            continue;
        } else {
            heuristic += abs( (i % x) - (goalMap[grid[i]] % x)) + abs(((i / x) % y) - ((goalMap[grid[i]] / x) % y)) + abs(((i / (x * y)) % z) - (((goalMap[grid[i]] / (x * y)) % z)));
        }

    }
    return heuristic;
}

//TODO: Fix Printing ancestors
void printAncestors(State s){
    if(s.depth == 0){
        std::cout << "Reached Initial State!" << std::endl;
        return;
    }

    std::cout << "Depth == " << s.depth;
    for(int i = 0; i < s.grid.size(); i++){
        if(i % x == 0){
            std::cout << std::endl;
        }
        std::cout << s.grid[i] << " ";
    }

    std::cout << std::endl;
    printAncestors(*s.parent);
}

void dfs(State current, std::vector<int> goal){
    visited.insert(current.grid);

    if(isGoalState(current, goal)){
        std::cout << "Depth is " << current.depth << std::endl;
        std::cout << "Goal state reached" << std::endl;
        //printAncestors(current);
        return;
    }
    //find the zero and move it in all possible positions
    int zeroIndex = findZero(current.grid);
    if((zeroIndex % x) < (x - 1) && zeroIndex + 1 < x * y * z){
        //you can move right
        std::vector<int> nextGrid = current.grid;
        std::swap(nextGrid[zeroIndex], nextGrid[zeroIndex + 1]);
        if(!isVisited(nextGrid)){
            State nextState(nextGrid, &current, current.depth + 1);
            dfs(nextState, goal);
        }
    }

    if((zeroIndex % x) > 0){
        //you can move left
        std::vector<int> nextGrid = current.grid;
        std::swap(nextGrid[zeroIndex], nextGrid[zeroIndex - 1]);
        if(!isVisited(nextGrid)){
            State nextState(nextGrid, &current, current.depth + 1);
            dfs(nextState, goal);
        }
    }

    if((zeroIndex % (x * y)) < (x * y - 1) && zeroIndex + x < x * y * z){
        //you can move up
        std::vector<int> nextGrid = current.grid;
        std::swap(nextGrid[zeroIndex], nextGrid[zeroIndex + x]);
        if(!isVisited(nextGrid)){
            State nextState(nextGrid, &current, current.depth + 1);
            dfs(nextState, goal);
        }
    }

    if((zeroIndex % (x * y)) > 0 && zeroIndex - x >= 0){
        //you can move down
        std::vector<int> nextGrid = current.grid;
        std::swap(nextGrid[zeroIndex], nextGrid[zeroIndex - x]);
        if(!isVisited(nextGrid)){
            State nextState(nextGrid, &current, current.depth + 1);
            dfs(nextState, goal);
        }
    }

    if((zeroIndex % (x * y * z)) < ((x * y * z) - 1) && zeroIndex + x * y < x * y * z){
        //you can move to the front
        std::vector<int> nextGrid = current.grid;
        std::swap(nextGrid[zeroIndex], nextGrid[zeroIndex + x * y]);
        if(!isVisited(nextGrid)){
            State nextState(nextGrid, &current, current.depth + 1);
            dfs(nextState, goal);
        }
    }

    if((zeroIndex % (x * y * z)) > 0 && zeroIndex - x * y >= 0){
        //you can move to the back
        std::vector<int> nextGrid = current.grid;
        std::swap(nextGrid[zeroIndex], nextGrid[zeroIndex - x * y]);
        if(!isVisited(nextGrid)){
            State nextState(nextGrid, &current, current.depth + 1);
            dfs(nextState, goal);
        }
    }
    return;
}

void bfs(State s, std::vector<int> goal){
    std::queue<State> bfsQueue;
    bfsQueue.push(s);
    visited.insert(s.grid);

    while(!bfsQueue.empty()){

        State current = bfsQueue.front();
        bfsQueue.pop();

        /*
        for(int i = 0; i < current.grid.size(); i++){
            if(i % x == 0) std::cout << std::endl;
            std::cout << current.grid[i] << " ";
        }
        std::cout << std::endl;
        */

        if(isGoalState(current, goal)){
            std::cout << "Depth is " << current.depth << std::endl;
            std::cout << "Goal state reached" << std::endl;
            //printAncestors(current);
            return;
        }

        int zeroIndex = findZero(current.grid);
        if((zeroIndex % x) < (x - 1)){
            //you can move right
            std::vector<int> nextGrid = current.grid;
            std::swap(nextGrid[zeroIndex], nextGrid[zeroIndex + 1]);
            if(!isVisited(nextGrid)){
                visited.insert(nextGrid);
                State nextState(nextGrid, &current, current.depth + 1);
                bfsQueue.push(nextState);
            }
        }
        if((zeroIndex % x) > 0){
            //you can move left
            std::vector<int> nextGrid = current.grid;
            std::swap(nextGrid[zeroIndex], nextGrid[zeroIndex - 1]);
            if(!isVisited(nextGrid)){
                visited.insert(nextGrid);
                State nextState(nextGrid, &current, current.depth + 1);
                bfsQueue.push(nextState);
            }
        }
        if((zeroIndex % (x * y)) < (x * y - 1) && zeroIndex + x < x * y * z){
            //you can move up
            std::vector<int> nextGrid = current.grid;
            std::swap(nextGrid[zeroIndex], nextGrid[zeroIndex + x]);
            if(!isVisited(nextGrid)){
                visited.insert(nextGrid);
                State nextState(nextGrid, &current, current.depth + 1);
                bfsQueue.push(nextState);
            }
        }
        if((zeroIndex % (x * y)) > 0 && zeroIndex - x > -1){
            //you can move down
            std::vector<int> nextGrid = current.grid;
            std::swap(nextGrid[zeroIndex], nextGrid[zeroIndex - x]);
            if(!isVisited(nextGrid)){
                visited.insert(nextGrid);
                State nextState(nextGrid, &current, current.depth + 1);
                bfsQueue.push(nextState);
            }
        }
        if((zeroIndex % (x * y * z)) < ((x * y * z) - 1) && zeroIndex + x * y < x * y * z){
            //you can move to the front
            std::vector<int> nextGrid = current.grid;
            std::swap(nextGrid[zeroIndex], nextGrid[zeroIndex + x * y]);
            if(!isVisited(nextGrid)){
                visited.insert(nextGrid);
                State nextState(nextGrid, &current, current.depth + 1);
                bfsQueue.push(nextState);
            }
        }
        if((zeroIndex % (x * y * z)) > 0 && zeroIndex - x * y > -1){
            //you can move to the back
            std::vector<int> nextGrid = current.grid;
            std::swap(nextGrid[zeroIndex], nextGrid[zeroIndex - x * y]);
            if(!isVisited(nextGrid)){
                visited.insert(nextGrid);
                State nextState(nextGrid, &current, current.depth + 1);
                bfsQueue.push(nextState);
            }
        }

    }
}

void greedyBFS(State s, std::vector<int> goal){
    std::priority_queue< std::pair<int, State> > bfsQueue;
    bfsQueue.push(std::make_pair(manhattanHeuristic(s.grid, goal), s));
    visited.insert(s.grid);

    while(!bfsQueue.empty()){

        State current = bfsQueue.top().second;
        bfsQueue.pop();

        /*
        for(int i = 0; i < current.grid.size(); i++){
            if(i % x == 0) std::cout << std::endl;
            std::cout << current.grid[i] << " ";
        }
        std::cout << std::endl;
        */

        if(isGoalState(current, goal)){
            std::cout << "Depth is " << current.depth << std::endl;
            std::cout << "Goal state reached" << std::endl;
            //printAncestors(current);
            return;
        }

        int zeroIndex = findZero(current.grid);
        if((zeroIndex % x) < (x - 1)){
            //you can move right
            std::vector<int> nextGrid = current.grid;
            std::swap(nextGrid[zeroIndex], nextGrid[zeroIndex + 1]);
            if(!isVisited(nextGrid)){
                visited.insert(nextGrid);
                State nextState(nextGrid, &current, current.depth + 1);
                bfsQueue.push(std::make_pair(manhattanHeuristic(nextGrid, goal), nextState));
            }
        }
        if((zeroIndex % x) > 0){
            //you can move left
            std::vector<int> nextGrid = current.grid;
            std::swap(nextGrid[zeroIndex], nextGrid[zeroIndex - 1]);
            if(!isVisited(nextGrid)){
                visited.insert(nextGrid);
                State nextState(nextGrid, &current, current.depth + 1);
                bfsQueue.push(std::make_pair(manhattanHeuristic(nextGrid, goal), nextState));
            }
        }
        if((zeroIndex % (x * y)) < (x * y - 1) && zeroIndex + x < x * y * z){
            //you can move up
            std::vector<int> nextGrid = current.grid;
            std::swap(nextGrid[zeroIndex], nextGrid[zeroIndex + x]);
            if(!isVisited(nextGrid)){
                visited.insert(nextGrid);
                State nextState(nextGrid, &current, current.depth + 1);
                bfsQueue.push(std::make_pair(manhattanHeuristic(nextGrid, goal), nextState));
            }
        }
        if((zeroIndex % (x * y)) > 0 && zeroIndex - x > -1){
            //you can move down
            std::vector<int> nextGrid = current.grid;
            std::swap(nextGrid[zeroIndex], nextGrid[zeroIndex - x]);
            if(!isVisited(nextGrid)){
                visited.insert(nextGrid);
                State nextState(nextGrid, &current, current.depth + 1);
                bfsQueue.push(std::make_pair(manhattanHeuristic(nextGrid, goal), nextState));
            }
        }
        if((zeroIndex % (x * y * z)) < ((x * y * z) - 1) && zeroIndex + x * y < x * y * z){
            //you can move to the front
            std::vector<int> nextGrid = current.grid;
            std::swap(nextGrid[zeroIndex], nextGrid[zeroIndex + x * y]);
            if(!isVisited(nextGrid)){
                visited.insert(nextGrid);
                State nextState(nextGrid, &current, current.depth + 1);
                bfsQueue.push(std::make_pair(manhattanHeuristic(nextGrid, goal), nextState));
            }
        }
        if((zeroIndex % (x * y * z)) > 0 && zeroIndex - x * y > -1){
            //you can move to the back
            std::vector<int> nextGrid = current.grid;
            std::swap(nextGrid[zeroIndex], nextGrid[zeroIndex - x * y]);
            if(!isVisited(nextGrid)){
                visited.insert(nextGrid);
                State nextState(nextGrid, &current, current.depth + 1);
                bfsQueue.push(std::make_pair(manhattanHeuristic(nextGrid, goal), nextState));
            }
        }

    }
}

// The A* Algorithm is basically the same as the greedyBFS + adding the depth to the heuristic function;
void aStar(State s, std::vector<int> goal){
    std::priority_queue< std::pair<int, State> > bfsQueue;
    bfsQueue.push(std::make_pair(s.depth + manhattanHeuristic(s.grid, goal), s));
    visited.insert(s.grid);

    while(!bfsQueue.empty()){

        State current = bfsQueue.top().second;
        bfsQueue.pop();

        if(isGoalState(current, goal)){
            std::cout << "Depth is " << current.depth << std::endl;
            std::cout << "Goal state reached" << std::endl;
            //printAncestors(current);
            return;
        }

        int zeroIndex = findZero(current.grid);
        if((zeroIndex % x) < (x - 1)){
            //you can move right
            std::vector<int> nextGrid = current.grid;
            std::swap(nextGrid[zeroIndex], nextGrid[zeroIndex + 1]);
            if(!isVisited(nextGrid)){
                visited.insert(nextGrid);
                State nextState(nextGrid, &current, current.depth + 1);
                bfsQueue.push(std::make_pair(nextState.depth + manhattanHeuristic(nextGrid, goal), nextState));
            }
        }
        if((zeroIndex % x) > 0){
            //you can move left
            std::vector<int> nextGrid = current.grid;
            std::swap(nextGrid[zeroIndex], nextGrid[zeroIndex - 1]);
            if(!isVisited(nextGrid)){
                visited.insert(nextGrid);
                State nextState(nextGrid, &current, current.depth + 1);
                bfsQueue.push(std::make_pair(nextState.depth + manhattanHeuristic(nextGrid, goal), nextState));
            }
        }
        if((zeroIndex % (x * y)) < (x * y - 1) && zeroIndex + x < x * y * z){
            //you can move up
            std::vector<int> nextGrid = current.grid;
            std::swap(nextGrid[zeroIndex], nextGrid[zeroIndex + x]);
            if(!isVisited(nextGrid)){
                visited.insert(nextGrid);
                State nextState(nextGrid, &current, current.depth + 1);
                bfsQueue.push(std::make_pair(nextState.depth + manhattanHeuristic(nextGrid, goal), nextState));
            }
        }
        if((zeroIndex % (x * y)) > 0 && zeroIndex - x > -1){
            //you can move down
            std::vector<int> nextGrid = current.grid;
            std::swap(nextGrid[zeroIndex], nextGrid[zeroIndex - x]);
            if(!isVisited(nextGrid)){
                visited.insert(nextGrid);
                State nextState(nextGrid, &current, current.depth + 1);
                bfsQueue.push(std::make_pair(nextState.depth + manhattanHeuristic(nextGrid, goal), nextState));
            }
        }
        if((zeroIndex % (x * y * z)) < ((x * y * z) - 1) && zeroIndex + x * y < x * y * z){
            //you can move to the front
            std::vector<int> nextGrid = current.grid;
            std::swap(nextGrid[zeroIndex], nextGrid[zeroIndex + x * y]);
            if(!isVisited(nextGrid)){
                visited.insert(nextGrid);
                State nextState(nextGrid, &current, current.depth + 1);
                bfsQueue.push(std::make_pair(nextState.depth + manhattanHeuristic(nextGrid, goal), nextState));
            }
        }
        if((zeroIndex % (x * y * z)) > 0 && zeroIndex - x * y > -1){
            //you can move to the back
            std::vector<int> nextGrid = current.grid;
            std::swap(nextGrid[zeroIndex], nextGrid[zeroIndex - x * y]);
            if(!isVisited(nextGrid)){
                visited.insert(nextGrid);
                State nextState(nextGrid, &current, current.depth + 1);
                bfsQueue.push(std::make_pair(nextState.depth + manhattanHeuristic(nextGrid, goal), nextState));
            }
        }

    }
}



int main(void){
    freopen("in.in", "r", stdin);
    //get dimensions of the grid
    std::cin >> x >> y >> z;
    int vectorSize = x * y * z;

    //Get initial State
    std::vector<int> inGrid(vectorSize);
    for(int i = 0; i < vectorSize; i++){
        std::cin >> inGrid[i];
    }
    State initialState = State(inGrid, NULL, 0);

    //Get goal state
    std::vector<int> goalGrid(vectorSize);
    for(int i = 0; i < vectorSize; i++){
        std::cin >> goalGrid[i];
    }

    std::cin.ignore();
    std::string alg; std::cin >> alg;

    if(alg == "dfs"){
        dfs(initialState, goalGrid);
    } else if(alg == "bfs"){
        bfs(initialState, goalGrid);
    } else if(alg == "greedyBFS"){
        greedyBFS(initialState, goalGrid);
    } else if(alg == "aStar"){
        aStar(initialState, goalGrid);
    }

    return 0;
}

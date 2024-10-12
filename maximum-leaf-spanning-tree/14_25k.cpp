#include <iostream>
#include <time.h>
#include <algorithm>
#include <vector>

using namespace std;


/*
Applying isConnected with removed vertices i.e. vertices with size = 0
Compare function calls from isBridge() and solveMLST() calls of isConnected()
*/

class MyException : public exception{
    const char* what () const throw(){
        return "Not exhaustively processed!";
    }
};

void addEdge(vector<int> adj[], int u, int v){
    adj[u].push_back(v);
    adj[v].push_back(u);
}

void removeEdge(vector<int> adj[], int v1, int v2){
    adj[v1].erase(remove(adj[v1].begin(), adj[v1].end(), v2), adj[v1].end());
    adj[v2].erase(remove(adj[v2].begin(), adj[v2].end(), v1), adj[v2].end());
}

void copyGraph(vector<int> adj[], vector<int>original[], int vertices){
    for(int i = 0; i < vertices; i++){
        original[i] = adj[i];
    }
}

void createGraph(vector<int> adj[], int vertices, int edges){
    vector<vector<int>> mat(vertices, vector<int>(vertices));

    cout << "Size of the adjacency Matrix: (" << mat.size() <<", " << mat[0].size() << ")" << endl;

    for(int i = 1; i < vertices; ++i){
        int neighbour = rand() % i;
        adj[i].push_back(neighbour);
        adj[neighbour].push_back(i);
        mat[i][neighbour] = 1;
        mat[neighbour][i] = 1;
    }

    for(int i = 0; i < edges - vertices + 1; ++i){
        while(true){
            int node1 = rand() % vertices;
            int node2 = rand() % vertices;

            if(mat[node1][node2] == 0 && node1 != node2){
                adj[node1].push_back(node2);
                adj[node2].push_back(node1);
                mat[node1][node2] = 1;
                mat[node2][node1] = 1;
                break;
            }
        }
    }
}

void printGraph(vector<int> adj[], int vertices){
    for (int v = 0; v < vertices; ++v){
        cout << "\nAdjacency list of vertex " << v << "\n head";
        vector<int>::iterator it;
        for (it = adj[v].begin(); it != adj[v].end(); ++it){
           cout << " -> " << *it;
        }
        printf("\n");
    }
}

void mergeVertices(vector<int> adj[], int v1, int v2){
    // Copy neighbours of v2 to v1
    vector<int>::iterator it;
    for(it = adj[v2].begin(); it != adj[v2].end(); it++){
        if(count(adj[v1].begin(), adj[v1].end(), *it)){
            continue;
        }
        adj[v1].push_back(*it);
    }
    adj[v2].clear();
}



void DFS(vector<int> adj[], int v, bool visited[]){
    visited[v] = true;

    vector<int>::iterator it;
    for(it = adj[v].begin(); it != adj[v].end(); ++it){
        if(!visited[*it]){
            DFS(adj, *it, visited);
        }
    }
}
  
bool isConnected(vector<int> adj[], int vertices){
    bool visited[vertices];
    memset(visited, false, sizeof(visited));
  
    DFS(adj, 0, visited);
    for(int i = 1; i < vertices; i++){
        if(visited[i] == false){
            return false;
        }
    }
    return true;
}

bool isBridge(vector<int> adj[], int v1, int v2, int vertices){
    vector<int>* x = new vector<int>[vertices];
    copyGraph(adj, x, vertices);

    x[v1].erase(remove(x[v1].begin(), x[v1].end(), v2), x[v1].end());
    x[v2].erase(remove(x[v2].begin(), x[v2].end(), v1), x[v2].end());
    bool is_connect = isConnected(x, vertices);

    delete[] x;

    if(is_connect){
        return false;
    }
    return true;
}

bool reductionOne(vector<int> adj[], int vertices){
    bool result = false;
    for(int i = 0; i < vertices; ++i){
        if(adj[i].size() > 1){
            vector<int>::iterator it;
            for (it = adj[i].begin(); it != adj[i].end(); ++it){
                if(adj[*it].size() > 1 && isBridge(adj, i, *it, vertices)){
                    result = true;
                    mergeVertices(adj, i, *it);
                }
            }
        }
    }
    return result;
}

bool reductionTwo(vector<int> adj[], int vertices){
    bool result = false;
    for(int i = 0; i < vertices; ++i){
        if(adj[i].size() == 2){
            vector<int>::iterator it;
            for(it = adj[i].begin(); it != adj[i].end(); ++it){
                if(adj[*it].size() == 2 && !isBridge(adj, i, *it, vertices)){
                    result = true;
                    adj[i].erase(it);
                    adj[*it].erase(remove(adj[*it].begin(), adj[*it].end(), i), adj[*it].end());
                }
            }
        }
    }
    return result;
}

bool reductionThree(vector<int> adj[], int vertices){
    bool result = false;
    for(int i = 0; i < vertices; ++i){
        if(adj[i].size() == 1){
            int w = adj[i][0];
            int not_leaf_check = false;
            vector<int>::iterator it;
            for(it = adj[w].begin(); it != adj[w].end(); ++it){
                if(adj[*it].size() > 1){
                    not_leaf_check = true;
                    break;
                }
            }
            if(not_leaf_check){
                result = true;
                adj[i].clear();
                adj[w].erase(remove(adj[w].begin(), adj[w].end(), i), adj[w].end());

                vector<int>::iterator it2, it1, ele_check_1, ele_check_2;
                for(it1 = adj[w].begin(); it1 != adj[w].end() - 1; ++it1){
                    for(it2 = it1 + 1; it2 != adj[w].end(); ++it2){
                        ele_check_1 = find(adj[*it1].begin(), adj[*it1].end(), *it2);
                        ele_check_2 = find(adj[*it2].begin(), adj[*it2].end(), *it1);

                        if(ele_check_1 != adj[*it1].end() && ele_check_2 != adj[*it2].end()){
                            adj[*it1].push_back(*it2);
                            adj[*it2].push_back(*it1);
                        }
                    }
                }                
            }
        }
    }
    return result;
}

void getComplementGraph(vector<int> adj[], vector<int> adj_k[], vector<int> bits_vector, int vertices, int k){
    int count = 0;
    for(int i = 0; i < vertices; ++i){
        if(!bits_vector[i]){
            vector<int>::iterator it;
            for(it = adj[i].begin(); it != adj[i].end(); ++it){
                if(!bits_vector[*it]){
                    adj_k[count].push_back(*it);
                }
            }
            count++;
        }
    }
}

bool isDominating(vector<int> adj[], vector<int> bits_vector, int vertices){
    for(int i = 0; i < vertices; ++i){
        if(bits_vector[i]){
            vector<int>::iterator it;
            bool is_neighbour = false;
            for(it = adj[i].begin(); it != adj[i].end(); ++it){
                if(!bits_vector[*it]){
                    is_neighbour = true;
                    break;
                }
            }
            if(!is_neighbour){
                return false;
            }
        }
    }
    return true;
}

bool solveMLST(vector<int> adj[], int vertices, int k){
    vector<int> bits_vector(vertices, 0);

    for (int i = vertices - k; i < vertices; i++){
        bits_vector[i] = 1;
    }

    do {
        vector<int> adj_k[vertices - k];
        getComplementGraph(adj, adj_k, bits_vector, vertices, k);
        if(isConnected(adj_k, vertices) && isDominating(adj, bits_vector, vertices)){
            return true;
        }
    } while (next_permutation(bits_vector.begin(), bits_vector.end()));
    return false;
}

int checkDegreeTwoPresent(vector<int> adj[], int vertices){
    for(int i = 0; i < vertices; ++i){
        if(adj[i].size() == 2){
            return i;
        }
    }
    return -1;
}

int verticesCount(vector<int> adj[], int vertices){
    int count = 0;
    for(int i = 0; i < vertices; ++i){
        if(adj[i].size() > 0){
            count++;
        }
    }
    return count;
}
bool MLST(vector<int> adj[], int vertices, int edges, int k){

    while(reductionOne(adj, vertices) || reductionTwo(adj, vertices) || reductionThree(adj, vertices)){

    }
    int two_deg = checkDegreeTwoPresent(adj, vertices);
    if(two_deg == -1){
        if(verticesCount(adj, vertices) >= 4*k - 8){
            return true;
        }
        else{
            if(solveMLST(adj, vertices, k)){
                return true;
            }
            else{
                return false;
            }
        }
    }
    else{
        int x = adj[two_deg][0], y = adj[two_deg][1];
        if(verticesCount(adj, vertices) > 5.75*k){
            return true;
        }
        else{
            int new_vertex = vertices;
            vector<int> adj_ann[vertices+1];
            copyGraph(adj, adj_ann, vertices);
            addEdge(adj_ann, x, vertices);
            if(solveMLST(adj_ann, vertices, k + 1)){
                return true;
            }
            removeEdge(adj_ann, x, vertices);
            addEdge(adj_ann, y, vertices);
            if(solveMLST(adj_ann, vertices, k + 1)){
                return true;
            }
            else{
                return false;
            }            
        }
    }
}

int main(){
    srand(time(0));
    int vertices = 7, edges = 10, k = 3;
    vector<int> adj[vertices], original[vertices];

    createGraph(adj, vertices, edges);
    copyGraph(adj, original, vertices);
    printGraph(adj, vertices);

    return 0;
}

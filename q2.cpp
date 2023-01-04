// GÖKSU GÜZ - 150180715
#include <iostream>
#include <string>
#include <fstream>
#include <string.h>
#include <map>
#include <algorithm> //only for sort operation
#include <vector>
#include<list>
#include <limits.h>

using namespace std;

class Graph{
public:
    //I hold the input data info in the two below
    map<int, string[2]> inFilePlaces; // keeps the places like [i][0] [i][1]
    vector<int> distanceP; // keeps the distance betweeen [i][0] [i][1]

    // the below is a function to check whether that node is taken to the places array or not.
    int checkIfThePlaceExist(string);
    //Below vector keeps the different node names like S1, Ma
    vector<string>places;
    //size of the different places(size of the places array)
    int difPlaces;

    //This is the graph where nodes will be kept
    int **graph;

    //Dijktras algorithm
    void dijkstrasFunction(int, int);

    //to find the shortest path between two nodes
    int minimumDistance(int[], bool[]);

    void printSolution(vector<string>tP1,vector<string>tP2);

    bool checkIfCloseToEnemy(int);

    string returnThePrev(vector<string>tP1,vector<string>tP2,string x);

};

bool Graph::checkIfCloseToEnemy(int x){
    vector<int>temp;
    //find all E's
    for(int i = 0; i < difPlaces; i++){
        if(places[i][0] == 'E')
            temp.push_back(i);
        int var = temp.size();
        for(int i = 0; i < var; i++){
        if(graph[x][temp[i]] < 5 && graph[x][temp[i]] != 0){
            return false;
        }
    }
}
return true;
}

// To find if we already put the place in the vector
int Graph::checkIfThePlaceExist(string placeName){
    int var = places.size();
    for(int i=0; i < var; i++){
        if(places[i] == placeName)
            return i;
    }
    return -1;
}

string Graph::returnThePrev(vector<string>tP1,vector<string>tP2,string x){
    for(int i = tP2.size()-1; i >= 0 ; i--){
        if(tP2[i] == x) 
            return tP1[i];
    }
    return " ";
}

void Graph::printSolution(vector<string>tP1,vector<string>tP2)
{
    vector<string>temp;
    string x = "#";
    while(1){
        if(x == "#"){
            temp.push_back("Mo");
            x = returnThePrev(tP1,tP2,"Mo");
        }
        else 
            x = returnThePrev(tP1,tP2,x);
        temp.push_back(x);
        if(x == "Ma"){
            for(int i = temp.size()-1; i >= 0 ; i--){
                cout << temp[i] << " ";
            }
            return;
        }
    }
}

// Dijktra alforithm for paths
void Graph::dijkstrasFunction(int src, int fin){


    //holds the final path
    vector<string>tP1;
    vector<string>tP2;

    int distance[difPlaces]; // distance keeps the shortest path
    bool nodeSet[difPlaces]; // checks if the node is added to the path

    for (int i = 0; i < difPlaces; i++){
        distance[i] = INT_MAX, nodeSet[i] = false;
    }

    distance[src] = 0;
    int fortheresult;
    for(int i = 0; i < difPlaces - 1; i++){
        int temp = minimumDistance(distance, nodeSet);

        nodeSet[temp] = true;
        for (int j = 0; j < difPlaces; j++){
            if (!nodeSet[j] 
            && graph[temp][j] 
            && distance[temp] != INT_MAX
            && distance[temp] + graph[temp][j] < distance[j]
            && places[j][0] != 'E'
            && places[temp][0] != 'E'
            && checkIfCloseToEnemy(j) == true
            && checkIfCloseToEnemy(temp) == true)
            {
                distance[j] = distance[temp] + graph[temp][j];
                tP1.push_back(places[temp]);
                tP2.push_back(places[j]);
            }
        fortheresult = j;
        }
        if(temp == fin)
            break;
    }
    printSolution(tP1,tP2);
    cout << distance[fortheresult] << endl; // maybe here there can be a mistake
}

int Graph::minimumDistance(int distance[], bool nodeSet[])
{
    // Initialize min value
    int min = INT_MAX, min_index;
  
    for (int v = 0; v < difPlaces; v++){
        if (nodeSet[v] == false && distance[v] <= min){
            min = distance[v], min_index = v;
        }
    }
    return min_index;
}
 
int main(int argc, char **argv){    
    string fname;
    cin >> fname;

    ifstream myFile(fname);
    
	if (!myFile.is_open()) {
		cerr << "FILE COULD NOT BE FOUND" << endl; 
		exit(1);
	}

    Graph g;

    //These values will be used to keep every row input in a map 
    string first,second,third;

    //This the input storing part
    for (int i = 0 ; !myFile.eof() ; i++) { 
        getline( myFile, first, ',' );
        getline( myFile, second, ',' );
        getline( myFile, third, '\n' );
        g.distanceP.push_back(stoi(third));
        g.inFilePlaces[i][0] = first;
        g.inFilePlaces[i][1] = second;
    }

    int src, fin;
    // In below code I find how many visit places there are
    int mapSize = g.inFilePlaces.size();
    for(int i = 0; i < mapSize; i++)
    {
        for (int j = 0; j < 2; j++){
            if(g.checkIfThePlaceExist(g.inFilePlaces[i][j]) == -1){
                g.places.push_back(g.inFilePlaces[i][j]);
                if(g.inFilePlaces[i][j] == "Ma")
                    src = g.places.size() - 1;
                else if(g.inFilePlaces[i][j] == "Mo")
                    fin = g.places.size() - 1;
            }
        }
    }

    //size of the different places 
    g.difPlaces = g.places.size();
    //I created the graph and assign 0 to each row at first
    g.graph = new int*[g.difPlaces];
    for(int i = 0; i < g.difPlaces; i++){
        g.graph[i] = new int[g.difPlaces];
        for(int j = 0; j < g.difPlaces; j++)
            g.graph[i][j] = 0;
    }

    // here i created the graph 
    for(int i = 0; i < mapSize; i++){
        int x = g.checkIfThePlaceExist(g.inFilePlaces[i][0]);
        int y = g.checkIfThePlaceExist(g.inFilePlaces[i][1]);
        int z = g.distanceP[i];
        g.graph[x][y] = z;
        g.graph[y][x] = z;
    }

    g.dijkstrasFunction(src, fin);
    myFile.close();
    return 0;
}

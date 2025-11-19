#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <stack>
#include <climits>
using namespace std;

// Function Declarations
void fileWriter(vector<vector<int>> &adjMat, vector<vector<int>> &weightMat, vector<string> &inputGraphVertices, vector<string> &inputWeightsGraphVertices, string bfsOutput, string dfsOutput, string dijkstraOutput);
string BFS(vector<vector<int>> &adjMat, int startVertex);
string DFS(vector<vector<int>> &adjMat, int startVertex);
string dijkstra(vector<vector<int>> &weightMat, int startVertex);
int minDistanceCalculator(vector<int> &distance, vector<bool> &seenList, int infinity);

int main() {

    // Variable for the file names
    const string fileName1 = "input.txt";
    const string fileName2 = "input_weights.txt";

    // Initializing the reading of the files
    ifstream inputGraphVertices(fileName1);
    ifstream inputWeightsGraphVertices(fileName2);

    // Starting vertex
    int startVertex = 'a' - 'a';

    // Grab the number of vertces for both files, and creat the variable line
    int inputGraphNumVertices = 0;
    int weightsGraphNumVertices = 0;
    string line;

    // Will increase the counter for the input.txt, and input_weights.txt file to get the number of vertices
    while (getline(inputGraphVertices, line)) inputGraphNumVertices++;
    while (getline(inputWeightsGraphVertices, line)) weightsGraphNumVertices++;

    // Vectors that are used for later
    vector<string> inputGraphVerticeChars(inputGraphNumVertices);
    vector<string> inputWeightsGraphVerticeChars(weightsGraphNumVertices);

    // Close the scanners
    inputGraphVertices.close();
    inputWeightsGraphVertices.close();

    // Creating the adacancy matrixes
    vector<vector<int>> adjMat(inputGraphNumVertices, vector<int>(inputGraphNumVertices, 0));
    vector<vector<int>> weightMat(weightsGraphNumVertices, vector<int>(weightsGraphNumVertices, 0));

    // Reads the files and creating two counters
    ifstream inputGraph(fileName1);
    ifstream inputWeightsGraph(fileName2);
    int counter1 = 0, counter2 = 0;

    // Add the values of the input graph txt file to the corresponding adjacency matrix
    while (getline(inputGraph, line)) {

        stringstream ss(line);
        string vertex;
        string neighbors;
        getline(ss, vertex, ':');
        inputGraphVerticeChars[counter1] = vertex;
        int startingIndex = vertex[0] - 'a';

        if (getline(ss, neighbors)) {
            stringstream ns(neighbors);
            string neighbor;
            while (getline(ns, neighbor, ',')) {
                int neighborIndex = neighbor[0] - 'a';
                adjMat[startingIndex][neighborIndex] = 1;
            }
        }
        counter1++;
    }

    // Add the values of the weighted txt file to th corresponding adjacency matrix
    while (getline(inputWeightsGraph, line)) {

        stringstream ss(line);
        string vertex;
        string neighbors;
        getline(ss, vertex, ':');
        inputWeightsGraphVerticeChars[counter2] = vertex;
        int startingIndex = vertex[0] - 'a';

        
        if (getline(ss, neighbors)) {

            stringstream ns(neighbors);
            string neighbor;

            while (getline(ns, neighbor, ',')) {
                stringstream parts(neighbor);
                string neighborChar, weightStr;
                getline(parts, neighborChar, '_');
                getline(parts, weightStr);
                int weight = stoi(weightStr);
                int neighborIndex = neighborChar[0] - 'a';
                weightMat[startingIndex][neighborIndex] = weight;
            }
        }

        counter2++;
    }

    // Closes the scanners
    inputGraph.close();
    inputWeightsGraph.close();

    // Run the BFS algorithm on the input graph
    string bfsOutput = BFS(adjMat, startVertex);

    // Run the DFS algorithm on the input graph
    string dfsOutput = DFS(adjMat, startVertex);

    // Run the Dijkstra algorithm  on the input weights graph
    string dijkstraOutput = dijkstra(weightMat, startVertex);

    // Creating the output file
    fileWriter(adjMat, weightMat, inputGraphVerticeChars, inputWeightsGraphVerticeChars, bfsOutput, dfsOutput, dijkstraOutput);

    return 0;
}

// Function Definitions

string BFS(vector<vector<int>> &adjMat, int startVertex) {

    // Variable for the result and initializing the queue
    string result;
    queue<int> queue;

    //Create a vector for the visited vetices
    vector<bool> seenList(adjMat.size(), false);

    // Sets the initial vertex to have been seen, and push it to the queue
    seenList[startVertex] = true;
    queue.push(startVertex);

    // Start of the search
    while (!queue.empty()) {

        // Sets the current vertex to the front of the queue, and then pop it, and add to the final string
        int curVert = queue.front();
        queue.pop();
        result += (char)(curVert + 'a');

        // Grab the neighbors of the current vertex
        for (int neighbor = 0; neighbor < adjMat.size(); neighbor++) {

            // Checks if the neighbor is adjacant to the current vertex, and no in the seen list
            if (adjMat[curVert][neighbor] == 1 && !seenList[neighbor]) {

                // Set the neighor as seen
                seenList[neighbor] = true;

                // Add the neighbor to the queue
                queue.push(neighbor);
            }
        }
    }

    // Return of the final result
    return "BFS: " + result;
}

// DFS algorithm
string DFS(vector<vector<int>> &adjMat, int startVertex) {

    // Variable for the result, and initializing the stack
    string result;
    stack<int> stack;

    // Create a vector for visited vertices
    vector<bool> seenList(adjMat.size(), false);

    // Sets the initial vertex to have been seen and add it to the stack
    seenList[startVertex] = true;
    stack.push(startVertex);

    // While loop for searching
    while (!stack.empty()) {

        // Sets the current vertex to the top of the stack, and then pop it, and add to the final string
        int curVert = stack.top();
        stack.pop();
        result += char(curVert + 'a');

        // Grab the neighbors of the current vertex
        for (int neighbor = 0; neighbor < adjMat.size(); neighbor++) {

            // Checks if the neighbor is adjacet to the current vertex, and it has not been seen
            if (adjMat[curVert][neighbor] == 1 && !seenList[neighbor]) {

                // Set the neighbor to seen
                seenList[neighbor] = true;

                // Push the neighbors to the stack
                stack.push(neighbor);
            }
        }
    }

    // Returns the final result
    return "DFS: " + result;
}

// Dijkstra algorithm
string dijkstra(vector<vector<int>> &weightMat, int startVertex) {
    // Variable for the final string, and infinity
    string result;
    int infinity = INT_MAX;

    // Distance and seen list
    vector<int> distance(weightMat.size(), infinity);
    vector<bool> seenList(weightMat.size(), false);

    // Sets the distance of the start vertx to 0
    distance[startVertex] = 0;

    // Dijkstra's Algorithm
    for (int i = 0; i < weightMat.size() - 1; i++) {

        // Finds which vertex had the smallest distance andset it to true in the seen list
        int minDistance = minDistanceCalculator(distance, seenList, infinity);
        seenList[minDistance] = true;

        // Finds the neighbors of the current vertex
        for (int j = 0; j < weightMat.size(); j++) {

            // Checks if the neighbor is not visited, there is an edge, and the new distance is shorter
            if (!seenList[j] && weightMat[minDistance][j] && distance[minDistance] != infinity && distance[minDistance] + weightMat[minDistance][j] < distance[j]) {
                distance[j] = distance[minDistance] + weightMat[minDistance][j];
            }
        }
    }

    // Used to display the output in the format
    for (int i = 0; i < weightMat.size(); i++) {
        result += (char)(i + 'a');
        result += ":" + to_string(distance[i]) + ",";
    }

    // Return the final result
    return "Dijkstra’s: " + result;
}

// Helper function
int minDistanceCalculator(vector<int> &distance, vector<bool> &seenList, int infinity) {
    int minDistance = infinity;
    int minDistanceVertex = -1;

    // Loops through all of the vertices
    for (int i = 0; i < distance.size(); i++) {

        // Checks if he vertex has not een seen, and the distance is smaller than the in distance
        if (!seenList[i] && distance[i] <= minDistance) {

            // Update the min distance, and the vertex
            minDistance = distance[i];
            minDistanceVertex = i;
        }
    }

    // Returns the minimum vertex
    return minDistanceVertex;
}

// Function for the output file
void fileWriter(vector<vector<int>> &adjMat, vector<vector<int>> &weightMat, vector<string> &inputGraphVertices, vector<string> &inputWeightsGraphVertices, string bfsOutput, string dfsOutput, string dijkstraOutput) {
    
    // Setting up the output.txt file
    ofstream outputFile("output.txt");

    // Creating header
    outputFile << "Adjacency Matrix (undirected):\n";
    outputFile << "  ";

    // Will automaticall move through and grb the vertices from the vector and then display them
    for(const auto &vertex : inputGraphVertices) {
        outputFile << vertex;
    }

    outputFile << "\n";

    // Displaying the total matrix
    for(int i = 0; i < adjMat.size(); i++) {
        outputFile << inputGraphVertices[i] << " ";

        // Displaying the matrix itself
        for(int value : adjMat[i]) {
            outputFile << value;
        }
        outputFile << "\n";
    }

    // Displaying the BFS and the DFS algorithm
    outputFile << "\n";
    outputFile << "BFS: " << bfsOutput << endl;
    outputFile << "DFS: " << dfsOutput << endl;

    // Dijkstra title
    outputFile << "\nAdjacency Matrix (directed w/weights):\n";
    outputFile << "  ";

    // Will automatically move through and grab the vertices from the vector and then display them
    for(const auto &vertex : inputGraphVertices) {
        outputFile << vertex << " ";
    }
    outputFile << "\n";

    // Displaying the total matrix
    for(int i = 0; i <inputWeightsGraphVertices.size(); i++) {
        outputFile << inputWeightsGraphVertices[i] << " ";

        // Displaying the matrix itself
        for(int value : weightMat[i]) {
            outputFile << value;
        }
        outputFile << "\n";
    }

    // Displaying the Dijkstra output
    outputFile << "\n";
    outputFile << "Dijkstra’s: " << dijkstraOutput << endl;

    // Clos the output file 
    outputFile.close();
}
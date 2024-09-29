#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <unistd.h> // For sleep()

#define MAX_NODES 5
#define MIN_NODES 2
#define MAX_DISTANCE 100
#define TIME_LIMIT 60 // Time limit in seconds
#define ANALYZE_TIME 10 // Time to analyze matrix in seconds
#define CONNECTED_PROBABILITY 70 // Probability (in percentage) of connecting two nodes

// Function prototypes
void printMatrix(int dist[][MAX_NODES], int num_nodes);
void dijkstra(int graph[][MAX_NODES], int num_nodes, int src, int dist[]);
void game();
void playAgain();

// Function to print the distance matrix
void printMatrix(int dist[][MAX_NODES], int num_nodes) {
    printf("Distance Matrix (Nodes 1 to %d):\n", num_nodes);
    printf("    ");
    for (int i = 0; i < num_nodes; i++) {
        printf(" %3d ", i + 1); // Label columns with node numbers starting from 1
    }
    printf("\n");

    for (int i = 0; i < num_nodes; i++) {
        printf("%3d ", i + 1); // Label rows with node numbers starting from 1
        for (int j = 0; j < num_nodes; j++) {
            if (dist[i][j] == -1)
                printf("  X  ");  // Print 'X' for no direct connection
            else
                printf(" %3d ", dist[i][j]);  // Print the distance
        }
        printf("\n");
    }
}

// Function to implement Dijkstra's algorithm
void dijkstra(int graph[][MAX_NODES], int num_nodes, int src, int dist[]) {
    int visited[MAX_NODES] = {0};

    // Initialize distances to a large value (considered infinity), except for the source node
    for (int i = 0; i < num_nodes; i++)
        dist[i] = -1;  // Initialize all distances as unreachable (-1)

    dist[src] = 0;  // Distance from source to itself is always 0

    for (int count = 0; count < num_nodes - 1; count++) {
        // Find the node with the minimum distance from the set of unvisited nodes
        int min = -1, min_index = -1;

        for (int v = 0; v < num_nodes; v++) {
            if (!visited[v] && dist[v] != -1 && (min == -1 || dist[v] < min)) {
                min = dist[v];
                min_index = v;
            }
        }

        if (min_index == -1)
            break;  // All remaining nodes are unreachable

        visited[min_index] = 1;

        // Update the distance for each adjacent node of the selected node
        for (int v = 0; v < num_nodes; v++) {
            if (!visited[v] && graph[min_index][v] != -1 && dist[min_index] != -1 &&
                (dist[v] == -1 || dist[min_index] + graph[min_index][v] < dist[v])) {
                dist[v] = dist[min_index] + graph[min_index][v];
            }
        }
    }
}

// Function to handle the game logic
void game() {
    int num_nodes;

    // Prompt user for number of nodes
    do {
        printf("Enter the number of nodes (between %d and %d): ", MIN_NODES, MAX_NODES);
        scanf("%d", &num_nodes);
        if (num_nodes < MIN_NODES || num_nodes > MAX_NODES) {
            printf("Invalid input. Please enter a number between %d and %d.\n", MIN_NODES, MAX_NODES);
        }
    } while (num_nodes < MIN_NODES || num_nodes > MAX_NODES);

    int graph[MAX_NODES][MAX_NODES];

    // Generate a distance matrix with random connections (ensuring no multiple links)
    for (int i = 0; i < num_nodes; i++) {
        for (int j = i + 1; j < num_nodes; j++) {  // j = i + 1 ensures one link between node pairs
            if (rand() % 100 < CONNECTED_PROBABILITY) {  // Randomly decide to connect nodes
                int distance = rand() % MAX_DISTANCE + 1;  // Random distance between 1 and MAX_DISTANCE
                graph[i][j] = distance;
                graph[j][i] = distance;  // Symmetric for undirected graph
            } else {
                graph[i][j] = -1;  // No direct connection between i and j
                graph[j][i] = -1;  // No direct connection between j and i
            }
        }
        graph[i][i] = 0;  // Distance from node to itself is always 0
    }

    // Print the matrix for analysis
    printMatrix(graph, num_nodes);

    // Pause for analysis (ANALYZE_TIME seconds)
    printf("You have %d seconds to analyze the matrix...\n", ANALYZE_TIME);
    sleep(ANALYZE_TIME);  // Pause for ANALYZE_TIME seconds

    // Variables for game
    int src, dest;
    int dist[MAX_NODES];
    time_t start_time, end_time;
    double elapsed_time;

    // Automatically select random source and destination nodes
    src = rand() % num_nodes;
    dest = rand() % num_nodes;
    while (src == dest) {
        dest = rand() % num_nodes; // Ensure the nodes are different
    }

    // Start timer
    start_time = time(NULL);

    // Calculate shortest path from source to all nodes
    dijkstra(graph, num_nodes, src, dist);

    // Automatically frame the shortest path question
    printf("Find the shortest distance from node %d to node %d.\n", src + 1, dest + 1);
    printf("You have 1 minute to answer. Type your answer below:\n");

    // Get user input for shortest distance
    int user_answer;
    scanf("%d", &user_answer);

    // End timer
    end_time = time(NULL);
    elapsed_time = difftime(end_time, start_time);

    // Check if the time exceeded
    if (elapsed_time > TIME_LIMIT) {
        printf("Time's up! You exceeded the 1-minute limit.\n");
        playAgain();
        return;
    }

    // Check user's answer and respond accordingly
    if (dist[dest] == -1) {
        printf("The shortest distance from node %d to node %d is unreachable through any path.\n", src + 1, dest + 1);
    } else if (user_answer == dist[dest]) {
        printf("Congratulations! You got it right! The shortest distance from node %d to node %d is indeed %d.\n", src + 1, dest + 1, dist[dest]);
    } else {
        printf("Oops! The correct shortest distance from node %d to node %d is %d, but you entered %d. Don't worry, you'll get it next time!\n", src + 1, dest + 1, dist[dest], user_answer);
    }

    playAgain();
}


// Function to ask the user if they want to play again
void playAgain() {
    char choice;
    printf("\nWould you like to play again? (y/n): ");
    scanf(" %c", &choice); // Space before %c to handle newline character
    if (choice == 'y' || choice == 'Y') {
        game(); // Start the game again
    } else {
        printf("Thanks for playing! See you next time!\n");
    }
}

// Main function to initiate the game
int main() {
    srand(time(NULL));
    game();
    return 0;
}

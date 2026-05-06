#pragma once

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

enum Colour { WHITE, GREY, BLACK };

class Queue {
    int arr[50];
    int front, rear;
public:
    Queue() {
        front = -1;
        rear = -1;
    }

    bool empty() {
        return (front == -1);
    }

    void enqueue(int x) {
        if (rear == 49) {
            return;
        }
        if (front == -1) {
            front = 0;
        }
        arr[++rear] = x;
    }

    int dequeue() {
        int x = arr[front];
        if (front == rear) {
            front = rear = -1;
        }
        else {
            front++;
        }
        return x;
    }
};

class Airport {
public:
    int id;
    string name;
    string city;
    string country;
    bool emergencyAvailable;

    Airport() : id(-1), name(""), city(""), country(""), emergencyAvailable(false) {}

    Airport(int i, string n, string c, string co, bool emergency) {
        id = i;
        name = n;
        city = c;
        country = co;
        emergencyAvailable = emergency;
    }

    void display() const {
        cout << "ID: " << id << " | " << name << " | " << city << ", " << country;
        if (emergencyAvailable) {
            cout << " | Emergency: Available";
        }
        cout << endl;
    }
};

class Graph {
private:
    int size;
    int adj[50][50];
    Colour visited[50];
    int distance[50];
    int parent[50];
    Airport airports[50];

public:
    Graph(int s = 0) {
        size = s;
        for (int i = 0; i < 50; i++) {
            for (int j = 0; j < 50; j++) {
                adj[i][j] = 0;
            }
            visited[i] = WHITE;
            distance[i] = -1;
            parent[i] = -1;
        }
    }

    void setSize(int s) {
        size = s;
    }

    void addAirport(int index, Airport airport) {
        if (index >= 0 && index < size) {
            airports[index] = airport;
        }
    }

    void addEdge(int u, int v) {
        if (u >= 0 && u < size && v >= 0 && v < size) {
            adj[u][v] = 1;
        }
    }

    void BFS(int s) {
        for (int i = 0; i < size; i++) {
            visited[i] = WHITE;
            distance[i] = -1;
            parent[i] = -1;
        }

        Queue Q;
        visited[s] = GREY;
        distance[s] = 0;
        Q.enqueue(s);

        while (!Q.empty()) {
            int u = Q.dequeue();

            for (int v = 0; v < size; v++) {
                if (adj[u][v] == 1 && visited[v] == WHITE) {
                    visited[v] = GREY;
                    distance[v] = distance[u] + 1;
                    parent[v] = u;
                    Q.enqueue(v);
                }
            }
            visited[u] = BLACK;
        }
    }

    void displayBFSTable() {
        cout << "\n===== BFS Traversal Table =====\n";
        cout.width(30); cout << left << "Airport";
        cout.width(10); cout << left << "Distance";
        cout.width(30); cout << left << "Parent" << endl;
        cout << string(50, '-') << endl;

        for (int i = 0; i < size; i++) {
            cout.width(30); cout << left << airports[i].name;
            cout.width(10); cout << left << (distance[i] == -1 ? "INF" : to_string(distance[i]));
            if (parent[i] == -1) {
                cout.width(30); 
                cout << left << "None";
            }
            else {
                cout.width(30); cout << left << airports[parent[i]].name;
            }
            cout << endl;
        }
        cout << endl;
    }

    void printPath(int s, int v) {
        if (v == s) {
            cout << airports[s].name;
        }
        else if (parent[v] == -1) {
            cout << "No path exists";
        }
        else {
            printPath(s, parent[v]);
            cout << " -> " << airports[v].name;
        }
    }

    void planTrip(int source, int dest) {
        if (source < 0 || source >= size || dest < 0 || dest >= size) {
            cout << "Invalid airport indices!" << endl;
            return;
        }

        BFS(source);

        cout << "\n===== Trip Planning Result =====\n";
        cout << "Source: " << airports[source].name << " (" << airports[source].city << ")" << endl;
        cout << "Destination: " << airports[dest].name << " (" << airports[dest].city << ")" << endl;

        if (distance[dest] == -1) {
            cout << "\nNo route available from " << airports[source].name
                << " to " << airports[dest].name << endl;
        }
        else {
            cout << "\nMinimum stops: " << distance[dest] << endl;
            cout << "Route: ";
            printPath(source, dest);
            cout << endl;
        }
        cout << endl;
    }

    void displayAllAirports() {
        cout << "\n===== All Airports =====\n";
        for (int i = 0; i < size; i++) {
            cout << i << ". ";
            airports[i].display();
        }
        cout << endl;
    }

    void loadAirports(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Could not open " << filename << endl;
            return;
        }

        int count = 0;
        int id;
        string name, city, country;
        int emergency;

        while (file >> id) {
            file.ignore(); // ignore space after id
            getline(file, name, ',');
            getline(file, city, ',');
            getline(file, country, ',');
            file >> emergency;

            airports[count] = Airport(id, name, city, country, emergency == 1);
            count++;
        }
        size = count;
        file.close();
        cout << "Loaded " << count << " airports from " << filename << endl;
    }

    void loadRoutes(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Could not open " << filename << endl;
            return;
        }

        int from, to, count = 0;
        while (file >> from >> to) {
            addEdge(from, to);
            count++;
        }
        file.close();
        cout << "Loaded " << count << " routes from " << filename << endl;
    }

    int getSize() const {
        return size;
    }

    Airport getAirport(int index) const {
        if (index >= 0 && index < size) {
            return airports[index];
        }
        return Airport();
    }
};


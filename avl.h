#pragma once


#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class FlightItem {
private:
    int flightID;
    string destination;
    int departureTime;
    int duration;
    string status;

public:
    FlightItem() : flightID(0), destination(""), departureTime(0), duration(0), status("") {}

    FlightItem(int id, string dest, int dep, int dur, string stat) {
        flightID = id;
        destination = dest;
        departureTime = dep;
        duration = dur;
        status = stat;
    }

    int getFlightID() const { return flightID; }
    string getDestination() const { return destination; }
    int getDepartureTime() const { return departureTime; }
    int getDuration() const { return duration; }
    string getStatus() const { return status; }

    void displayTableRow(int rotations = -1) const {
        cout.width(10); cout << left << flightID;
        cout.width(15); cout << left << destination;
        cout.width(12); cout << left << (to_string(departureTime) + ":00");
        cout.width(10); cout << left << (to_string(duration) + "h");
        cout.width(12); cout << left << status;
        if (rotations >= 0) {
            cout.width(10); cout << left << rotations;
        }
        cout << endl;
    }
};

class AVLNode {
public:
    FlightItem key;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(FlightItem k) {
        key = k;
        left = nullptr;
        right = nullptr;
        height = 1;
    }
};

// AVL Visualization helper
void printAVL(AVLNode* root, int space = 0, int gap = 6) {
    if (!root) return;
    space += gap;

    printAVL(root->right, space);

    cout << endl;
    for (int i = gap; i < space; i++) cout << " ";
    cout << "(" << root->key.getFlightID() << ")";

    printAVL(root->left, space);
}

class AVL {
private:
    AVLNode* root;
    int totalRotations;

    int height(AVLNode* n) {
        return n ? n->height : 0;
    }

    int getBalance(AVLNode* n) {
        return n ? height(n->left) - height(n->right) : 0;
    }

    void updateHeight(AVLNode* n) {
        n->height = 1 + max(height(n->left), height(n->right));
    }

    AVLNode* rightRotate(AVLNode* y) {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;

        x->right = y;
        y->left = T2;

        updateHeight(y);
        updateHeight(x);

        totalRotations++;
        return x;
    }

    AVLNode* leftRotate(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;

        y->left = x;
        x->right = T2;

        updateHeight(x);
        updateHeight(y);

        totalRotations++;
        return y;
    }

    AVLNode* balance(AVLNode* node) {
        updateHeight(node);
        int bf = getBalance(node);

        // LL Case
        if (bf > 1 && getBalance(node->left) >= 0) {
            return rightRotate(node);
        }
        // LR Case
        if (bf > 1 && getBalance(node->left) < 0) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        // RR Case
        if (bf < -1 && getBalance(node->right) <= 0) {
            return leftRotate(node);
        }
        // RL Case
        if (bf < -1 && getBalance(node->right) > 0) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        return node;
    }

    AVLNode* insert(AVLNode* node, FlightItem k) {
        if (node == NULL) {
            return new AVLNode(k);
        }
        if (k.getFlightID() < node->key.getFlightID()) {
            node->left = insert(node->left, k);
        }
        else if (k.getFlightID() > node->key.getFlightID()) {
            node->right = insert(node->right, k);
        }
        else {
            return node; // Duplicate not allowed
        }
        return balance(node);
    }

    AVLNode* minVal(AVLNode* n) {
        while (n->left != nullptr) {
            n = n->left;
        }
        return n;
    }

    AVLNode* deleteNode(AVLNode* node, int key) {
        if (node == nullptr) {
            return node;
        }
        if (key < node->key.getFlightID()) {
            node->left = deleteNode(node->left, key);
        }
        else if (key > node->key.getFlightID()) {
            node->right = deleteNode(node->right, key);
        }
        else {
            if (node->left == nullptr || node->right == nullptr) {
                AVLNode* temp = (node->left != nullptr) ? node->left : node->right;
                if (temp == nullptr) {
                    temp = node;
                    node = nullptr;
                }
                else {
                    *node = *temp;
                }
                delete temp;
            }
            else {
                AVLNode* temp = minVal(node->right);
                node->key = temp->key;
                node->right = deleteNode(node->right, temp->key.getFlightID());
            }
        }
        if (node == nullptr) {
            return node;
        }
        return balance(node);
    }

    AVLNode* search(AVLNode* node, int key) {
        if (node == nullptr || node->key.getFlightID() == key) {
            return node;
        }
        if (key < node->key.getFlightID()) {
            return search(node->left, key);
        }
        else {
            return search(node->right, key);
        }
    }

    void inorder(AVLNode* node) {
        if (node == nullptr) {
            return;
        }
        inorder(node->left);
        node->key.displayTableRow();
        inorder(node->right);
    }

    void deleteTree(AVLNode* node) {
        if (node != nullptr) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }

public:
    AVL() {
        root = nullptr;
        totalRotations = 0;
    }

    ~AVL() {
        deleteTree(root);
    }

    void insert(FlightItem flight) {
        root = insert(root, flight);
    }

    void remove(int key) {
        root = deleteNode(root, key);
    }

    bool find(int key) {
        return search(root, key) != nullptr;
    }

    void displayInorder() {
        if (root == nullptr) {
            cout << "No flight records available." << endl;
            return;
        }
        cout << "\n===== AVL Tree - All Flights (Inorder Traversal) =====\n";
        cout.width(10); cout << left << "FlightID";
        cout.width(15); cout << left << "Destination";
        cout.width(12); cout << left << "Departure";
        cout.width(10); cout << left << "Duration";
        cout.width(12); cout << left << "Status" << endl;
        cout << string(60, '-') << endl;
        inorder(root);
        cout << endl;
    }

    int getRotationCount() const {
        return totalRotations;
    }

    void resetRotationCount() {
        totalRotations = 0;
    }

    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Note: " << filename << " not found. Starting with empty AVL tree." << endl;
            return;
        }

        int id, dep, dur;
        string dest, stat;
        int count = 0;

        while (file >> id >> dest >> dep >> dur >> stat) {
            FlightItem flight(id, dest, dep, dur, stat);
            insert(flight);
            count++;
        }
        file.close();
        cout << "Loaded " << count << " flights into AVL tree from " << filename << endl;
    }
    public:
        void displayTree() {
            if (!root) {
                cout << "AVL tree is empty.\n";
                return;
            }
            cout << "\n===== AVL Tree Visualization =====\n";
            printAVL(root);
            cout << endl;
        }

};

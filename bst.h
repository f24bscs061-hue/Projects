#pragma once
#include <iostream>
#include <fstream>
#include <string>
using namespace std;




class itemtype {
private:
    int flightID;
    string destination;
    int departureTime;  // 24-hour format (0-23)
    int duration;       // hours
    string status;

public:
    itemtype() : flightID(0), destination(""), departureTime(0), duration(0), status("") {}

    itemtype(int id, string dest, int dep, int dur, string stat) {
        flightID = id;
        destination = dest;
        departureTime = dep;
        duration = dur;
        status = stat;
    }

    enum CompareResult { LESS, EQUAL, GREATER };

    int getFlightID() const { return flightID; }
    string getDestination() const { return destination; }
    int getDepartureTime() const { return departureTime; }
    int getDuration() const { return duration; }
    string getStatus() const { return status; }

    void setFlightID(int id) { flightID = id; }
    void setDestination(string dest) { destination = dest; }
    void setDepartureTime(int dep) { departureTime = dep; }
    void setDuration(int dur) { duration = dur; }
    void setStatus(string stat) { status = stat; }

    CompareResult Compare(const int& otherID) const {
        if (flightID < otherID) return LESS;
        else if (flightID > otherID) return GREATER;
        else return EQUAL;
    }

    void display() const {
        cout << "------------------------------------------------------------------" << endl;
        cout << "Flight ID: " << flightID << endl;
        cout << "Destination: " << destination << endl;
        cout << "Departure (24-hour format): " << departureTime << endl;
        cout << "Duration: " << duration << " hrs" << endl;
        cout << "Status: " << status << endl;
        cout << "------------------------------------------------------------------" << endl;
    }

    void displayTableRow() const {
        cout.width(10); cout << left << flightID;
        cout.width(15); cout << left << destination;
        cout.width(12); cout << left << (to_string(departureTime) + ":00");
        cout.width(10); cout << left << (to_string(duration) + "h");
        cout.width(12); cout << left << status << endl;
    }
};

class nodetype {
public:
    nodetype* parent;
    nodetype* left;
    nodetype* right;
    itemtype key;

    nodetype() {
        parent = NULL;
        left = NULL;
        right = NULL;
    }

    nodetype(itemtype k) {
        key = k;
        parent = NULL;
        left = NULL;
        right = NULL;
    }
};

// BST Visualization helper
void printBST(nodetype* root, int space = 0, int gap = 6) {
    if (!root) return;
    space += gap;

    printBST(root->right, space);

    cout << endl;
    for (int i = gap; i < space; i++) cout << " ";
    cout << "(" << root->key.getFlightID() << ")";

    printBST(root->left, space);
}

class BST {
private:
    nodetype* root;

    void transplant(nodetype* u, nodetype* v) {
        if (u->parent == NULL) {
            root = v;
        }
        else if (u == u->parent->left) {
            u->parent->left = v;
        }
        else {
            u->parent->right = v;
        }
        if (v != NULL) {
            v->parent = u->parent;
        }
    }

    nodetype* minimum(nodetype* x) {
        while (x->left != NULL) {
            x = x->left;
        }
        return x;
    }

    void inorder(nodetype* x) {
        if (x != NULL) {
            inorder(x->left);
            x->key.display();
            inorder(x->right);
        }
    }

    void displaybydest(nodetype* x, string dest) {
        if (x != NULL) {
            displaybydest(x->left, dest);
            if (x->key.getDestination() == dest)
                x->key.display();
            displaybydest(x->right, dest);
        }
    }

    void displaybystat(nodetype* x, string stat) {
        if (x != NULL) {
            displaybystat(x->left, stat);
            if (x->key.getStatus() == stat)
                x->key.display();
            displaybystat(x->right, stat);
        }
    }

    void calcETA(int dep, int dur, string status) {
        float eta = dep + dur;
        if (eta >= 24) {
            eta -= 24;
            cout << "Expected time of arrival: " << eta << " (next day)" << endl;
        }
        else {
            cout << "Expected time of arrival: " << eta << endl;
        }

        string ETAstatus;
        if (status == "Departed" || status == "departed")
            ETAstatus = "On-Time";
        else if (status == "Landed" || status == "landed")
            ETAstatus = "Early";
        else
            ETAstatus = "Delayed";

        cout << "Punctuality Status: " << ETAstatus << endl;
    }

    void displaybyETA(nodetype* x) {
        if (x != NULL) {
            displaybyETA(x->left);
            calcETA(x->key.getDepartureTime(), x->key.getDuration(), x->key.getStatus());
            x->key.display();
            displaybyETA(x->right);
        }
    }

    nodetype* findLongest(nodetype* x, nodetype* currentLongest) {
        if (x == NULL) return currentLongest;
        if (x->key.getDuration() > currentLongest->key.getDuration())
            currentLongest = x;
        currentLongest = findLongest(x->left, currentLongest);
        currentLongest = findLongest(x->right, currentLongest);
        return currentLongest;
    }

    void saveToFile(nodetype* x, ofstream& file) {
        if (x != NULL) {
            saveToFile(x->left, file);
            file << x->key.getFlightID() << " "
                << x->key.getDestination() << " "
                << x->key.getDepartureTime() << " "
                << x->key.getDuration() << " "
                << x->key.getStatus() << endl;
            saveToFile(x->right, file);
        }
    }

    void deleteTree(nodetype* node) {
        if (node != NULL) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }

public:
    BST() { root = NULL; }

    ~BST() { deleteTree(root); }

    nodetype* getRoot() { return root; }

    bool Insert(nodetype* z) {
        nodetype* y = NULL;
        nodetype* x = root;
        while (x != NULL) {
            y = x;
            if (z->key.getFlightID() < x->key.getFlightID()) {
                x = x->left;
            }
            else if (z->key.getFlightID() == x->key.getFlightID()) {
                cout << "Duplicates not allowed" << endl;
                return false;
            }
            else {
                x = x->right;
            }
        }
        z->parent = y;
        if (y == NULL) {
            root = z;
        }
        else if (z->key.getFlightID() < y->key.getFlightID()) {
            y->left = z;
        }
        else {
            y->right = z;
        }
        return true;
    }

    void Delete(nodetype* z) {
        if (z->left == NULL) {
            transplant(z, z->right);
        }
        else if (z->right == NULL) {
            transplant(z, z->left);
        }
        else {
            nodetype* y = minimum(z->right);
            if (y->parent != z) {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
        }
    }

    nodetype* search(nodetype* x, int flightID) {
        if (x == NULL || flightID == x->key.getFlightID()) {
            return x;
        }
        if (flightID < x->key.getFlightID()) {
            return search(x->left, flightID);
        }
        else {
            return search(x->right, flightID);
        }
    }

    void display() {
        if (root == NULL) {
            cout << "No flight records available\n";
            return;
        }
        cout << "All flights in ascending order of Flight IDs:" << endl;
        inorder(root);
    }

    void displayByDestination(string dest) {
        if (root == NULL) {
            cout << "No flight records available.\n";
            return;
        }
        cout << "\nFlights to " << dest << ":\n";
        displaybydest(root, dest);
    }

    void displayByStatus(string stat) {
        if (root == NULL) {
            cout << "No flight records available\n";
            return;
        }
        cout << "\nFlights with status: " << stat << "\n";
        displaybystat(root, stat);
    }

    void displayETA() {
        if (root == NULL) {
            cout << "No flight records available to calculate ETA.\n";
            return;
        }
        cout << "\nFlight ETAs:\n";
        displaybyETA(root);
    }

    void displayLongestFlight() {
        if (root == NULL) {
            cout << "No flight records available.\n";
            return;
        }
        nodetype* longest = findLongest(root, root);
        cout << "\nLongest Flight:\n";
        longest->key.display();
    }
    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Note: " << filename << " not found. Starting with empty database." << endl;
            return;
        }

        int id, dep, dur;
        string dest, stat;
        int count = 0;

        while (file >> id >> dest >> dep >> dur >> stat) {
            itemtype flight(id, dest, dep, dur, stat);
            nodetype* node = new nodetype(flight);
            if (Insert(node)) {
                count++;
            }
        }
        file.close();
        cout << "Loaded " << count << " flights from " << filename << endl;
    }

    void saveToFile(const string& filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Could not save to " << filename << endl;
            return;
        }
        saveToFile(root, file);
        file.close();
        cout << "Flight data saved to " << filename << endl;
    }
    public:
        void displayTree() {
            if (!root) {
                cout << "BST is empty.\n";
                return;
            }
            cout << "\n===== BST Tree Visualization =====\n";
            printBST(root);
            cout << endl;
        }

};

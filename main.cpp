#include <iostream>
#include <chrono>
#include "bst.h"
#include "avl.h"
#include "bfs.h"

using namespace std;
using namespace std::chrono;

void updateFlightInfo(BST& bstTree, AVL& avlTree)
{
    if (bstTree.getRoot() == NULL) {
        cout << "No flight records available to update.\n";
        return;
    }

    int id;
    cout << "\nEnter Flight ID to update: ";
    cin >> id;

    nodetype* result = bstTree.search(bstTree.getRoot(), id);
    if (result == NULL) {
        cout << "Flight not found.\n";
        return;
    }

    cout << "\nCurrent flight details:\n";
    result->key.display();

    int choice;
    do {
        cout << "\nWhich detail do you want to update?\n";
        cout << "1. Flight ID\n";
        cout << "2. Destination\n";
        cout << "3. Departure Time\n";
        cout << "4. Duration\n";
        cout << "5. Status\n";
        cout << "6. Cancel / Exit Update\n";
        cout << "Enter your choice: ";
        cin >> choice;

        itemtype updated = result->key; // copy

        switch (choice)
        {
            //--------------- UPDATE FLIGHT ID ----------------
        case 1:
        {
            int newID;
            cout << "Enter new Flight ID: ";
            cin >> newID;

            if (bstTree.search(bstTree.getRoot(), newID) != NULL) {
                cout << "A flight with this ID already exists. Update cancelled.\n";
                break;
            }

            // UPDATE BST:
            updated.setFlightID(newID);
            bstTree.Delete(result);
            bstTree.Insert(new nodetype(updated));

            // UPDATE AVL:
            avlTree.remove(id);
            avlTree.insert(FlightItem(updated.getFlightID(),
                updated.getDestination(),
                updated.getDepartureTime(),
                updated.getDuration(),
                updated.getStatus()));

            cout << "\nFlight ID updated successfully.\n";
            updated.display();
            return;
        }

        //---------------- UPDATE DESTINATION ----------------
        case 2:
        {
            string newDest;
            cout << "Enter new Destination: ";
            cin >> newDest;

            result->key.setDestination(newDest);

            avlTree.remove(id);
            avlTree.insert(FlightItem(result->key.getFlightID(),
                newDest,
                result->key.getDepartureTime(),
                result->key.getDuration(),
                result->key.getStatus()));

            cout << "\nDestination updated successfully.\n";
            break;
        }

        //---------------- UPDATE DEPARTURE TIME ----------------
        case 3:
        {
            int newDep;
            do {
                cout << "Enter new Departure Time (0–23): ";
                cin >> newDep;
                if (newDep < 0 || newDep > 23)
                    cout << "Invalid time!\n";
            } while (newDep < 0 || newDep > 23);

            result->key.setDepartureTime(newDep);

            avlTree.remove(id);
            avlTree.insert(FlightItem(result->key.getFlightID(),
                result->key.getDestination(),
                newDep,
                result->key.getDuration(),
                result->key.getStatus()));

            cout << "\nDeparture time updated successfully.\n";
            break;
        }

        //---------------- UPDATE DURATION ----------------
        case 4:
        {
            int newDur;
            cout << "Enter new Duration (hours): ";
            cin >> newDur;

            result->key.setDuration(newDur);

            avlTree.remove(id);
            avlTree.insert(FlightItem(result->key.getFlightID(),
                result->key.getDestination(),
                result->key.getDepartureTime(),
                newDur,
                result->key.getStatus()));

            cout << "\nDuration updated successfully.\n";
            break;
        }

        //---------------- UPDATE STATUS ----------------
        case 5:
        {
            string newStatus;
            do {
                cout << "Enter new Status (Departed / Landed / Delayed): ";
                cin >> newStatus;

                if (newStatus != "Landed" && newStatus != "landed" &&
                    newStatus != "Departed" && newStatus != "departed" &&
                    newStatus != "Delayed" && newStatus != "delayed")
                {
                    cout << "Invalid input! Try again.\n";
                }

            } while (newStatus != "Landed" && newStatus != "landed" &&
                newStatus != "Departed" && newStatus != "departed" &&
                newStatus != "Delayed" && newStatus != "delayed");

            result->key.setStatus(newStatus);

            avlTree.remove(id);
            avlTree.insert(FlightItem(result->key.getFlightID(),
                result->key.getDestination(),
                result->key.getDepartureTime(),
                result->key.getDuration(),
                newStatus));

            cout << "\nStatus updated successfully.\n";
            break;
        }

        case 6:
            cout << "Update cancelled.\n";
            break;

        default:
            cout << "Invalid choice! Try again.\n";
        }

        if (choice >= 2 && choice <= 5)
        {
            cout << "\nUpdated flight details:\n";
            result->key.display();
        }

    } while (choice != 6);
}




void displayMainMenu() {
    cout << "\n------------------------------" << endl;
    cout << "  Flight Management System" << endl;
    cout << "------------------------------" << endl;
    cout << "1. Add New Flight " << endl;
    cout << "2. Search Flight (BST)" << endl;
    cout << "3. Delete Flight (BST)" << endl;
    cout << "4. Display All Flights (BST)" << endl;
    cout << "5. Display Flights (AVL) + Rotation Info" << endl;
    cout << "6. Compare Search Time (BST vs AVL)" << endl;
    cout << "7. Plan Trip from Airport A to B" << endl;
    cout << "8. Visualize BST Tree" << endl;
    cout << "9. Visualize AVL Tree" << endl;
    cout << "10. Display Flights by Destination " << endl;
    cout << "11. Display Flights by Status " << endl;
    cout << "12. Display ETA and Punctuality Status " << endl;
    cout << "13. Update Flight Information (BST + AVL)" << endl;
    cout << "14. Display Longest Flight " << endl;
    cout << "15. Exit" << endl;
    cout << "------------------------------" << endl;
    cout << "Enter your choice: ";
}

int main() {
    BST bstTree;
    AVL avlTree;
    Graph graph;

    cout << "\n========== Loading Data ==========\n";

    // Load flights into BST
    bstTree.loadFromFile("flights.txt");

    // Load flights into AVL
    avlTree.loadFromFile("flights.txt");

    // Load airports and routes
    graph.loadAirports("airports.txt");
    graph.loadRoutes("routes.txt");

    cout << "==================================\n";

    int choice;
    do {
        displayMainMenu();
        cin >> choice;

        switch (choice) {
        case 1: { // Add New Flight (BST)
            int id, dep, dur;
            string dest, stat;

            cout << "\n--- Add New Flight ---" << endl;
            cout << "Enter Flight ID: ";
            cin >> id;

            cout << "Enter Destination: ";
            cin >> dest;

            do {
                cout << "Enter Departure Time (in 24hr format, e.g 13 for 1 pm): ";
                cin >> dep;
                if (dep < 0 || dep > 23) {
                    cout << "Invalid input! Departure time must be between 0 and 24.\n";
                }
            } while (dep < 0 || dep > 23);

            cout << "Enter Duration (in hours, e.g. 2 for 2 hours): ";
            cin >> dur;

            do {
                cout << "Enter Status (Departed / Landed / Delayed): ";
                cin >> stat;
                if (stat != "Landed" && stat != "landed" && stat != "Departed" &&
                    stat != "departed" && stat != "Delayed" && stat != "delayed") {
                    cout << "Invalid input! Please enter only Departed, Landed, or Delayed.\n";
                }
            } while (stat != "Landed" && stat != "landed" && stat != "Departed" &&
                stat != "departed" && stat != "Delayed" && stat != "delayed");

            itemtype newFlight(id, dest, dep, dur, stat);
            nodetype* node = new nodetype(newFlight);

            if (bstTree.Insert(node)) {
                cout << "\nFlight record inserted successfully.\n";

                // Also add to AVL
                FlightItem avlFlight(id, dest, dep, dur, stat);
                avlTree.insert(avlFlight);
            }
            break;
        }

        case 2: { // Search Flight (BST)
            int id;
            cout << "\n--- Search Flight ---" << endl;
            cout << "Enter Flight ID to search: ";
            cin >> id;

            nodetype* result = bstTree.search(bstTree.getRoot(), id);
            if (result == NULL) {
                cout << "\nFlight not found.\n";
            }
            else {
                cout << "\nFlight found:\n";
                result->key.display();
            }
            break;
        }

        case 3: { // Delete Flight (BST)
            int id;
            cout << "\n--- Delete Flight ---" << endl;
            cout << "Enter Flight ID to delete: ";
            cin >> id;

            nodetype* z = bstTree.search(bstTree.getRoot(), id);
            if (z == NULL) {
                cout << "\nFlight not found." << endl;
            }
            else {
                bstTree.Delete(z);
                cout << "\nFlight deleted successfully." << endl;

                // Also delete from AVL
                avlTree.remove(id);
            }
            break;
        }

        case 4: { // Display All Flights (BST)
            bstTree.display();
            break;
        }

        case 5: { // Display Flights (AVL) + Rotation Info
            avlTree.displayInorder();
            cout << "Total Rotations Performed: " << avlTree.getRotationCount() << endl;
            break;
        }

        case 6: { // Compare Search Time (BST vs AVL)
            int id;
            cout << "\n--- Compare Search Time ---" << endl;
            cout << "Enter Flight ID to search: ";
            cin >> id;

            // BST Search
            auto start_bst = high_resolution_clock::now();
            nodetype* result_bst = bstTree.search(bstTree.getRoot(), id);
            auto end_bst = high_resolution_clock::now();
            auto duration_bst = duration_cast<nanoseconds>(end_bst - start_bst);

            // AVL Search
            auto start_avl = high_resolution_clock::now();
            bool result_avl = avlTree.find(id);
            auto end_avl = high_resolution_clock::now();
            auto duration_avl = duration_cast<nanoseconds>(end_avl - start_avl);

            cout << "\n===== Search Time Comparison =====\n";
            cout << "BST Search Time: " << duration_bst.count() << " nanoseconds";
            if (result_bst) cout << " (Found)";
            else cout << " (Not Found)";
            cout << endl;

            cout << "AVL Search Time: " << duration_avl.count() << " nanoseconds";
            if (result_avl) cout << " (Found)";
            else cout << " (Not Found)";
            cout << endl;

            if (duration_bst.count() > duration_avl.count()) {
                cout << "\nAVL is faster by " << (duration_bst.count() - duration_avl.count())
                    << " nanoseconds" << endl;
            }
            else {
                cout << "\nBST is faster by " << (duration_avl.count() - duration_bst.count())
                    << " nanoseconds" << endl;
            }
            break;
        }

        case 7: { // Plan Trip from Airport A to B
            graph.displayAllAirports();

            int source, dest;
            cout << "Enter source airport index: ";
            cin >> source;

            if (source >= 0 && source < graph.getSize()) {
                graph.BFS(source);
                graph.displayBFSTable();
            }
            else {
                cout << "Invalid airport index!" << endl;
            }

            cout << "Enter destination airport index: ";
            cin >> dest;
            if (dest >= 0 && dest < graph.getSize()) {

                graph.planTrip(source, dest);
            }
            else {
                cout << "Invalid airport index!" << endl;
            }
            break;
        }

        case 8: { 
            bstTree.displayTree();
            break;

        }

        case 9: { 
            avlTree.displayTree();
            break;

        }

        case 10: { // Display Flights by Destination (BST)
            string dest;
            cout << "\n--- Display Flights by Destination ---" << endl;
            cout << "Enter destination to display: ";
            cin >> dest;
            bstTree.displayByDestination(dest);
            break;
        }

        case 11: { // Display Flights by Status (BST)
            string stat;
            cout << "\n--- Display Flights by Status ---" << endl;
            cout << "Enter status to display (Departed / Landed / Delayed): ";
            cin >> stat;
            bstTree.displayByStatus(stat);
            break;
        }

        case 12: { // Display ETA and Punctuality Status (BST)
            bstTree.displayETA();
            break;
        }

        case 13: { // Update Flight Information (BST)
            updateFlightInfo(bstTree, avlTree);
            break;
        }

        case 14: { // Display Longest Flight (BST)
            bstTree.displayLongestFlight();
            break;
        }

        case 15: { // Exit
            cout << "\n--- Saving Data ---" << endl;
            bstTree.saveToFile("flights.txt");
            cout << "\nThank you for using Flight Management System!" << endl;
            cout << "Exiting..." << endl;
            break;
        }

        default:
            cout << "\nInvalid choice! Please try again.\n";
        }

    } while (choice != 15);

    return 0;
}

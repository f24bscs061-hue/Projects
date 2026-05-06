#include <iostream>
#include <set>
#include <algorithm>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <cmath> 

using namespace std;

// ------- Core Set Operations --------

// Reads elements for a set from standard input
set<int> readSet(string name)
{
    cout << "Enter elements of Set " << name << ": ";
    string line;
    // stringstream to parse integers
    getline(cin, line);
    stringstream ss(line);
    set<int> s;
    int x;
    while (ss >> x) s.insert(x);
    return s;
}

//Prints a set to the console
void printSet(string name, const set<int>& s)
{
    cout << name << " = { ";
    bool first = true;
    for (int x : s)
    {
        if (!first)
            cout << ", ";
        cout << x;
        first = false;
    }
    cout << " }\n";
}

//Calculates the Union of two sets
set<int> Union(const set<int>& A, const set<int>& B)
{
    set<int> R;
    set_union(A.begin(), A.end(), B.begin(), B.end(), inserter(R, R.begin()));
    return R;
}

//Calculates the Intersection of two sets
set<int> Intersect(const set<int>& A, const set<int>& B)
{
    set<int> R;
    set_intersection(A.begin(), A.end(), B.begin(), B.end(), inserter(R, R.begin()));
    return R;
}

//Calculates the Difference (A - B) of two sets
set<int> Difference(const set<int>& A, const set<int>& B)
{
    set<int> R;
    set_difference(A.begin(), A.end(), B.begin(), B.end(), inserter(R, R.begin()));
    return R; 
}

// Takes Complement of Set
set<int> complementOf(const set<int>& S, const set<int>& U)
{
    return Difference(U, S);
}


void testCommutativeUnion(const set<int>& A, const set<int>& B)
{
    set<int> R1 = Union(A, B);
    set<int> R2 = Union(B, A);
    printSet(" (A U B)", R1);
    printSet(" (B U A)", R1);
    cout << " Result: A U B " << (R1 == R2 ? "==" : "!=") << "B U A\n";
}

set<int>Complement(const set<int>& S, const set<int>& U)
{
    return Difference(U, S);
}

void testDeMorgansIntersection(const set<int>& A, const set<int>& B, const set<int>& U) {
    set<int> A_int_B = Intersect(A, B);

    // Left side: (A ∩ B)'
    set<int> R_LHS = Complement(A_int_B, U);
    printSet("  (A V B)'", R_LHS);

    // Right side: A' U B'
    set<int> A_comp = Complement(A, U);
    set<int> B_comp = Complement(B, U);
    set<int> R_RHS = Union(A_comp, B_comp);
    printSet("  A' U B'", R_RHS);

    cout << "  Result: (A V B)' " << (R_LHS == R_RHS ? "==" : "!=") << " A' U B'\n";
}

void testDistributiveUnion(const set<int>& A, const set<int>& B, const set<int>& C) {
    // Left side: A U (B ∩ C)
    set<int> B_int_C = Intersect(B, C);
    set<int> R_LHS = Union(A, B_int_C);
    printSet("  A U (B V C)", R_LHS);

    // Right side: (A U B) ∩ (A U C)
    set<int> A_union_B = Union(A, B);
    set<int> A_union_C = Union(A, C);
    set<int> R_RHS = Intersect(A_union_B, A_union_C);
    printSet("  (A U B) V (A U C)", R_RHS);

    cout << "  Result: A U (B V C) " << (R_LHS == R_RHS ? "==" : "!=") << " (A U B) V (A U C)\n";
}

// -------------- MENU FUNCTIONS --------------
void showBasicOperations(const set<int>& A, const set<int>& B)
{
    cout << "\n--- Basic Set Operations on A and B ---\n";
    printSet("A U B", Union(A, B));
    printSet("A V B", Intersect(A, B));
    printSet("A - B", Difference(A, B));
    printSet("B - A", Difference(B, A));
}

void showAdvancedOperations(const set<int>& A, const set<int>& B, const set<int>& C)
{
    cout << "\n--- Advanced Set Operations ---\n";
    set<int> ABC_union = Union(Union(A, B), C);
    set<int> ABC_intersect = Intersect(Intersect(A, B), C);

    printSet("A U B U C", ABC_union);
    printSet("A V B V C", ABC_intersect);
    printSet("A V (B U C)", Intersect(A, Union(B, C)));
    printSet("(A - B) U (B - A) [Symmetric Difference]", Union(Difference(A, B), Difference(B, A)));
}

void showSetLaws(const set<int>& A, const set<int>& B, const set<int>& C)
{
    cout << "\n--- Testing Set Laws ---\n";

    // Define Universal Set (U) as A U B U C for the context of these sets
    set<int> U = Union(Union(A, B), C);
    cout << "Note: Universal Set (U) for Complement is: ";
    printSet("U", U);

    cout << "\n1. Commutative Law (Union): A U B == B U A\n";
    testCommutativeUnion(A, B);

    cout << "\n2. De Morgan's Law (Intersection): (A V B)' == A' U B'\n";
    testDeMorgansIntersection(A, B, U);

    cout << "\n3. Distributive Law (Union over Intersect): A U (B V C) == (A U B) V (A U C)\n";
    testDistributiveUnion(A, B, C);
}

//Custom Operations
set<int> evaluate(string expr, const set<int>& A, const set<int>& B, const set<int>& C)
{
    // Fix: console converts ' to \ so normalize
    for (auto& ch : expr)
        if (ch == '\\')
            ch = '\'';

    auto getSet = [&](char x)->set<int> {
        if (x == 'A') return A;
        if (x == 'B') return B;
        if (x == 'C') return C;
        return {};
        };

    vector<set<int>> val;
    vector<char> op;

    auto apply = [&]() {
        char o = op.back(); op.pop_back();
        auto b = val.back(); val.pop_back();
        auto a = val.back(); val.pop_back();
        if (o == 'U') val.push_back(Union(a, b));
        if (o == 'V') val.push_back(Intersect(a, b));
        if (o == '-') val.push_back(Difference(a, b));
        };

    for (int i = 0; i < (int)expr.size(); i++)
    {
        if (expr[i] == ' ') continue;

        if (expr[i] == '(') op.push_back('(');

        else if (expr[i] == ')')
        {
            while (op.back() != '(') apply();
            op.pop_back();
        }

        else if (expr[i] == 'A' || expr[i] == 'B' || expr[i] == 'C')
        {
            set<int> tmp = getSet(expr[i]);

            if (i + 1 < expr.size() && expr[i + 1] == '\'')
            {
                tmp = complementOf(tmp, Union(Union(A, B), C));
                i++;
            }
            val.push_back(tmp);
        }

        else if (expr[i] == 'U' || expr[i] == 'V' || expr[i] == '-')
        {
            while (!op.empty() && op.back() != '(') apply();
            op.push_back(expr[i]);
        }
    }

    while (!op.empty()) apply();
    return val.back();
}


// Display Menu
void showMenu() 
{
    cout << "\n===================================\n";
    cout << "  SET THEORY INTERACTIVE MENU\n";
    cout << "===================================\n";
    cout << "1. Show Current Sets\n";
    cout << "2. Prove Set Laws\n";
    cout << "3. Basic Operations\n";
    cout << "4. Advanced Operations\n";
    cout << "5. Display Venn Diagram\n";
    cout << "6. Solve Custom Expression\n";
    cout << "7. Exit\n";

    cout << "Enter your choice: ";
}

// ----------- Visualization Helpers ---------------

//Converts a set to a string and centers/pads it to a minimum width.
//param s The set to format.
//param minWidth The target width for padding.
//return The padded and centered string representation of the set 

string padString(const set<int>& s, int minWidth)
{
    stringstream ss;
    // Concatenate elements with a space
    for (int x : s) {
        ss << x << " ";
    }
    string content = ss.str();
    if (!content.empty()) {
        content.pop_back(); // Remove trailing space
    }

    // Calculate padding needed to center the text
    int padding = max(0, minWidth - (int)content.length());
    int pad_left = padding / 2;
    int pad_right = padding - pad_left;

    // Return the centered string
    return string(pad_left, ' ') + content + string(pad_right, ' ');
}

// --- Main Visualization Function ---

/*Draws a text-based Venn diagram with set elements embedded.
 details This function calculates all 7 distinct regions
 and uses a fixed structure for a clear, readable diagram.*/
void venn3(set<int>A, set<int>B, set<int>C)
{
    // Define a stable width for all element display areas for visual alignment
    const int W_MAIN = 12; // Width for intersection and 'only' regions

    // Calculate all 7 distinct regions (R1-R7)
    set<int> AB_union = Union(A, B);
    set<int> AC_union = Union(A, C);
    set<int> BC_union = Union(B, C);
    set<int> ABC_union = Union(AB_union, C); // A U B U C

    set<int> A_int_B = Intersect(A, B);
    set<int> B_int_C = Intersect(B, C);
    set<int> A_int_C = Intersect(A, C);

    // R7: A ∩ B ∩ C
    set<int> ABC = Intersect(A_int_B, C);

    // R4: (A ∩ B) - C
    set<int> AB_only = Difference(A_int_B, C);
    // R5: (B ∩ C) - A
    set<int> BC_only = Difference(B_int_C, A);
    // R6: (A ∩ C) - B
    set<int> AC_only = Difference(A_int_C, B);

    // R1: A only (A - (B U C))
    set<int> Aonly = Difference(A, BC_union);
    // R2: B only (B - (A U C))
    set<int> Bonly = Difference(B, AC_union);
    // R3: C only (C - (A U B))
    set<int> Conly = Difference(C, AB_union);


    cout << "\n======================  VENN DIAGRAM =========================\n\n";
    cout << string(6, ' ') << "                     (U)\n\n";
    cout << string(6, ' ') << "  (" << string(W_MAIN / 2 - 1, ' ') << "A" << string(W_MAIN / 2 - 1, ' ') << ")            (" << string(W_MAIN / 2 - 1, ' ') << "B" << string(W_MAIN / 2 - 1, ' ') << ")\n";

    // Line 1: Top horizontal lines and A-only/B-only/AB-only
    cout << "     .------------------.   .--------------------.\n";
    cout << "    / " << padString(Aonly, W_MAIN) << " | " << padString(AB_only, W_MAIN) << " | " << padString(Bonly, W_MAIN) << "  \\\n";
    cout << "    |---------------------|-----------------------|\n";

    // Line 2: Intersections involving C
    // The '|' lines are staggered to imply the overlap with C below
    cout << "    |(" << padString(AC_only, W_MAIN) << ") (";
    cout << padString(ABC, W_MAIN) << ") (" << padString(BC_only, W_MAIN) << ") |\n";

    // Line 3: Vertical separators/structure
    cout << "    |         .-----------^----------.            |\n";

    // Line 4: C-only region
    cout << "    \\        /" << padString(Conly, W_MAIN) << "|\           \\           /\n";
    cout << "     `------'--------------------------'-----'---'\n";
    cout << "                   (" << string(W_MAIN / 2 - 1, ' ') << "C" << string(W_MAIN / 2 - 1, ' ') << ")\n\n";
    cout << "================================================================\n";
}

int main()
{
    // Setting up the environment
    cin.exceptions(std::ios::failbit);

    int choice = 0;
    bool sets_read = false;
    set<int> A, B, C;

    cout << " ==================== SET THEORY VISUALIZER ====================== \n";
    cout << " ===== KEY : U = UNION || V = INTERSECTION || \\ = COMPLIMENT ===== \n";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    A = readSet("A");
    B = readSet("B");
    C = readSet("C");
    sets_read = true;

    while (choice != 7)
    {
        showMenu();
        if (!(cin >> choice))
        {
            cout << "\n--- Invalid input. Please enter a number from the menu. ---\n";
            // Clear error flags and ignore the rest of the line
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = 0; // Reset choice to loop again
            continue;
        }

        switch (choice)
        {
        case 1:
        {
            cout << "\n========= CURRENT SETS =========\n";
            printSet("A", A);
            printSet("B", B);
            printSet("C", C);
            break;
        }
        case 2:
        {
            showSetLaws(A, B, C);
            break;
        }
        case 3:
        {
            showBasicOperations(A, B);
            break;
        }
        case 4:
        {
            showAdvancedOperations(A, B, C);
            break;
        }
        case 5:
        {
            venn3(A, B, C);
            break;
        }

        case 6:
        {
            cin.ignore();
            cout << "\nEnter expression (e.g. AUBUC, A-(BVC)) : ";
            string expr;
            getline(cin, expr);

            set<int> result = evaluate(expr, A, B, C);
            printSet("Result", result);
            break;
        }
        case 7:
        {
            cout << "\nProgram Finished.\n";
            break;
        }
        default:
        {
            cout << "\n--- Invalid choice. Please select a number from 1 to 7. ---\n";
            break;
        }
        }
    }
    return 0;
}
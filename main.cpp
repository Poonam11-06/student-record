/*
 * =====================================================================================
 *  Student Record Search System using Binary Search Tree (BST)
 *  
 *  Features:
 *   - Enrolment ID as unique BST key
 *   - Duplicate key detection and rejection
 *   - Search with comparison counting
 *   - Deletion (Leaf, One-Child, Two-Children with Inorder Successor)
 *   - Traversals: Inorder (Sorted), Preorder, Postorder
 *   - 10 Pre-loaded sample student records for testing
 *   - Comparison analysis: Balanced vs Degenerate (Sorted) BST
 *   - Time complexity and Self-Balancing BST explanation
 * =====================================================================================
 */

#include <iostream>
#include <string>
#include <iomanip>

using namespace std;


struct Student {
    int enrolmentId;        // Unique Key for BST
    string name;
    string department;
};

// BST Node Structure
struct Node {
    Student student;
    Node* left;
    Node* right;

    Node(int id, const string& studentName, const string& dept)
        : student{id, studentName, dept}, left(nullptr), right(nullptr) {}
};


Node* insertRecord(Node* root, int id, const string& name, const string& dept, bool& inserted) {
    if (root == nullptr) {
        inserted = true;
        return new Node(id, name, dept);
    }

    if (id < root->student.enrolmentId) {
        root->left = insertRecord(root->left, id, name, dept, inserted);
    } else if (id > root->student.enrolmentId) {
        root->right = insertRecord(root->right, id, name, dept, inserted);
    } else {
        // Duplicate key detected!
        cout << "\n[!] Insertion Rejected: Student with Enrolment ID " << id 
             << " already exists (" << root->student.name << ", " 
             << root->student.department << ").\n";
        inserted = false;
    }
    return root;
}

Node* searchRecord(Node* root, int id, int& comparisons) {
    if (root == nullptr) {
        return nullptr;
    }

    comparisons++;
    if (root->student.enrolmentId == id) {
        return root;
    }

    if (id < root->student.enrolmentId) {
        return searchRecord(root->left, id, comparisons);
    } else {
        return searchRecord(root->right, id, comparisons);
    }
}

// Helper to find minimum node in a subtree (Inorder Successor)
Node* findMin(Node* root) {
    while (root && root->left != nullptr) {
        root = root->left;
    }
    return root;
}

//  BST Deletion covering leaf, one-child, and two-children cases
Node* deleteRecord(Node* root, int id, bool& deleted) {
    if (root == nullptr) {
        return nullptr;
    }

    if (id < root->student.enrolmentId) {
        root->left = deleteRecord(root->left, id, deleted);
    } else if (id > root->student.enrolmentId) {
        root->right = deleteRecord(root->right, id, deleted);
    } else {
        // Node found!
        deleted = true;

        // Case 1: Leaf Node (No child)
        if (root->left == nullptr && root->right == nullptr) {
            delete root;
            return nullptr;
        }
        // Case 2A: One Child (Only right child)
        else if (root->left == nullptr) {
            Node* temp = root->right;
            delete root;
            return temp;
        }
        // Case 2B: One Child (Only left child)
        else if (root->right == nullptr) {
            Node* temp = root->left;
            delete root;
            return temp;
        }
        // Case 3: Two Children
        else {
            // Find inorder successor (minimum value in right subtree)
            Node* successor = findMin(root->right);
            // Copy successor data to this node
            root->student = successor->student;
            // Delete inorder successor from right subtree
            bool dummy = false;
            root->right = deleteRecord(root->right, successor->student.enrolmentId, dummy);
        }
    }
    return root;
}

//  Traversals
// Inorder Traversal: Left -> Root -> Right (Produces Sorted Enrolment IDs)
void inorderTraversal(Node* root) {
    if (root != nullptr) {
        inorderTraversal(root->left);
        cout << " | " << setw(14) << left << root->student.enrolmentId
             << " | " << setw(20) << left << root->student.name
             << " | " << setw(25) << left << root->student.department << " |\n";
        inorderTraversal(root->right);
    }
}

// Preorder Traversal: Root -> Left -> Right
void preorderTraversal(Node* root) {
    if (root != nullptr) {
        cout << root->student.enrolmentId << " (" << root->student.name << ") -> ";
        preorderTraversal(root->left);
        preorderTraversal(root->right);
    }
}

// Postorder Traversal: Left -> Right -> Root
void postorderTraversal(Node* root) {
    if (root != nullptr) {
        postorderTraversal(root->left);
        postorderTraversal(root->right);
        cout << root->student.enrolmentId << " (" << root->student.name << ") -> ";
    }
}

// Helper to print table header
void printTableHeader() {
    cout << " +----------------+----------------------+---------------------------+\n";
    cout << " | Enrolment ID   | Student Name         | Department / Programme    |\n";
    cout << " +----------------+----------------------+---------------------------+\n";
}

void printTableFooter() {
    cout << " +----------------+----------------------+---------------------------+\n";
}

// Free memory when exiting
void clearTree(Node* root) {
    if (root != nullptr) {
        clearTree(root->left);
        clearTree(root->right);
        delete root;
    }
}

//  Pre-load 10 Sample Student Records
void load10SampleRecords(Node*& root) {
    struct Sample {
        int id;
        string name;
        string dept;
    } samples[] = {
        {1050, "Alice Smith",     "Computer Science"},
        {1020, "Bob Johnson",      "Information Tech"},
        {1070, "Charlie Brown",    "Electronics & Comm"},
        {1010, "Diana Prince",     "Mechanical Eng"},
        {1035, "Ethan Hunt",       "Computer Science"},
        {1065, "Fiona Gallagher",  "Civil Eng"},
        {1090, "George Clark",     "Biotechnology"},
        {1005, "Hannah Abbott",    "Chemical Eng"},
        {1028, "Ian Malcolm",      "Mathematics & Data"},
        {1080, "Julia Roberts",    "Artificial Intelligence"}
    };

    int count = 0;
    for (const auto& s : samples) {
        bool inserted = false;
        root = insertRecord(root, s.id, s.name, s.dept, inserted);
        if (inserted) count++;
    }
    cout << "\n[✓] Successfully loaded " << count << " sample records into the BST!\n";
}

// Task 15: Demonstration of Balanced vs Sorted Insertion Order
void compareBalancedVsSortedBST() {
    cout << "\n========================================================================\n";
    cout << "    PERFORMANCE COMPARISON (Balanced vs Sorted Insertion Order)   \n";
    cout << "========================================================================\n";

    // 10 sample keys
    int sortedKeys[]   = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    int balancedKeys[] = {50, 20, 80, 10, 30, 60, 90, 40, 70, 100};

    Node* balancedTree = nullptr;
    Node* sortedTree = nullptr;

    bool dummy = false;
    for (int k : balancedKeys) {
        balancedTree = insertRecord(balancedTree, k, "Student", "Dept", dummy);
    }
    for (int k : sortedKeys) {
        sortedTree = insertRecord(sortedTree, k, "Student", "Dept", dummy);
    }

    cout << "\nInserted 10 keys into two BSTs:\n";
    cout << " - Balanced-order insertion: {50, 20, 80, 10, 30, 60, 90, 40, 70, 100}\n";
    cout << " - Sorted-order insertion:   {10, 20, 30, 40, 50, 60, 70, 80, 90, 100} (Right-skewed line)\n\n";

    int testKeys[] = {10, 50, 100, 75}; // 75 is not present
    cout << setw(12) << left << "Search Key" 
         << " | " << setw(25) << left << "Balanced Tree Comparisons" 
         << " | " << setw(25) << left << "Sorted Tree Comparisons" << "\n";
    cout << "-------------+---------------------------+---------------------------\n";

    for (int target : testKeys) {
        int compBalanced = 0;
        int compSorted = 0;
        searchRecord(balancedTree, target, compBalanced);
        searchRecord(sortedTree, target, compSorted);

        string targetStr = to_string(target) + (target == 75 ? " (missing)" : "");
        cout << setw(12) << left << targetStr
             << " | " << setw(25) << left << compBalanced
             << " | " << setw(25) << left << compSorted << "\n";
    }

    cout << "-------------------------------------------------------------------------\n";
    cout << "OBSERVATION:\n";
    cout << " 1. In a balanced BST, search takes O(log n) comparisons (max ~4 for n=10).\n";
    cout << " 2. In a sorted-insertion BST, the tree degenerates into a Linked List,\n";
    cout << "    requiring O(n) comparisons (e.g. key 100 takes 10 comparisons).\n";
    cout << "=========================================================================\n";

    clearTree(balancedTree);
    clearTree(sortedTree);
}

//  Explanation of Complexity & Self-Balancing Trees
void explainComplexityAndBalancing() {
    cout << "\n========================================================================\n";
    cout << "          COMPLEXITY ANALYSIS & SELF-BALANCING TREES             \n";
    cout << "========================================================================\n";
    cout << "1. TIME COMPLEXITY OF STANDARD BST:\n";
    cout << "   --------------------------------------------------------------------\n";
    cout << "   Operation | Average Case (Balanced) | Worst Case (Degenerate/Skewed)\n";
    cout << "   ----------+-------------------------+-------------------------------\n";
    cout << "   Search    |        O(log n)         |             O(n)              \n";
    cout << "   Insert    |        O(log n)         |             O(n)              \n";
    cout << "   Delete    |        O(log n)         |             O(n)              \n";
    cout << "   Traversal |          O(n)           |             O(n)              \n";
    cout << "   --------------------------------------------------------------------\n\n";

    cout << "2. WHY DOES A BST DEGRADE TO O(n)?\n";
    cout << "   - When keys are inserted in sorted (ascending or descending) order,\n";
    cout << "     every new node is added as the right (or left) child of the previous node.\n";
    cout << "   - The tree loses its branching structure and becomes a Skewed Tree\n";
    cout << "     (effectively a singly linked list).\n";
    cout << "   - Tree height degrades from h = log2(n) to h = n.\n\n";

    cout << "3. HOW DO SELF-BALANCING TREES SOLVE THIS?\n";
    cout << "   - Self-balancing trees (e.g., AVL Tree, Red-Black Tree) monitor the\n";
    cout << "     height or color balance of subtrees after every insertion & deletion.\n";
    cout << "   - If an imbalance is detected (e.g. balance factor > 1 or < -1 in AVL),\n";
    cout << "     they automatically perform TREE ROTATIONS (Left, Right, Left-Right, Right-Left).\n";
    cout << "   - This guarantees the tree height remains strictly bounded at O(log n),\n";
    cout << "     ensuring worst-case O(log n) time for search, insert, and delete.\n";
    cout << "========================================================================\n";
}

int main() {
    Node* root = nullptr;
    int choice;

    cout << "=======================================================\n";
    cout << "      STUDENT RECORD SEARCH SYSTEM (BST IMPLEMENTATION)\n";
    cout << "=======================================================\n";

    do {
        cout << "\n--------------------- MAIN MENU ---------------------\n";
        cout << " 1. Insert New Student Record\n";
        cout << " 2. Search Student by Enrolment ID\n";
        cout << " 3. Delete Student Record\n";
        cout << " 4. Display Sorted Records (Inorder Traversal)\n";
        cout << " 5. Display Preorder Traversal\n";
        cout << " 6. Display Postorder Traversal\n";
        cout << " 7. Load 10 Sample Student Records (Auto-populate)\n";
        cout << " 8. Compare Balanced vs Sorted BST Efficiency \n";
        cout << " 9. View Complexity Analysis & Self-Balancing Info \n";
        cout << " 0. Exit System\n";
        cout << "-----------------------------------------------------\n";
        cout << "Enter your choice (0-9): ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "\n[!] Invalid input. Please enter a valid number.\n";
            continue;
        }

        switch (choice) {
            case 1: { // Insert
                int id;
                string name, dept;
                cout << "\n--- Insert Student Record ---\n";
                cout << "Enter Enrolment ID (integer key): ";
                while (!(cin >> id)) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Invalid ID. Please enter an integer: ";
                }
                cin.ignore(); // flush newline
                cout << "Enter Student Name: ";
                getline(cin, name);
                cout << "Enter Programme / Department: ";
                getline(cin, dept);

                bool inserted = false;
                root = insertRecord(root, id, name, dept, inserted);
                if (inserted) {
                    cout << "[✓] Student record with ID " << id << " successfully inserted!\n";
                }
                break;
            }

            case 2: { // Search
                if (root == nullptr) {
                    cout << "\n[!] The BST is currently empty. Insert records first.\n";
                    break;
                }
                int id;
                cout << "\n--- Search Student Record ---\n";
                cout << "Enter Enrolment ID to search: ";
                while (!(cin >> id)) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Invalid ID. Please enter an integer: ";
                }

                int comparisons = 0;
                Node* found = searchRecord(root, id, comparisons);
                if (found) {
                    cout << "\n[✓] Student Record Found (in " << comparisons << " comparison" 
                         << (comparisons > 1 ? "s" : "") << "):\n";
                    cout << "    - Enrolment ID : " << found->student.enrolmentId << "\n";
                    cout << "    - Name         : " << found->student.name << "\n";
                    cout << "    - Department   : " << found->student.department << "\n";
                } else {
                    cout << "\n[x] Student with Enrolment ID " << id 
                         << " not found after " << comparisons << " comparisons.\n";
                }
                break;
            }

            case 3: { // Delete
                if (root == nullptr) {
                    cout << "\n[!] The BST is currently empty. Nothing to delete.\n";
                    break;
                }
                int id;
                cout << "\n--- Delete Student Record ---\n";
                cout << "Enter Enrolment ID to delete: ";
                while (!(cin >> id)) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Invalid ID. Please enter an integer: ";
                }

                bool deleted = false;
                root = deleteRecord(root, id, deleted);
                if (deleted) {
                    cout << "[✓] Student record with ID " << id << " successfully deleted.\n";
                } else {
                    cout << "[x] Student with ID " << id << " does not exist in the system.\n";
                }
                break;
            }

            case 4: { // Inorder (Sorted)
                if (root == nullptr) {
                    cout << "\n[!] The BST is currently empty.\n";
                    break;
                }
                cout << "\n--- Sorted Student Records (Inorder Traversal: Left -> Root -> Right) ---\n";
                printTableHeader();
                inorderTraversal(root);
                printTableFooter();
                break;
            }

            case 5: { // Preorder
                if (root == nullptr) {
                    cout << "\n[!] The BST is currently empty.\n";
                    break;
                }
                cout << "\n--- Preorder Traversal (Root -> Left -> Right) ---\n";
                preorderTraversal(root);
                cout << "END\n";
                break;
            }

            case 6: { // Postorder
                if (root == nullptr) {
                    cout << "\n[!] The BST is currently empty.\n";
                    break;
                }
                cout << "\n--- Postorder Traversal (Left -> Right -> Root) ---\n";
                postorderTraversal(root);
                cout << "END\n";
                break;
            }

            case 7: { // Load 10 Sample records
                load10SampleRecords(root);
                break;
            }

            case 8: { // Compare Balanced vs Sorted BST
                compareBalancedVsSortedBST();
                break;
            }

            case 9: { // Complexity and Self Balancing
                explainComplexityAndBalancing();
                break;
            }

            case 0: {
                cout << "\nThank you for using the Student Record Search System. Goodbye!\n";
                break;
            }

            default:
                cout << "\n[!] Invalid choice. Please select an option between 0 and 9.\n";
                break;
        }

    } while (choice != 0);

    clearTree(root);
    return 0;
}


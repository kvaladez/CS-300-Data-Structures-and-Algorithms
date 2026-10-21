/*
 * Name: Karina Valadez
 * Course: CS 300 - Data Structures and Algorithms
 * Project: Project Two
 * Description: ABCU Advising C++ Program that uses a Binary Search Tree to store, search, and display course information in alphanumeric order.
 */


#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits>
#include <cctype>
using namespace std;

//Define the structure to hold the course information
struct Course {

    //Store course number
	string courseNumber;

    //Store course name
	string courseName;

    //Store course prerequisites
	vector<string> prerequisites;

};

// Define a node for the Binary Search Tree
struct Node {

    // Store the course information
    Course course;

    // Point to the left and right nodes
    Node* left;
    Node* right;

    // Constructor to create a new node
    Node(Course newCourse) {
        course = newCourse;
        left = nullptr;
        right = nullptr;
    }
};

// Define the Binary Search Tree
struct BinarySearchTree {

    // Store the root of the tree
    Node* root = nullptr;

};

// Create an empty Binary Search Tree
BinarySearchTree courseTree;

// Keep track of whether course data has been loaded
bool dataLoaded = false;


// Insert a course into the Binary Search Tree
Node* insertCourse(Node* node, Course newCourse) {

    // If the position is empty, create a new node
    if (node == nullptr) {
        return new Node(newCourse);
    }

    // If the course number is smaller, go left
    if (newCourse.courseNumber < node->course.courseNumber) {
        node->left = insertCourse(node->left, newCourse);
    }
    else {
        // Otherwise, go right
        node->right = insertCourse(node->right, newCourse);
    }

    return node;
}

// Search the Binary Search Tree and return a course
Course* findCourse(BinarySearchTree& courses, string courseNumber) {

    // Start searching from the root
    Node* current = courses.root;

    // Keep searching while the current node is not empty
    while (current != nullptr) {

        // Check if the course number matches
        if (courseNumber == current->course.courseNumber) {
            return &current->course;
        }

        // If the course number is smaller, go left
        if (courseNumber < current->course.courseNumber) {
            current = current->left;
        }
        else {
            // Otherwise, go right
            current = current->right;
        }
    }

    // Return nullptr if the course was not found
    return nullptr;
}

//Search the BST for a course
void searchCourse(BinarySearchTree& courses, string courseNumber) {

    // Find the course in the tree
    Course* course = findCourse(courses, courseNumber);

    // Check if the course was found
    if (course != nullptr) {

        // Print the course number and name
        cout << course->courseNumber << ", " << course->courseName << endl;

        // Check if the course has prerequisites
        if (course->prerequisites.empty()) {
            cout << "Prerequisites: None" << endl;
        }
        else {
            cout << "Prerequisites:" << endl;

            // Go through each prerequisite
            for (string prerequisite : course->prerequisites) {

                // Find the prerequisite course information
                Course* prerequisiteCourse = findCourse(courses, prerequisite);

                // Print the prerequisite number and name
                if (prerequisiteCourse != nullptr) {
                    cout << prerequisiteCourse->courseNumber << ", "
                    << prerequisiteCourse->courseName << endl;
                }
            }
        }
    }
    else {
        cout << "Course not found" << endl;
    }
}


// Print all courses in alphanumeric order
void printCourseList(Node* node) {

    // Check if the node is not empty
    if (node != nullptr) {

        // Visit the left side first for smaller course numbers
        printCourseList(node->left);

        // Print the current course
        cout << node->course.courseNumber << ", "
            << node->course.courseName << endl;

        // Visit the right side for larger course numbers
        printCourseList(node->right);
    }
}


// Load course data from the file
bool loadCourses(BinarySearchTree& courseTree) {

    // Create a list to temporarily store course data
    vector<vector<string>> courseData;

    string filename;
    string line;

    cout << "Enter file name: ";
    getline(cin >> ws, filename);

    // Open the course file
    ifstream courseFile(filename);

    // Check if the file opened
    if (!courseFile.is_open()) {
        cout << "Error: Course file could not be opened" << endl;
        return false;
    }

    // Read every line from the file
    while (getline(courseFile, line)) {

        // Separate the line at each comma
        vector<string> tokens;
        string token;
        stringstream ss(line);

        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        // Check that the course number and name exist
        if (tokens.size() < 2 || tokens[0].empty() || tokens[1].empty()) {
            cout << "Error: Course line is missing a course number or course name" << endl;
            courseFile.close();
            return false;
        }

        // Add the course information to the temporary list
        courseData.push_back(tokens);
    }

    courseFile.close();

    // Check that every prerequisite exists
    for (vector<string> courseLine : courseData) {

        // Start checking after the course number and name
        for (int i = 2; i < courseLine.size(); i++) {

            string prerequisite = courseLine[i];

            //Skip empty fields
            if (prerequisite.empty()) {
                continue;
            }

            bool prerequisiteFound = false;

            // Search the course data for the prerequisite
            for (vector<string> otherCourseLine : courseData) {

                if (prerequisite == otherCourseLine[0]) {
                    prerequisiteFound = true;
                    break;
                }
            }

            // Display an error if the prerequisite does not exist
            if (prerequisiteFound == false) {
                cout << "Error: Prerequisite " << prerequisite
                    << " not found" << endl;
                return false;
            }
        }
    }

    // Create course objects and insert them into the tree
    for (vector<string> courseLine : courseData) {

        Course newCourse;

        // Store the course number and name
        newCourse.courseNumber = courseLine[0];
        newCourse.courseName = courseLine[1];

        // Store any prerequisites
        for (int i = 2; i < courseLine.size(); i++) {

            // Only store prerequisites that are not empty
            if (!courseLine[i].empty()) {
                newCourse.prerequisites.push_back(courseLine[i]);
            }
        }

        // Insert the course into the Binary Search Tree
        courseTree.root = insertCourse(courseTree.root, newCourse);
    }

    return true;
}

// Main program menu
int main() {

    int userChoice = 0;
    string courseNumber;

    cout << "Welcome to the course planner." << endl;

    // Keep showing the menu until the user chooses 9
    while (userChoice != 9) {

        cout << endl;
        cout << "1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit" << endl;
        cout << "What would you like to do? ";
        cin >> userChoice;

        //Check if the user entered something other than a number
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid option. Please enter a number." << endl;
            continue;
        }

        // Option 1: Load course data
        if (userChoice == 1) {
            dataLoaded = loadCourses(courseTree);
        }

        // Option 2: Print all courses in alphanumeric order
        else if (userChoice == 2) {

            if (dataLoaded == true) {
                cout << "Here is a sample schedule:" << endl;
                printCourseList(courseTree.root);
            }
            else {
                cout << "Please load the course data first" << endl;
            }
        }

        // Option 3: Print information for an individual course
        else if (userChoice == 3) {

            if (dataLoaded == true) {
                cout << "What course do you want to know about? ";
                cin >> courseNumber;

                // Convert to uppercase
                for (char& letter : courseNumber) {
                    letter = toupper(letter);
                }

                searchCourse(courseTree, courseNumber);
            }
            else {
                cout << "Please load the course data first" << endl;
            }
        }

        // Option 9: Exit 
        else if (userChoice == 9) {
            cout << "Thank you for using the course planner!" << endl;
        }

        // Display an error for invalid menu choices
        else {
            cout << userChoice << " is not a valid option." << endl;
        }
    }

    return 0;
}



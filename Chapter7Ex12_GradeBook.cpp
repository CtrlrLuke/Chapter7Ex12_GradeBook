/*
File Name: Chapter7Ex12_GradeBook.cpp
Developer/Programmer: Luke Pritchard
Date: 04/07/2025
Requirements:
- Load/save student names and grades from file
- Display student grades and averages (drop lowest)
- Add/change/delete student grades
- Search by student name
- Modular + file-based Grade Book system
*/

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <limits>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

// Function Prototypes
void LoadData(vector<string>&, vector<vector<int>>&);
void SaveData(const vector<string>&, const vector<vector<int>>&);
void DisplayMenu();
int GetValidatedMenuChoice();
void AddStudents(vector<string>&, vector<vector<int>>&);
void AddGrades(vector<string>&, vector<vector<int>>&);
void DisplayGrades(const vector<string>&, const vector<vector<int>>&);
double CalculateAverage(const vector<int>&);
char GetLetterGrade(double);
void SearchAndEdit(vector<string>&, vector<vector<int>>&);

/*
   main
   Entry point and menu loop
*/
int main()
{
    vector<string> studentNames;
    vector<vector<int>> studentGrades;

    LoadData(studentNames, studentGrades);

    bool running = true;
    while (running)
    {
        DisplayMenu();
        int choice = GetValidatedMenuChoice();

        switch (choice)
        {
        case 1: AddStudents(studentNames, studentGrades); break;
        case 2: AddGrades(studentNames, studentGrades); break;
        case 3: DisplayGrades(studentNames, studentGrades); break;
        case 4: SearchAndEdit(studentNames, studentGrades); break;
        case 5:
            SaveData(studentNames, studentGrades);
            cout << "Data saved. Exiting.\n";
            running = false;
            break;
        }
    }

    return 0;
}

/*
   LoadData
   Loads names and grades from grades.txt file
*/
void LoadData(vector<string>& names, vector<vector<int>>& grades)
{
    ifstream file("grades.txt");
    if (!file) return;

    string line;
    while (getline(file, line))
    {
        istringstream iss(line);
        string name;
        iss >> name;

        names.push_back(name);
        vector<int> studentGrades;
        int g;
        while (iss >> g)
            studentGrades.push_back(g);

        grades.push_back(studentGrades);
    }

    file.close();
}

/*
   SaveData
   Saves names and grades to grades.txt file
*/
void SaveData(const vector<string>& names, const vector<vector<int>>& grades)
{
    ofstream file("grades.txt");

    for (size_t i = 0; i < names.size(); i++)
    {
        file << names[i];
        for (int g : grades[i])
            file << " " << g;
        file << "\n";
    }

    file.close();
}

/*
   DisplayMenu
   Lists available options
*/
void DisplayMenu()
{
    cout << "\n--- Grade Book Menu ---\n";
    cout << "1. Add Students\n";
    cout << "2. Add Grades\n";
    cout << "3. Display All Students & Grades\n";
    cout << "4. Search and Edit Student Grades\n";
    cout << "5. Save and Exit\n";
}

/*
   GetValidatedMenuChoice
   Ensures choice is 1–5
*/
int GetValidatedMenuChoice()
{
    string input;
    int choice;
    while (true)
    {
        cout << "Enter choice (1-5): ";
        getline(cin, input);
        if (input.length() == 1 && input[0] >= '1' && input[0] <= '5')
            return input[0] - '0';
        cout << "Invalid input. Try again.\n";
    }
}

/*
   AddStudents
   Add names until sentinel "done"
*/
void AddStudents(vector<string>& names, vector<vector<int>>& grades)
{
    string name;
    cout << "Enter student names (type 'done' to finish):\n";
    while (true)
    {
        cout << "Name: ";
        getline(cin, name);
        if (name == "done") break;
        if (!name.empty())
        {
            names.push_back(name);
            grades.push_back(vector<int>());
        }
        else
        {
            cout << "Name cannot be blank.\n";
        }
    }
}

/*
   AddGrades
   Add multiple grades per student, validated
*/
void AddGrades(vector<string>& names, vector<vector<int>>& grades)
{
    if (names.empty())
    {
        cout << "No students in list.\n";
        return;
    }

    for (size_t i = 0; i < names.size(); i++)
    {
        cout << "\nEnter grades for " << names[i] << " (-1 to finish):\n";
        int grade;
        while (true)
        {
            cout << "Grade: ";
            cin >> grade;
            if (grade == -1) break;
            if (cin.fail() || grade < 0 || grade > 100)
            {
                cout << "Invalid. Enter a number 0–100.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            grades[i].push_back(grade);
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

/*
   DisplayGrades
   Displays each student’s name, grades, average, and letter
*/
void DisplayGrades(const vector<string>& names, const vector<vector<int>>& grades)
{
    if (names.empty())
    {
        cout << "No students to display.\n";
        return;
    }

    cout << fixed << setprecision(2);
    for (size_t i = 0; i < names.size(); i++)
    {
        cout << "\n" << names[i] << ": ";
        for (int g : grades[i])
            cout << g << " ";

        double avg = CalculateAverage(grades[i]);
        cout << "\nAverage (lowest dropped): " << avg;
        cout << " -> Grade: " << GetLetterGrade(avg) << "\n";
    }
}

/*
   CalculateAverage
   Drops lowest, averages rest
*/
double CalculateAverage(const vector<int>& g)
{
    if (g.empty()) return 0.0;
    if (g.size() == 1) return g[0];

    int sum = 0, lowest = g[0];
    for (int grade : g)
    {
        sum += grade;
        if (grade < lowest)
            lowest = grade;
    }

    return static_cast<double>(sum - lowest) / (g.size() - 1);
}

/*
   GetLetterGrade
   A/B/C/D/F scale
*/
char GetLetterGrade(double avg)
{
    if (avg >= 90) return 'A';
    if (avg >= 80) return 'B';
    if (avg >= 70) return 'C';
    if (avg >= 60) return 'D';
    return 'F';
}

/*
   SearchAndEdit
   Search student, then add/edit/delete grades
*/
void SearchAndEdit(vector<string>& names, vector<vector<int>>& grades)
{
    string target;
    cout << "Enter student name to search: ";
    getline(cin, target);

    for (size_t i = 0; i < names.size(); i++)
    {
        if (names[i] == target)
        {
            cout << "\nFound: " << names[i] << "\nGrades: ";
            for (int g : grades[i]) cout << g << " ";
            cout << "\n";

            cout << "\n1. Add a Grade\n2. Edit a Grade\n3. Delete a Grade\nChoice: ";
            int opt;
            cin >> opt;

            if (opt == 1)
            {
                int newGrade;
                cout << "Enter new grade: ";
                cin >> newGrade;
                if (newGrade >= 0 && newGrade <= 100)
                    grades[i].push_back(newGrade);
                else
                    cout << "Invalid grade.\n";
            }
            else if (opt == 2)
            {
                int index;
                cout << "Which grade index (0–" << grades[i].size() - 1 << "): ";
                cin >> index;
                if (index >= 0 && index < static_cast<int>(grades[i].size()))
                {
                    int newVal;
                    cout << "Enter new value: ";
                    cin >> newVal;
                    if (newVal >= 0 && newVal <= 100)
                        grades[i][index] = newVal;
                    else
                        cout << "Invalid grade.\n";
                }
                else
                    cout << "Invalid index.\n";
            }
            else if (opt == 3)
            {
                int index;
                cout << "Which grade index to delete: ";
                cin >> index;
                if (index >= 0 && index < static_cast<int>(grades[i].size()))
                    grades[i].erase(grades[i].begin() + index);
                else
                    cout << "Invalid index.\n";
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }
    }

    cout << "Student not found.\n";
}

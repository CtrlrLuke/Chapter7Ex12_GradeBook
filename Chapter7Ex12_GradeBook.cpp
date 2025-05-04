/*
File Name: Chapter7Ex12_GradeBook.cpp
Developer/Programmer: Luke Pritchard
Date: 04/07/2025
Requirements:
- Add student names (stop with sentinel "done")
- Add grades (positive, <101)
- Drop lowest grade for average
- Display average + letter grade per student
- Use parallel vectors
*/

#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <iomanip>
#include <algorithm>

using namespace std;

// Function Prototypes
void DisplayMenu();
int GetValidatedMenuChoice();
void AddStudents(vector<string>&, vector<vector<int>>&);
void AddGrades(vector<string>&, vector<vector<int>>&);
void DisplayGrades(const vector<string>&, const vector<vector<int>>&);
double CalculateAverage(const vector<int>&);
char GetLetterGrade(double);

/*
   main
   Controls program loop
*/
int main()
{
    vector<string> studentNames;
    vector<vector<int>> studentGrades;
    bool running = true;

    while (running)
    {
        DisplayMenu();
        int choice = GetValidatedMenuChoice();

        switch (choice)
        {
        case 1:
            AddStudents(studentNames, studentGrades);
            break;
        case 2:
            AddGrades(studentNames, studentGrades);
            break;
        case 3:
            DisplayGrades(studentNames, studentGrades);
            break;
        case 4:
            running = false;
            cout << "Exiting Grade Book. Goodbye!\n";
            break;
        }
    }

    return 0;
}

/*
   DisplayMenu
   Shows the main options
*/
void DisplayMenu()
{
    cout << "\n--- Grade Book Menu ---\n";
    cout << "1. Add Students\n";
    cout << "2. Add Grades\n";
    cout << "3. Display Student Grades\n";
    cout << "4. Exit\n";
}

/*
   GetValidatedMenuChoice
   Validates menu input (1–4)
*/
int GetValidatedMenuChoice()
{
    string input;
    int choice;

    while (true)
    {
        cout << "Enter choice (1-4): ";
        getline(cin, input);

        if (input.length() == 1 && input[0] >= '1' && input[0] <= '4')
        {
            choice = input[0] - '0';
            return choice;
        }

        cout << "ERROR: Please enter 1, 2, 3, or 4.\n";
    }
}

/*
   AddStudents
   Prompts for student names; ends on sentinel "done"
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

        if (name.empty())
        {
            cout << "ERROR: Name cannot be blank.\n";
        }
        else
        {
            names.push_back(name);
            grades.push_back(vector<int>()); // Initialize empty grade list
        }
    }
}

/*
   AddGrades
   Adds numeric grades to each student; input validated
*/
void AddGrades(vector<string>& names, vector<vector<int>>& grades)
{
    if (names.empty())
    {
        cout << "No students entered yet.\n";
        return;
    }

    for (size_t i = 0; i < names.size(); i++)
    {
        cout << "\nEntering grades for " << names[i] << " (type -1 to finish):\n";
        int grade;

        while (true)
        {
            cout << "Enter grade (0-100): ";
            cin >> grade;

            if (grade == -1) break;

            if (cin.fail() || grade < 0 || grade > 100)
            {
                cout << "ERROR: Grade must be between 0 and 100.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            grades[i].push_back(grade);
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer
    }
}

/*
   DisplayGrades
   Displays all students, their grades, average (w/o lowest), and letter grade
*/
void DisplayGrades(const vector<string>& names, const vector<vector<int>>& grades)
{
    if (names.empty())
    {
        cout << "No students to display.\n";
        return;
    }

    cout << fixed << setprecision(2);
    cout << "\n--- Student Grades ---\n";

    for (size_t i = 0; i < names.size(); i++)
    {
        cout << "\n" << names[i] << ": ";

        if (grades[i].empty())
        {
            cout << "No grades entered.\n";
            continue;
        }

        for (int grade : grades[i])
            cout << grade << " ";

        double avg = CalculateAverage(grades[i]);
        char letter = GetLetterGrade(avg);

        cout << "\nAverage (lowest dropped): " << avg;
        cout << " -> Letter Grade: " << letter << "\n";
    }
}

/*
   CalculateAverage
   Returns average of grades after dropping the lowest
*/
double CalculateAverage(const vector<int>& g)
{
    if (g.size() <= 1)
        return (g.empty() ? 0.0 : static_cast<double>(g[0]));

    int sum = 0;
    int lowest = g[0];

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
   Returns A/B/C/D/F based on average
*/
char GetLetterGrade(double avg)
{
    if (avg >= 90) return 'A';
    else if (avg >= 80) return 'B';
    else if (avg >= 70) return 'C';
    else if (avg >= 60) return 'D';
    else return 'F';
}

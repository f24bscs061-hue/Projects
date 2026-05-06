// include directives and
// all relevant header files 
// containing class definitions
#include <iostream> 
#include "Person.h"
#include "Student.h"
#include "Admin.h"
#include "Degree.h"
#include "Subject.h"
#include "College.h"
#include "FileHandling.h"
#include "AdmissionRecord.h"

using namespace std;   //include standard library

// initialize static public 
// attrinutes of College class
int College::totalAdmissions = 0;
int College::totalDegrees = 0;


int main()   // start program
{

	FileHandling file;
	College college;
	Admin admin;
	// Instances used in main loop 
	// to execute functionality 
	
	int id;
	int option;
	double matric, fsc, test;
	string name, cnic, degree,status;
	// Variables to store user-input
	
	file.LoadStudents(college);
	file.LoadDegrees(college);
	// Auto-load to initialize program 
	

	do    // Do-while repeats switch-cases
	{
		cout << "------------------------------" << endl;
		cout << "=== College Admission Menu ===" << endl;
		cout << " 1. Register new Student" << endl;
		cout << " 2. View all Students" << endl;
		cout << " 3. View Degree Programs" << endl;
		cout << " 4. Search Student by CNIC" << endl;
		cout << " 5. Generate Merit List" << endl;
		cout << " 6. Save Records to file" << endl;
		cout << " 7. Load Records from file" << endl;
		cout << " 8. Admin Account Login" << endl;
		cout << " 0. Exit" << endl;
		cout << "--------------------------" << endl;
		cout << "\033[33mSelect one of the options\033[0m" << endl;
		// Main menu design for user

		cin >> option;   
		switch (option)       // Switch functionality of program
		{
		case 1:
		{
			cin.ignore();
			cout << "\033[33mEnter Student's Information\033[0m" << endl;
			cout << "ID: ";
			cin >> id;
			cin.ignore();
			cout << "Name: ";
			getline(cin, name);
			cout << "CNIC: ";
			getline(cin, cnic);
			// Student input 

			bool degree_found = false;
			while (!degree_found)
			{
				cout << "Degree Program: ";
				getline(cin, degree);

				//check if specified degree program exists
				Degree* degrees = college.getDegrees();   // using College instance 
				for (int i = 0; i < sizeof(degrees); i++) 
				{
					if (degree == degrees[i].getTitle()) // match degree title
					{
						degree_found = true;
						break;
					}
				}
				if (!degree_found) 
				{
					cout << "\033[31mSpecified Degree Programs is not being offered. Please try again...\033[0m" << endl;
				}
			} // end while - Degree program matched

			// Record to calculate Aggregate
			cout << "Marks in Matric: ";
			cin >> matric;
			cout << "Marks in Inter: ";
			cin >> fsc;
			cout << "Marks in Entry Test: ";
			cin >> test;
			cout << "------------------------" << endl;

			if (degree_found) 
			{
				Student s(id, name, cnic, degree, matric, fsc, test, 0, "Pending");
				college.registerStudent(s);
			}// Student Registered
			break;
		}
		case 2:
			college.viewStudents();   // View all registered Students
			break;
		case 3:
			college.viewDegrees();   // View added Degree programs 
			break;
		case 4:
		{
			//Search record in file by cnic
			string cnic;
			cout << "Enter CNIC to search: ";
			cin >> cnic;
			college.searchStudent(cnic);
			break;
		}
		case 5:
		{
			// Checks criteria and shows list
			// Student qualified or not with aggregate
			college.generateMeritList();
			break;
		}
		case 6:
		{
			// file handling instance
			file.SaveStudents(college);
			cout << "-------------------------" << endl;
			//Saves each new entry of student and degree in files
			break;
		}
		case 7:
		{
			// file handling instance
			file.LoadStudents(college);
			file.LoadDegrees(college);
			cout << "--------------------------" << endl;
			// Optional loading after saving data in files
			break;
		}
		case 8:
		{
			cin.ignore();
			string user, pass;
			cout << "UserName: ";
			getline(cin, user);
			cout << "Password: ";
			getline(cin, pass);
			// Admin input

			// Admin instance used
			if (admin.login(user, pass)) {
				admin.adminMenu(college);
			}// Checks account Login data
			break;
		}
		}
	} while (option != 0);  // end do-while if user enters 0 
	return 0;
} // end program

// Friend function 
static void generateMerit(Student s, Degree d)
{
	if (s.getAggregate() >= 50.0)
		cout << "Merit Eligible: \n" << s << "\n" << d;
}
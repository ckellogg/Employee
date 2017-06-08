#include "Employee.h"
using namespace std;

int main(int argc, char* argv[]) {
	string xmlFile = "";
	ifstream myStream;
	
	// 1) Obtain the name of an XML file to read from the command line(argv[1]).Print an error
	// message and halt the program if there is no command-line argument provided, or if the file does not exist.
	
	if (argc == 2) {
		xmlFile = argv[1];
		myStream.open(xmlFile);
		if (myStream.fail()) {
			cout <<  xmlFile << " could not be opened." << endl;
			return -1;
		}
	}
	else {
		cout << "Please work with me here try: ./a.out employee.xml" << endl;
		return -1;
	}

	// 2) Read each XML record in the file by repeatedly calling Employee::fromXML, which creates 
	// an Employee object on - the - fly, and store it in a vector of Employee pointers(you may 
	// use smart pointers if you wish).
	vector<unique_ptr<Employee>> EmpVector;
	while (!myStream.eof()) {
		try {
			EmpVector.push_back(unique_ptr<Employee>(Employee::fromXML((myStream))));

			if (EmpVector.back() == nullptr) {
				EmpVector.pop_back();
			}
		}
		catch (runtime_error e) {
			cout << e.what() << endl;
			return -1;
		}
	}
	myStream.close();

	// 3)	Loop through your vector and print to cout the Employee data for each object(using the display member function).
	for (int i = 0; i < EmpVector.size(); i++) {
		(*EmpVector[i]).display(cout); 
	}

	// 4) The next step is to create a new file of fixed - length Employee records.This is 
	//	explained below.Write the records for each employee to your new file(call it 
	//	“employee.bin”) in the order they appear in your vector.Open this file as an 
	//	fstream object with both read and write capability, and in binary format.
	ofstream writeStream("employee.bin", ios::out | ios::binary);
	for (int i = 0; i < EmpVector.size(); i++) {
		(*EmpVector[i]).write(writeStream);
	}
	writeStream.close();

	// 5)	Clear your vector in for the next step.
	EmpVector.clear();
	
	// 6)	Traverse the file by repeated calls to Employee::read, filling your newly emptied 
	//	vector with Employee pointers for each record read.
	fstream editStream("employee.bin", ios::in | ios::out | ios::binary);
	bool good = true;
	while (good) {
		try {
			EmpVector.push_back(unique_ptr<Employee>(Employee::read((editStream))));
			if (EmpVector.back() == nullptr) {
				EmpVector.pop_back();
				good = false;
			}
		}
		catch (runtime_error e) {
			cout << e.what() << endl;
			return -1;
		}
	}
	// 7)	Loop through your vector and print to cout an XML representation of each Employee using Employee::toXML.
	for (int i = 0; i < EmpVector.size(); i++) {
		(*EmpVector[i]).toXML(cout);
	}
	// 8)	Search the file for the Employee with id 12345 using Employee::retrieve.
	unique_ptr<Employee> emp(Employee::retrieve(editStream, 12345));
	cout << "\nFound:" << endl;
	(*emp).display(cout);

	// 9)	Change the salary for the retrieved object to 150000.0
	(*emp).salary = 150000.0;
	
	// 10)	Write the modified Employee object back to file using Employee::store
	(*emp).store(editStream);

	// 11)	Retrieve the object again by id(12345) and print its salary to verify that the 
	//	file now has the updated salary.
	unique_ptr<Employee> newSalary(Employee::retrieve(editStream, 12345));
	cout << (*newSalary).salary <<endl << endl;
	editStream.close();

	fstream writeEmpStream("employee.bin", ios::in | ios::out | ios::binary );

	// 12)	Create a new Employee object of your own with a new, unique id, along with other information.
	int id = 652;
	double salary = 100000.01;
	string name = "Max a Million";
	string address = "100 Bankrupt drive", city = "Dollarville", state = "NY", country = "USA", phone = "661-236-2213";
	
	unique_ptr<Employee> newEmp(new Employee(name, id, address, city, state, country, phone, salary));

	// 13)	Store it in the file using Employee::store.
	(*newEmp).store(writeEmpStream);

	// 14)	Retrieve the record with Employee::retrieve and display it to cout.
	unique_ptr<Employee> Max(Employee::retrieve(writeEmpStream, id));
	cout << "Found:" << endl;
	(*Max).display(cout);
}
//Let me just say WOW. Lots of new libraries that are a 1st time use. Im thankful for the lab and people willing to teach me :) 
//I learned that even the simpliest things can get you frustrated. Revisting the reinturpretcast was good.
//Unique_ptrs where new but not too bad to get. I can see good things to come for those. 
//I wanted to lookup if there was a template for formating data so the code could be more efficent.
//I hadnt really used vectors much before this class. So that was nice. I can already see some ideas where Id 
//like to use them in my own projects.
//Employee.h
#ifndef EMPLOYEE_H
#define EMPLOYEE_H
#include <limits>
#include <map>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string>
#include <memory>


class Employee {
private:
	struct EmployeeRec {
		int id;
		char name[31];
		char address[26];
		char city[21];
		char state[21];
		char country[21];
		char phone[21];
		double salary;
	};
	
public:
	int id;
	std::string name;
	std::string address;
	std::string city;
	std::string state;
	std::string country;
	std::string phone;
	double salary;

	Employee(); // Constructor
	Employee(std::string, int, std::string, std::string, std::string, std::string, std::string, double); // Paramaterized constructor
	void display(std::ostream&) const; // Write readable Employee representation to a stream
	void write(std::ostream&) const; // Write fixed-length record to current file position
	void store(std::iostream&) const; // Overwrite (or append) record in (to) file
	void toXML(std::ostream&) const; // Write XML record for Employee
	static Employee* read(std::istream&); // Read record from current file position
	static Employee* retrieve(std::istream&, int); // Search file for record by id
	static Employee* fromXML(std::istream&); // Read the XML record from a stream
	static std::string getNextTag(std::istream&);
	static std::string getNextValue(std::istream&);
	static void compareTags(std::string, std::string);
	static bool repeatTags(std::string, std::string);
};

#endif 

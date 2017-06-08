//Employee.cpp
#include "Employee.h"
using namespace std;
char openingTagSym= '<';
char closingTagsym= '>';

// default Constructor
Employee::Employee() {
	name = "";
	id = -1;
	address = "";
	city = "";
	state = "";
	country = "";
	phone = "";
	salary = 0.0;
}

Employee::Employee(string ename, int eid, string eaddress, string ecity, string estate, string ecountry, string ephone, double esalary) :
id(eid), name(ename) {
    this->address = eaddress; 
    this->city = ecity; 
    this->state = estate; 
    this->country = ecountry; 
    this->phone = ephone; 
    this->salary = esalary;
}


void Employee::display(std::ostream& os) const {    // Write a readable Employee representation to a stream
	os << "id: " << id << endl;
	os << "name: " << name << endl;
	os << "address: " << address << endl;
	os << "city: " << city << endl;
	os << "state: " << state << endl;
	os << "country: " << country << endl;
	os << "phone: " << phone << endl;
	os << "salary: " << salary << endl << endl;
}

void Employee::write(std::ostream& os) const {    // Write a fixed-length record to current file curition
	EmployeeRec outbuf;
	strncpy(outbuf.name, name.c_str(), 31)[31] = '\0';
	outbuf.id = id;
	strncpy(outbuf.address, address.c_str(), 26)[26] = '\0';
	strncpy(outbuf.city, city.c_str(), 21)[21] = '\0';
	strncpy(outbuf.state, state.c_str(), 21)[21] = '\0';
	strncpy(outbuf.country, country.c_str(), 21)[21] = '\0';
	strncpy(outbuf.phone, phone.c_str(), 21)[21] = '\0';
	outbuf.salary = salary;
	os.write(reinterpret_cast<const char*>(&outbuf), sizeof(outbuf));
}

void Employee::store(std::iostream& io) const {  // Overwrite (or append) record in (to) file
	io.clear();
	io.seekg(0, ios::beg);
	ios::pos_type cur;
	while (!io.eof()) {
		cur = io.tellg();
		unique_ptr<Employee> emp(Employee::read(io));

		if (emp != nullptr && (*emp).id == id) {
			io.seekp(cur, ios::beg);
			write(io);
			return;
		}
	}
	io.clear();
	io.seekp(cur, ios::beg);
	write(io);
}

void Employee::toXML(std::ostream& os) const {  // Write XML record for Employee
	os << "<Employee>\n\t<Name>" << name << "</Name>\n\t<ID>" << id << "</ID>";
	if (address != "")
		os << "\n\t<Address>" << address << "</Address>";
	if (city != "")
		os << "\n\t<City>" << city << "</City>";
	if (state != "")
		os << "\n\t<State>" << state << "</State>";
	if (country != "")
		os << "\n\t<Country>" << country << "</Country>";
	if (phone != "")
		os << "\n\t<Phone>" << phone << "</Phone>";
	if (salary != 0.0)
		os << "\n\t<Salary>" << salary << "</Salary>";
	os << "\n</Employee>\n";
}

Employee* Employee::read(std::istream& is) {    // Read record from current file curition
	EmployeeRec inbuf;
	is.read(reinterpret_cast<char*>(&inbuf), sizeof inbuf);
	unique_ptr<Employee> emp(new Employee);
	if (is) {
		(*emp).name = inbuf.name;
		(*emp).id = inbuf.id;
		(*emp).address = inbuf.address;
		(*emp).city = inbuf.city;
		(*emp).state = inbuf.state;
		(*emp).country = inbuf.country;
		(*emp).phone = inbuf.phone;
		(*emp).salary = inbuf.salary;
		return emp.release();
	}
	return nullptr;
}

Employee* Employee::retrieve(std::istream& is, int idNum) {     // Search file for record by id
	is.clear();
	is.seekg(0, ios::beg);

	while (!is.eof()) {
		unique_ptr<Employee> emp(Employee::read(is));
		if ((*emp).id == idNum) {
			return emp.release();
		}
	}
	return nullptr;
}

Employee* Employee::fromXML(std::istream& is) {     // Read the XML record from a stream

	if (!is.eof()) {
		unique_ptr<Employee> emp(new Employee);
		bool missing = false;
		string tag;
		string firstTag = getNextTag(is);
		if (firstTag == "") {
			return nullptr;
		}
		else if (firstTag != "<Employee>") {
			string e = "Missing <Employee> tag";
			throw runtime_error(e);
		}

		while (missing == false) {
			tag = getNextTag(is);

			if (repeatTags(tag, "</Employee>")) {
				if ((*emp).id != -1 && (*emp).name != "") {
					missing = true;
				}
				if (missing == false) {
					if ((*emp).name == "") {
						throw runtime_error("Missing <Name> tag");
					}
					else if ((*emp).id == -1) {
						throw runtime_error("Missing <Id> tag");
					}
				}
				else
					missing = true;
			}
			else if (repeatTags(tag, "<Id>")) {
				if ((*emp).id != -1)
					throw runtime_error("Multiple <Id> tags");
				int idVal;
				stringstream(getNextValue(is)) >> idVal;
				(*emp).id = idVal;
				string closed = getNextTag(is);
				compareTags(closed, "</Id>");
			}
			else if (repeatTags(tag, "<Salary>")) {
				if ((*emp).salary != 0.0)
					throw runtime_error("Multiple </Salary> tags");
				double salaryVal;
				stringstream(getNextValue(is)) >> salaryVal;
				(*emp).salary = salaryVal;
				string closed = getNextTag(is);
				compareTags(closed, "</Salary>");
			}
			else if (repeatTags(tag, "<Name>")) {
				if ((*emp).name != "")
					throw runtime_error("Multiple </Name> tags");
				string nameVal = getNextValue(is);
				(*emp).name = nameVal;
				string closed = getNextTag(is);
				compareTags(closed, "</Name>");
			}
			else if (repeatTags(tag, "<Address>")) {
				if ((*emp).address != "")
					throw runtime_error("Multiple </Address> tags");
				string addressVal = getNextValue(is);
				(*emp).address = addressVal;
				string closed = getNextTag(is);
				compareTags(closed, "</Address>");
			}
			else if (repeatTags(tag, "<City>")) {
				if ((*emp).city != "")
					throw runtime_error("Multiple <City> tags");
				string cityVal = getNextValue(is);
				(*emp).city = cityVal;
				string closed = getNextTag(is);
				compareTags(closed, "</City>");
			}
			else if (repeatTags(tag, "<State>")) {
				if ((*emp).state != "")
					throw runtime_error("Multiple <State> tags");
				string stateVal = getNextValue(is);
				(*emp).state = stateVal;
				string closed = getNextTag(is);
				compareTags(closed, "</State>");
			}
			else if (repeatTags(tag, "<Country>")) {
				if ((*emp).country != "")
					throw runtime_error("Multiple <Country> tags");
				string countryVal = getNextValue(is);
				(*emp).country = countryVal;
				string closed = getNextTag(is);
				compareTags(closed, "</Country>");
			}
			else if (repeatTags(tag, "<Phone>")) {
				if ((*emp).phone != "")
					throw runtime_error("Multiple <Phone> tags");
				string phoneVal = getNextValue(is);
				(*emp).phone = phoneVal;
				string closed = getNextTag(is);
				compareTags(closed, "</Phone>");
			}
			else {
				string e = "Invalid tag: " + tag;
				throw runtime_error(e);
			}
		}
		return emp.release();
	}
	return nullptr;
}

string Employee::getNextValue(std::istream& is) {   //gets the next value/data
	string tempVal = "";
	char readVal;
	readVal = is.get();
	while (readVal != openingTagSym) {
		tempVal += readVal;
		readVal = is.get();
	}
	is.unget();
	return tempVal;
}

string Employee::getNextTag(std::istream& is) {     //gets the next tag
	bool found = false;
	bool isOkay = true;
	string data = "";
	while (found == false && !is.eof()) {
		char readVal;
		readVal = is.get();
		if (readVal == openingTagSym) {
			isOkay = false;
		}
		else if (readVal == closingTagsym) {
			found = true;
		}
		if (isOkay == false) {
			data += readVal;
		}
	}
	return data;
}

void Employee::compareTags(string checkTag, string compare) {   //compares tags
	int value = strcasecmp(checkTag.c_str(), compare.c_str());
	if (value == 0) {
		return;
	}
	else {
		string e = "Missing " + compare + " tag";
		throw runtime_error(e);
	}
}

bool Employee::repeatTags(string checkTag, string toCompare) {      //checks for repeats tags
	int val = strcasecmp(checkTag.c_str(), toCompare.c_str());
	if (val == 0) {
		return true;
	}
	else
		return false;
}

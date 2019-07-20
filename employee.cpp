#include "employee.h"
#include <cstring>
#include <vector>

static std::bitset<8> bset;

static int setBit(std::string);

static void toRec(const Employee& e, emprec& rec) {
	strncpy_s(rec.address, e.address.c_str(), sizeof(rec.address)-1);
	strncpy_s(rec.city, e.city.c_str(), sizeof(rec.city)-1);
	strncpy_s(rec.country, e.country.c_str(), sizeof(rec.country)-1);
	rec.id = e.id;
	strncpy_s(rec.name, e.name.c_str(), sizeof(rec.name)-1);
	strncpy_s(rec.phone, e.phone.c_str(), sizeof(rec.phone)-1);
	rec.salary = e.salary;
	strncpy_s(rec.state, e.state.c_str(), sizeof(rec.state)-1);
}

Employee::Employee(std::string _name, int _id, std::string _address, std::string _city,
	std::string _state, std::string _country, std::string _phone, double _salary) : name{ _name },
	id{ _id }, address{ _address }, city{ _city }, state{ _state },
	country{ _country }, phone{ _phone }, salary{ _salary } { }

Employee::~Employee() {
}

// Overwrite (or append) record in (to) file
void Employee::store(std::iostream& ios) const {
	ios.clear(); ios.seekg(0, std::ios::beg);//start from beginning
	auto pos = ios.tellg();
	emprec rec;
	while (ios) {
		ios.read(reinterpret_cast<char*>(&rec), sizeof(emprec));
		if (rec.id == id) {
			//overwrite
			emprec _rec;
			toRec(*this, _rec);
			ios.seekp(pos);
			ios.write(reinterpret_cast<const char*>(&_rec), sizeof(emprec));
			return;
		}
		else {
			pos = ios.tellg();
			if (pos == -1) {
				ios.clear();
				ios.seekp(0, std::ios::end);
				break;
			}
		}
			
	}//end of file...write at end:
	toRec(*this, rec);
	ios.write(reinterpret_cast<const char*>(&rec), sizeof(emprec));
}

// Write a readableEmployee representation to a stream
void Employee::display(std::ostream& os) const {
	os << "Name: " << this->name << std::endl;
	os << "ID: " << this->id << std::endl;
	if (this->address != "")
		os << "Address: " << this->address << std::endl;
	if (this->city != "")
		os << "City: " << this->city << std::endl;
	if (this->state != "")
		os << "State: " << this->state << std::endl;
	if (this->country != "")
		os << "Country: " << this->country << std::endl;
	if (this->phone != "")
		os << "Phone: " << this->phone << std::endl;
	if (this->salary != 0.0) {
		os << std::fixed;
		os << std::setprecision(2); //ensures correct dollar format
		os << "Salary: $" << this->salary << std::endl;
	}
	os << std::endl;
}

// Write a value of an employee.
// string s is the value to be found
// os is the stream to write to
void Employee::display(std::ostream& os, const std::string& s) const {
	int bit = -1;
	bit = setBit(s);
	bset.reset();

	switch (bit)
	{
	case 0:
		os.write(name.c_str(), strlen(name.c_str()));
		os << std::endl;
		break;
	case 1:
		os.write(std::to_string(id).c_str(), strlen(std::to_string(id).c_str()));
		os << std::endl;
		break;
	case 2:
		os.write(address.c_str(), strlen(address.c_str()));
		os << std::endl;
		break;
	case 3:
		os.write(city.c_str(), strlen(city.c_str()));
		os << std::endl;
		break;
	case 4:
		os.write(state.c_str(), strlen(city.c_str()));
		os << std::endl;
		break;
	case 5:
		os.write(country.c_str(), strlen(country.c_str()));
		os << std::endl;
		break;
	case 6:
		os.write(phone.c_str(), strlen(phone.c_str()));
		os << std::endl;
		break;
	case 7:
		os << std::fixed;
		os << std::setprecision(2); //ensures correct dollar format
		os << "$" << this->salary << std::endl;
		break;
	case -1:
		throw(std::runtime_error("bitSet() returned an unexpected integer.\n"));
	default:
		break;
	}
}

// Write a fixed-length record to current file position
void Employee::write(std::ostream& os) const {
	emprec rec;
	toRec(*this, rec);
	os.write(reinterpret_cast<const char*>(&rec), sizeof(emprec));
}

// Write XML record for Employee
void Employee::toXML(std::ostream& os) const {
	emprec rec;
	toRec(*this, rec);

	os.write("<employee>\n", sizeof("<employee>\n") - 1);
	if (address != "") {
		os.write("\t<address>", sizeof("\t<address>") - 1);
		os.write(rec.address, strlen(rec.address));
		os.write("</address>\n", sizeof("</address>\n") - 1);
	}
	if (city != "") {
		os.write("\t<city>", sizeof("\t<city>") - 1);
		os.write(rec.city, strlen(rec.city));
		os.write("</city>\n", sizeof("</city>\n") - 1);
	}
	if (country != "") {
		os.write("\t<country>", sizeof("\t<country>") - 1);
		os.write(rec.country, strlen(rec.country));
		os.write("</country>\n", sizeof("</country>\n") - 1);
	}
	os.write("\t<id>", sizeof("\t<id>") - 1);
	std::string _id(std::to_string(rec.id));
	os.write(_id.c_str(), strlen(_id.c_str()));
	os.write("</id>\n\t<name>", sizeof("</id>\n\t<name>") - 1);
	os.write(rec.name, strlen(rec.name));
	os.write("</name>\n", sizeof("</name>\n") - 1);
	if (phone != "") {
		os.write("\t<phone>", sizeof("\t<phone>") - 1);
		os.write(rec.phone, strlen(rec.phone));
		os.write("</phone>\n", sizeof("</phone>\n") - 1);
	}
	if (salary != 0.0) {
		os.write("\t<salary>", sizeof("\t<salary>") - 1);
		std::string _salary(std::to_string(rec.salary));
		os.write(_salary.c_str(), strlen(_salary.c_str()));
		os.write("</salary>\n", sizeof("</salary>\n") - 1);
	}
	if (state != "") {
		os.write("\t<state>", sizeof("\t<state>") - 1);
		os.write(rec.state, strlen(rec.state));
		os.write("</state>\n", sizeof("</state>\n") - 1);
	}
	os.write("</employee>\n", sizeof("</employee>\n") - 1);
}

//returns index of bit set or -1 if fail
int setBit(std::string s) {
	if (!_stricmp(s.c_str(), "name")) {
		if (bset[0] == 1) { bset.reset(); return -1; }
		bset.set(0, 1); return 0; 
	}
	else if (!_stricmp(s.c_str(), "id")) {
		if (bset[1] == 1) { bset.reset(); return -1; }
		bset.set(1, 1); return 1;
	}
	else if (!_stricmp(s.c_str(), "address")) {
		if (bset[2] == 1) { bset.reset(); return -1; }
		bset.set(2, 1); return 2;
	}
	else if (!_stricmp(s.c_str(), "city")) {
		if (bset[3] == 1) { bset.reset(); return -1; }
		bset.set(3, 1); return 3;
	}
	else if (!_stricmp(s.c_str(), "state")) {
		if (bset[4] == 1) { bset.reset(); return -1; }
		bset.set(4, 1); return 4;
	}
	else if (!_stricmp(s.c_str(), "country")) {
		if (bset[5] == 1) { bset.reset(); return -1; }
		bset.set(5, 1); return 5;
	}
	else if (!_stricmp(s.c_str(), "phone")) {\
		if (bset[6] == 1) { bset.reset(); return -1; }
		bset.set(6, 1); return 6;
	} 
	else if (!_stricmp(s.c_str(), "salary")) {
			if (bset[7] == 1) { bset.reset(); return -1; }
		bset.set(7, 1); return 7;
	}
	else { return -1; }
}

// Read record from current file position
//	*uses new
Employee* Employee::read(std::istream& is) {
	emprec rec;
	is.read(reinterpret_cast<char*>(&rec), sizeof(emprec));
	if (is.eof()) {
		return nullptr;
	}
	if (rec.name == "") {
		throw std::runtime_error("Employee was missing a name.");
		return nullptr;
	}
	else if (rec.id == 0) {
		throw std::runtime_error("Employee was missing an id.");
		return nullptr;
	}
	Employee* emp = new Employee(rec.name, rec.id, rec.address, rec.city, rec.state, rec.country, rec.phone, rec.salary);
	return emp;
}

// Search file for record by id
//	*uses new
Employee* Employee::retrieve(std::istream& is, int n) {
	emprec rec;
	is.clear();  is.seekg(0, std::ios::beg);
	is.read(reinterpret_cast<char*>(&rec), sizeof(emprec));
	while (is) {
		if (rec.id == n) {
			Employee* emp = new Employee(rec.name, rec.id, rec.address, rec.city, rec.state, rec.country, rec.phone, rec.salary);
			return emp;
		}
		is.read(reinterpret_cast<char*>(&rec), sizeof(emprec));
	}
	if (rec.id == 0 || rec.name == "") {
		return nullptr;
	}
}

// Read the XML record from a stream
//	*uses new
Employee* Employee::fromXML(std::istream& is) {
	int bit = -1;
	std::string vals[8];

	if (is.eof())
		return nullptr;

	is.ignore(std::numeric_limits<std::streamsize>::max(), '<');
	std::string s;
	getline(is, s, '>');

	while (!_stricmp(s.c_str(), "employee")) {
		is.ignore(std::numeric_limits<std::streamsize>::max(), '<');
		getline(is, s, '>');
	}//found first data in employee
	bit = setBit(s);
	getline(is, s, '<');
	vals[bit] = s; //store value

	while (_stricmp(s.c_str(), "/employee")) {
		getline(is, s, '>');
		if (s[0] != '/') {
			bit = setBit(s);
			if (bit == -1) {
				//duplicate or mismatched xml tag
				std::cout << "ERROR\n"; return nullptr;
			}
			else { getline(is, s, '<'); vals[bit] = s; }
		}
		else
			is.ignore(std::numeric_limits<std::streamsize>::max(), '<');
	}
	if (vals[0] == "") {
		throw (std::runtime_error("Employee was missing a name."));
		return nullptr;
	}
	else if (vals[1] == "") {
		throw (std::runtime_error("Employee was missing an id."));
		return nullptr;
	}
	else {
		int _id = std::stoi(vals[1]);
		double _salary = std::stod(vals[7]);
		Employee* emp = new Employee(vals[0], _id, vals[2], vals[3], vals[4], vals[5], vals[6], _salary);
		bset.reset();
		return emp;
	}
}

// Updates a specified field with a specified value given in string form
void Employee::update(const std::string& field, const std::string& value) {
	if (!_stricmp(field.c_str(), "address")) {
		address = value;
	}
	else if (!_stricmp(field.c_str(), "city")) {
		city = value;
	}
	else if (!_stricmp(field.c_str(), "country")) {
		country = value;
	}
	else if (!_stricmp(field.c_str(), "id")) {
		id = std::stoi(value);
	}
	else if (!_stricmp(field.c_str(), "name")) {
		name = value;
	}
	else if (!_stricmp(field.c_str(), "phone")) {
		phone = value;
	}
	else if (!_stricmp(field.c_str(), "salary")) {
		salary = std::stod(value);
	}
	else if (!_stricmp(field.c_str(), "state")) {
		state = value;
	}
	else { //!error

	}
}

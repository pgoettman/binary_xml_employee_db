#include <string>
#include <iostream>
#include <fstream>
#include <cstddef>
#include <stdexcept>
#include <bitset>
#include <string.h>
#include <iomanip>
#include "emprec.h"

#ifndef EMPLOYEE_H
#define EMPLOYEE_H
class Employee
{
public:
	Employee(std::string, int, std::string, std::string, std::string, std::string, std::string, double);
	~Employee();
	void display(std::ostream&) const;	// Write a readableEmployee representation to a stream
	void write(std::ostream&) const;	// Write a fixed-length record to current file position
	void store(std::iostream&) const;	// Overwrite (or append) record in (to) file
	void toXML(std::ostream&) const;	// Write XML record for Employee
	static Employee* read(std::istream&);	// Read record from current file position
	static Employee* retrieve(std::istream&, int);	// Search file for record by id
	static Employee* fromXML(std::istream&);	// Read the XML record from a stream
	void update(const std::string&, const std::string&);
	void display(std::ostream&, const std::string&) const;
private:
	std::string name;
	int id;
	std::string address;
	std::string city;
	std::string state; 
	std::string country;
	std::string phone;
	double salary;
	void friend toRec(const Employee&, emprec&);
};
#endif // !EMPLOYEE_H

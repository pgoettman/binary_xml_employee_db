#include <string>
#include <cstddef>
#include <cstdlib>

#ifndef EMPREC_H
#define EMPREC_H
struct emprec {
	emprec() {
		memset(name, 0, 31);
		id = 0;
		memset(address, 0, 26);
		memset(city, 0, 21);
		memset(state, 0, 21);
		memset(country, 0, 21);
		memset(phone, 0, 21);
		salary = 0.0;
	}
	char name[31];
	int id;
	char address[26];
	char city[21];
	char state[21];
	char country[21];
	char phone[21];
	double salary;
};
#endif // !EMPREC_H

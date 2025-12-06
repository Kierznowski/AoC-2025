#include <fstream>
#include <iostream>
#include <vector>

int count_zeros(std::vector<std::string>& moves);
int count_new_method(std::vector<std::string>& moves);

int main() {

	//std::fstream file("test-input-6-1.txt");
	std::fstream file("input-1.txt");

	if (!file) {
		std::cerr << "File cannot be opened!" << std::endl;
		return 1;
	}
	
	std::vector<std::string> moves;

	std::string line;
	while(getline(file, line)) {
		moves.push_back(line);
	}
	file.close();

	int password = count_zeros(moves);
	int new_password = count_new_method(moves);

	std::cout << "The password is: " << password << '\n';
	std::cout << "The new password is: " << new_password << '\n';
	return 0;
}

int count_zeros(std::vector<std::string>& moves) {
	int pos = 50;
	int count {0};

	for (std::string move : moves) {
		char dir = move[0];
		if (dir == 'L') {
			pos -= stoi(move.substr(1));
		} else {
			pos += stoi(move.substr(1));
		}
		pos %= 100;
		if (pos == 0) count++;
	}

	return count;
}

int count_new_method(std::vector<std::string>& moves) {
	int pos = 50;
	int count = 0;

	for (std::string move : moves) {
		char dir = move[0];
		int distance = stoi(move.substr(1));

		for (int i = 0; i < distance; i++) {
			if (dir == 'L') {
				pos--;
				if (pos < 0) pos = 99;
			} else {
				pos++;
				if (pos > 99) pos = 0;
			}

			if (pos == 0) count++;
		}
	}
	return count;
}
// 6561
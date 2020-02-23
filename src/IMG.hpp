#pragma once

#include <string>
#include <fstream>
#include <map>

class IMG {
public:
	IMG(const std::string& file);

	std::istream& operator[](const std::string& ind);
	unsigned int getSize(const std::string& ind) const;

private:
	struct File {
		unsigned int offset;
		unsigned int size;
	};

	std::ifstream m_file;
	std::map<std::string, File> m_files;
};

#include <IMG.hpp>
#include <IO.hpp>
#include <vector>
#include <cstring>

struct Entry {
	unsigned int offset;
	unsigned short size;
	unsigned short pad;
	char name[24];
};

IMG::IMG(const std::string& file): m_file(file, std::ios::binary) {
	if (read<unsigned int>(m_file) != 0x32524556)
		throw std::runtime_error("file does not exist");

	std::vector<Entry> entries(read<unsigned int>(m_file));
	read(m_file, (char*) entries.data(), entries.size() * sizeof(Entry));

	for (auto& entry : entries) {
		m_files[std::string(entry.name, strnlen(entry.name, sizeof(entry.name)))] = {
			entry.offset * 2048, (unsigned int) entry.size * 2048
		};
	}
}

std::istream& IMG::operator[](const std::string& ind) {
	auto it = m_files.find(ind);

	if (it == m_files.end())
		throw std::runtime_error("file does not exist");

	// Clear all flags, so reading doesn't fail
	m_file.clear();

	return m_file.seekg(it->second.offset);
}

unsigned int IMG::getSize(const std::string& ind) const {
	auto it = m_files.find(ind);

	if (it == m_files.end())
		throw std::runtime_error("file does not exist");

	return it->second.size;
}

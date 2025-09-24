#pragma once

#include "fstream"
#include "iostream"

class STR {
	char* str = nullptr;
	size_t size = 0;
	static const char mark = '\0';
public:
	void SetSize(const size_t& Size);
	const size_t& GetSize() const;
	char& operator[](const size_t& Ind);
	bool operator==(STR& Str);
	//operator bool() const;
	friend std::ostream& operator<<(std::ostream& Os, STR& Str);
	char* begin() const;
	char* end() const;
	~STR();
};


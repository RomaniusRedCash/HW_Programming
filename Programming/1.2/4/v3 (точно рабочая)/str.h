#pragma once

#include "fstream"
#include "iostream"

class STR {
	char* str = nullptr;
	static const char mark = '\0';
	static const size_t const_size = 10;
	size_t size = 0;
public:
	STR();
	void Clear();
	bool PushEnd(const char& C);
	const size_t& GetSize();
	char& operator[](const size_t& Ind);
	bool operator==(STR& Str);
	bool operator!=(STR& Str);
	bool operator<(STR& Str);
	bool operator>(STR& Str);
	char* begin() const;
	char* end() const;
	~STR();
	//STR& operator=(STR& Str);
	//operator bool() const;
	friend std::ostream& operator<<(std::ostream& Os, STR& Str);
};
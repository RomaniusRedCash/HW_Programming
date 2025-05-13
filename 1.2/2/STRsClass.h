#pragma once

struct STR {
	char mark = '\0';
	char* str = nullptr;
	size_t GetSize();
	char& operator[] (const size_t& i);
	//STR& operator== (STR& Str);
	void SetSize(const size_t& Size);
	//bool AddNePov(const char C) {};
	//void CreateChild(STR& Str, size_t NowS) {};
	~STR();
};

class Screen {
	STR* strs = nullptr;
	size_t size = 0;
public:
	bool is_last = false;
	const size_t& GetSize();
	STR& operator[] (const size_t& i);
	void NewScr();
	void NewScr(const char& Mark, const size_t& Size);
	STR* begin(); 
	STR* end();
	~Screen();
};









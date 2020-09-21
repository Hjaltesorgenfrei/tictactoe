#include "pch.h"
#include <string>

TEST(FindsError, TooManyXorO) {
	std::stringstream buffer;
	std::streambuf* sbuf = std::cout.rdbuf();
	std::cout.rdbuf(buffer.rdbuf());
	EXPECT_EQ("ERROR", *sbuf);
	std::cout.rdbuf(sbuf);
}
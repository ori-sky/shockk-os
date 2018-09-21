#include <ctype.h>

int isalnum(int c) {
	return isalpha(c) || isdigit(c);
}

int isalpha(int c) {
	return isupper(c) || islower(c);
}

int isblank(int c) {
	switch(c) {
	case ' ':
	case '\t':
		return 1;
	default:
		return 0;
	}
}

int iscntrl(int c) {
	return c < 32;
}

int isdigit(int c) {
	switch(c) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		return 1;
	default:
		return 0;
	}
}

int isgraph(int c) {
	return isprint(c) && c != ' ';
}

int islower(int c) {
	switch(c) {
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':
		return 1;
	default:
		return 0;
	}
}

int isprint(int c) {
	return c >= 32 && c <= 127;
}

int ispunct(int c) {
	return isprint(c) && !isspace(c) && !isalnum(c);
}

int isspace(int c) {
	switch(c) {
	case ' ':
	case '\f':
	case '\n':
	case '\r':
	case '\t':
	case '\v':
		return 1;
	default:
		return 0;
	}
}

int isupper(int c) {
	switch(c) {
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
		return 1;
	default:
		return 0;
	}
}

int isxdigit(int c) {
	switch(c) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
		return 1;
	default:
		return 0;
	}
}

int tolower(int c) {
	switch(c) {
	case 'A':
		return 'a';
	case 'B':
		return 'b';
	case 'C':
		return 'c';
	case 'D':
		return 'd';
	case 'E':
		return 'e';
	case 'F':
		return 'f';
	case 'G':
		return 'g';
	case 'H':
		return 'h';
	case 'I':
		return 'i';
	case 'J':
		return 'j';
	case 'K':
		return 'k';
	case 'L':
		return 'l';
	case 'M':
		return 'm';
	case 'N':
		return 'n';
	case 'O':
		return 'o';
	case 'P':
		return 'p';
	case 'Q':
		return 'q';
	case 'R':
		return 'r';
	case 'S':
		return 's';
	case 'T':
		return 't';
	case 'U':
		return 'u';
	case 'V':
		return 'v';
	case 'W':
		return 'w';
	case 'X':
		return 'x';
	case 'Y':
		return 'y';
	case 'Z':
		return 'z';
	default:
		return c;
	}
}

int toupper(int c) {
	switch(c) {
	case 'a':
		return 'A';
	case 'b':
		return 'B';
	case 'c':
		return 'C';
	case 'd':
		return 'D';
	case 'e':
		return 'E';
	case 'f':
		return 'F';
	case 'g':
		return 'G';
	case 'h':
		return 'H';
	case 'i':
		return 'I';
	case 'j':
		return 'J';
	case 'k':
		return 'K';
	case 'l':
		return 'L';
	case 'm':
		return 'M';
	case 'n':
		return 'N';
	case 'o':
		return 'O';
	case 'p':
		return 'P';
	case 'q':
		return 'Q';
	case 'r':
		return 'R';
	case 's':
		return 'S';
	case 't':
		return 'T';
	case 'u':
		return 'U';
	case 'v':
		return 'V';
	case 'w':
		return 'W';
	case 'x':
		return 'X';
	case 'y':
		return 'Y';
	case 'z':
		return 'Z';
	default:
		return c;
	}
}

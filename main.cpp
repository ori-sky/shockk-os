#if defined(__cplusplus)
extern "C"
#endif

class Screen {
private:
	unsigned char * const ptr = nullptr;
public:
	Screen(unsigned char * const ptr) : ptr(ptr) {}
	Screen(const unsigned int address) : Screen(reinterpret_cast<unsigned char * const>(address)) {}

	inline void SetChar(const unsigned char row, const unsigned char column, const char c) {
		ptr[(row * 80 + column) << 1] = c;
	}
};

void entry(void) {
	Screen screen(0xB8000);
	for(unsigned char row = 0; row < 24; ++row) {
		for(unsigned char col = 0; col < 80; ++col) {
			screen.SetChar(row, col, 'A' + col % 26);
		}
	}
	for(;;);
}

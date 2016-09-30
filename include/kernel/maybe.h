#ifndef MAYBE_H
#define MAYBE_H

template<typename T> class Maybe {
private:
	const bool is_just;
	const T value;
public:
	Maybe(void)      : is_just(false), value(T{}) {}
	Maybe(const T x) : is_just(true),  value(x)   {}

	bool IsNothing(void) { return !this->is_just; }
	bool IsJust(void)    { return  this->is_just; }
	T FromJust(void) { return value; }
};

#endif

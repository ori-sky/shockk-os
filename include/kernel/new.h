#ifndef NEW_H
#define NEW_H

inline void *operator new  (size_t, void *p) throw() { return p; }
inline void *operator new[](size_t, void *p) throw() { return p; }

#endif

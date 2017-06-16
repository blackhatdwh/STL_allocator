#include <iostream>
#include <cstdlib>
#define CHAIN_LENGTH 10000
#define GAP 2000
#define LAYER 20
#define MAX_SIZE GAP*LAYER
// GAP * LAYER = 40000
using namespace std;


char* free_list[LAYER];


template <typename T>
class Allocator{
public:
	typedef T           value_type;
	typedef T*          pointer;
	typedef const T*    const_pointer;
	typedef T&          reference;
	typedef const T&    const_reference;
	typedef size_t      size_type;
	typedef ptrdiff_t   difference_type;
    template< typename U > struct rebind { typedef Allocator<U> other; };


	pointer allocate(size_type size = 1);
	void deallocate(pointer trash, size_type size = 1);
    size_type max_size() const noexcept;
	pointer address(reference x) const noexcept;
	const_pointer address(const_reference x) const noexcept;


    void Mount(int layer);      // mount blocks on given layer
    void Free(int layer);       // free all blocks on given layer

};
#include"Allocator.cpp"

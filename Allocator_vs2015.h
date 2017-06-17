#include <iostream>
#include <cstdlib>
#define CHAIN_LENGTH 200
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
	Allocator() noexcept {}
	template<class U> Allocator(const Allocator<U>&) noexcept {}
	pointer allocate(size_type size = 1) {
		size = sizeof(value_type) * size;
		if (size <= MAX_SIZE) {
			int layer = (size - 1) / GAP;
			if (free_list[layer] == nullptr) {
				Mount(layer);
			}
			T* result = (T*)(free_list[layer] + sizeof(char*));
			free_list[layer] = *(reinterpret_cast<char**>(free_list[layer]));
			return result;
		}
		else {
			return reinterpret_cast<pointer>(operator new (size));
		}
	}
	void deallocate(pointer trash, size_type size = 1) {
		size = size * sizeof(value_type);
		if (size <= MAX_SIZE) {
			char* recycle = (char*)trash - sizeof(char*);
			int layer = (size - 1) / GAP;
			*(reinterpret_cast<char**>(recycle)) = free_list[layer];
			free_list[layer] = recycle;
		}
		else {
			delete trash;
			return;
		}
	}
	size_type max_size() const noexcept {
		return (size_type)((size_type)-1 / (size_type)40000);

	}
	pointer address(reference x) const noexcept {
		return &x;
	}
	const_pointer address(const_reference x) const noexcept {
		return &x;
	}


	void Mount(int layer) {
		for (int i = 0; i < CHAIN_LENGTH; i++) {
			char* new_block = reinterpret_cast<char*>(operator new (sizeof(char*) + GAP * (layer + 1)));
			*(reinterpret_cast<char**>(new_block)) = free_list[layer];
			free_list[layer] = new_block;
		}
	}
	void Free(int layer) {
		while (free_list[layer]) {
			char* trash = free_list[layer];
			free_list[layer] = *reinterpret_cast<char**>(free_list[layer]);
			delete trash;
		}

	}
};

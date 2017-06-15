#include <iostream>
#include <cstdlib>
#define CHAIN_LENGTH 20
#define GAP 2000
#define LAYER 20
#define MAX_SIZE GAP*LAYER
// GAP * LAYER = 40000
using namespace std;

typedef char* slot_pointer;
typedef char** ptr2slot_pointer;

class Layer{
public:
    Layer();
    ~Layer();
    

public:
    slot_pointer current_block;
    slot_pointer current_slot;
    slot_pointer last_slot;
    slot_pointer free_list;
    size_t slot_size;
};
Layer class_layer[LAYER];

template <typename T>
class MemoryPool{
public:
	typedef T           value_type;
	typedef T*          pointer;
	typedef const T*    const_pointer;
	typedef T&          reference;
	typedef const T&    const_reference;
	typedef size_t      size_type;
	typedef ptrdiff_t   difference_type;
    template< typename U > struct rebind { typedef MemoryPool<U> other; };

    MemoryPool() noexcept;
    MemoryPool(const MemoryPool& memoryPool) noexcept;
    MemoryPool(MemoryPool&& memoryPool) noexcept;
    template <class U> MemoryPool(const MemoryPool<U>& memoryPool) noexcept;

	pointer allocate(size_type size = 1);
	void deallocate(pointer trash, size_type size = 1);
    size_type max_size() const noexcept;
	pointer address(reference x) const noexcept;
	const_pointer address(const_reference x) const noexcept;
    template <class U, class... Args> void construct(U* p, Args&&... args);
    template <class U> void destroy(U* p);

private:

    void Mount(int layer);

};
#include"mempool.cpp"

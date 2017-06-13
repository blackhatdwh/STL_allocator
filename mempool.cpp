#include <iostream>
#include <cstdlib>
using namespace std;


template <typename T>
MemoryPool<T>::MemoryPool() noexcept{
    current_block = nullptr;
    current_slot = nullptr;
    last_slot = nullptr;
    free_list = nullptr;
    size_t object_size = sizeof(value_type);
    slot_size = (((object_size - 1) / 8) + 1) * 8;
}


template <typename T>
MemoryPool<T>::~MemoryPool() noexcept{
    while(current_block){
        slot_pointer temp = current_block;
        current_block = *reinterpret_cast<ptr2slot_pointer>(current_slot);
        free(temp);
    }
}

template <typename T>
inline typename MemoryPool<T>::pointer
MemoryPool<T>::address(reference x)
const noexcept{
    return &x;
}

template <typename T>
inline typename MemoryPool<T>::const_pointer
MemoryPool<T>::address(const_reference x)
const noexcept{
    return &x;
}

template <typename T>
inline typename MemoryPool<T>::pointer
MemoryPool<T>::allocate(size_type size){
    if(free_list != nullptr){
        pointer result = reinterpret_cast<pointer>(free_list);
        free_list = *reinterpret_cast<ptr2slot_pointer>(free_list);
        return reinterpret_cast<pointer>(result);
    }
    else{
        if(current_slot >= last_slot){
            allocateBlock();
        }
        current_slot += slot_size;
        return reinterpret_cast<pointer>(current_slot - slot_size);
    }
}

template <typename T>
inline void
MemoryPool<T>::deallocate(pointer trash, size_type size){
    if(trash != nullptr){
        *reinterpret_cast<ptr2slot_pointer>(trash) = free_list;
        free_list = reinterpret_cast<slot_pointer>(trash);
    }
}

template <typename T>
inline void
MemoryPool<T>::allocateBlock(){
    slot_pointer new_block = (slot_pointer)malloc(BLOCKSIZE);   // apply for a new block
    current_slot = new_block + slot_size;      // the first slot in the new block is used to link this block with the previous block
    *reinterpret_cast<ptr2slot_pointer>(new_block) = current_block;     // set the link information
    current_block = new_block;
    last_slot = new_block + BLOCKSIZE - slot_size + 1;
}

#include <iostream>
#include <cstdlib>
#define CHAIN_LENGTH 20
#define GAP 2000
#define LAYER 20
#define MAX_SIZE GAP*LAYER
// GAP * LAYER = 40000

using namespace std;


template <typename T>
MemoryPool<T>::MemoryPool() noexcept{
    class_layer = new Layer[LAYER];
}

template <typename T>
MemoryPool<T>::MemoryPool(const MemoryPool& memoryPool) noexcept : MemoryPool(){}


template <typename T>
template<class U>
MemoryPool<T>::MemoryPool(const MemoryPool<U>& memoryPool)
noexcept :
MemoryPool()
{}

//TODO
template <typename T>
MemoryPool<T>::~MemoryPool() noexcept{
    for(int i = 0; i < LAYER; i++){
        while(class_layer[i].current_block){
            slot_pointer temp = class_layer[i].current_block;
            class_layer[i].current_block = *reinterpret_cast<ptr2slot_pointer>(class_layer[i].current_block);
            free(temp);
        }
    }
    delete[] class_layer;
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
inline typename MemoryPool<T>::size_type
MemoryPool<T>::max_size()
const noexcept
{
  size_type max_blocks = -1 / (size_type)40000;
  return max_blocks;
}


template <typename T>
inline typename MemoryPool<T>::pointer
MemoryPool<T>::allocate(size_type size){
    if(size > MAX_SIZE){
        return (pointer)malloc(size);
    }
    int layer = (size - 1) / GAP;
    // if free_list is available, use it
    if(class_layer[layer].free_list != nullptr){
        pointer result = reinterpret_cast<pointer>(class_layer[layer].free_list);
        class_layer[layer].free_list = *reinterpret_cast<ptr2slot_pointer>(class_layer[layer].free_list);
        // store which layer does this slot belong to
        *(reinterpret_cast<int*>(result)) = layer;
        // skip the information section
        slot_pointer final_result = reinterpret_cast<slot_pointer>(result) + sizeof(slot_pointer);
        return reinterpret_cast<pointer>(final_result);
    }
    else{
        if(class_layer[layer].current_slot >= class_layer[layer].last_slot){
            Mount(layer);
        }
        slot_pointer result = class_layer[layer].current_slot;
        *(reinterpret_cast<int*>(result)) = layer;
        class_layer[layer].current_slot += class_layer[layer].slot_size;
        result += sizeof(slot_pointer);
        return reinterpret_cast<pointer>(result);
    }

}

template <typename T>
inline void
MemoryPool<T>::deallocate(pointer trash, size_type size){
    if(size > MAX_SIZE){
        free(trash);
        return;
    }
    // retrieve the skipped information section
    slot_pointer recycle = reinterpret_cast<slot_pointer>(trash) - sizeof(slot_pointer);
    // figure out which layer does it belong to
    int layer = *reinterpret_cast<int*>(recycle);
    // create link
    *reinterpret_cast<ptr2slot_pointer>(recycle) = class_layer[layer].free_list;
    class_layer[layer].free_list = recycle;
}

template <typename T>
inline void
MemoryPool<T>::Mount(int layer){
    // the size of one memory slot
    // + sizeof(slot_pointer) means add a space at the beginning of the slot, showing which layer it comes from
    class_layer[layer].Set_Slot_Size(sizeof(slot_pointer) + GAP * (layer + 1));
    // + sizeof(slot_pointer) means add a slot_pointer at the beginning of the chunk, used to point to the next chunk
    slot_pointer new_block = (slot_pointer)malloc(sizeof(slot_pointer) + CHAIN_LENGTH * class_layer[layer].slot_size);
    // allocate start from the head of the new chunk (the first slot_pointer was skipped)
    class_layer[layer].current_slot = new_block + sizeof(slot_pointer);
    // set up the link between chunks
    *reinterpret_cast<ptr2slot_pointer>(new_block) = class_layer[layer].current_block;
    class_layer[layer].current_block = new_block;
    // the last slot is here:
    class_layer[layer].last_slot = new_block + sizeof(slot_pointer) + (CHAIN_LENGTH - 1) * class_layer[layer].slot_size + 1;
}




template <typename T>
template <class U, class... Args>
inline void
MemoryPool<T>::construct(U* p, Args&&... args)
{
  new (p) U (std::forward<Args>(args)...);
}



template <typename T>
template <class U>
inline void
MemoryPool<T>::destroy(U* p)
{
  p->~U();
}

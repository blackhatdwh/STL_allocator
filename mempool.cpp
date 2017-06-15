#include <iostream>
#include <cstdlib>


using namespace std;

long long int trace = 0;

Block::Block(size_t size){
    next = nullptr;
    block = (char*)malloc(size);
    id = 12345;
    cout ;
}

Block::~Block(){
    free(block);
}

template <typename T>
MemoryPool<T>::MemoryPool() noexcept{
}

template <typename T>
MemoryPool<T>::MemoryPool(const MemoryPool& memoryPool) noexcept : MemoryPool(){}


template <typename T>
template<class U>
MemoryPool<T>::MemoryPool(const MemoryPool<U>& memoryPool)
noexcept :
MemoryPool()
{}


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
    size = sizeof(value_type) * size;
    if(size > MAX_SIZE){
        return (pointer)malloc(size);
    }
    int layer = (size - 1) / GAP;
    if(free_list[layer] == nullptr){
        Mount(layer);
    }
    T* result = (T*)(free_list[layer]->block);
    free_list[layer] = (*free_list[layer]).next;
    return result;
}

template <typename T>
inline void
MemoryPool<T>::deallocate(pointer trash, size_type size){
    size = size * sizeof(value_type);
    if(size > MAX_SIZE){
        free(trash);
        return;
    }
    else{
        Block* recycle = (Block*)trash;
        int layer = (size - 1) / GAP;
        recycle->next = free_list[layer];
        free_list[layer] = recycle;
    }
}

template <typename T>
inline void
MemoryPool<T>::Mount(int layer){
    for (int i = 0; i < CHAIN_LENGTH; i++){
        Block* temp = new Block(GAP * (layer + 1));
        temp->id = trace++;
        temp->next = free_list[layer];
        free_list[layer] = temp;
    }
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

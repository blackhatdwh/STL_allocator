using namespace std;

template <typename T>
inline typename Allocator<T>::const_pointer
Allocator<T>::address(const_reference x)
const noexcept{
    return &x;
}


template <typename T>
inline typename Allocator<T>::size_type
Allocator<T>::max_size()
const noexcept {
  return (size_type)( -1 / (size_type)40000);
}

template <typename T>
inline typename Allocator<T>::pointer
Allocator<T>::allocate(size_type size){
    size = sizeof(value_type) * size;
    if(size <= MAX_SIZE){
        int layer = (size - 1) / GAP;
        if(free_list[layer] == nullptr){
            Mount(layer);
        }
        T* result = (T*)(free_list[layer] + sizeof(char*));
        free_list[layer] = *(reinterpret_cast<char**>(free_list[layer]));
        return result;
    }
    else{
        return reinterpret_cast<pointer>(operator new (size));
    }
}

template <typename T>
inline void
Allocator<T>::deallocate(pointer trash, size_type size){
    size = size * sizeof(value_type);
    if(size <= MAX_SIZE){
        char* recycle = (char*)trash - sizeof(char*);
        int layer = (size - 1) / GAP;
        *(reinterpret_cast<char**>(recycle)) = free_list[layer];
        free_list[layer] = recycle;
    }
    else{
        delete trash;
        return;
    }
}

template <typename T>
inline void
Allocator<T>::Mount(int layer){
    for (int i = 0; i < CHAIN_LENGTH; i++){
        char* new_block = reinterpret_cast<char*>(operator new (sizeof(char*) + GAP * (layer + 1)));
        *(reinterpret_cast<char**>(new_block)) = free_list[layer];
        free_list[layer] = new_block;
    }
}

template <typename T>
inline void
Allocator<T>::Free(int layer){
    while(free_list[layer]){
        char* trash = free_list[layer];
        free_list[layer] = *reinterpret_cast<char**>(free_list[layer]);
        delete trash;
    }
}


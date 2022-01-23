#ifndef CLASS_VECTOR_H
#define CLASS_VECTOR_H

template <typename T> class Vector 
{
    // arr is the integer pointer which stores the address of our vector
    T* arr;
    // capacity is the total storage capacity of the vector
    int capacity;
    // current is the number of elements currently present in the vector
    int current;
  public:
    // Default constructor to initialise an initial capacity of 1 element and allocating storage using dynamic allocation
    Vector()
    {
      arr = new T[1];
      capacity = 1;
      current = 0;
    }
    // Function to add an element at the last
    void push_back(T data)
    {
      // if the number of elements is equal to the capacity, that means we don't have space to accommodate more elements. We need to double the capacity
      if (current == capacity) {
        T* temp = new T[2 * capacity];
        
        // copying old array elements to new array
        for (int i = 0; i < capacity; i++) {
          temp[i] = arr[i];
        }
  
        // deleting previous array
        delete[] arr;
        capacity *= 2;
        arr = temp;
      }
  
      // Inserting data
      arr[current] = data;
      current++;
    }
  
    // function to extract element at any index
    T getElement(int index)
    {
      // if index is within the range
      if (index < current)
            return arr[index];
    }
  
    // function to delete last element
    void pop() { current--; }
  
    // function to get size of the vector
    int size() { return current; }
  
    // function to get capacity of the vector
    int getcapacity() { return capacity; }
  
    // function to get the first element
    T front() { return arr[0]; }
  
    // function to get the last element
    T back() { return arr[current]; }
};


/*
template <typename T> class Vector 
{
    // arr is the integer pointer which stores the address of our vector
    T* arr;
    // capacity is the total storage capacity of the vector
    int capacity;
    // current is the number of elements currently present in the vector
    int current;
  public:
    // Default constructor to initialise an initial capacity of 1 element and allocating storage using dynamic allocation
    Vector()
        {
      arr = new T[1];
      capacity = 1;
      current = 0;
        }

    // Function to add an element at the last
    void push_back(T data);
  
    // function to extract element at any index
    T getElement(int index);
  
    // function to delete last element
    void pop();
  
    // function to get size of the vector
    int size();
  
    // function to get capacity of the vector
    int getcapacity();
  
    // function to get the first element
    T front();
  
    // function to get the last element
    T back();
};*/

#endif //CLASS_VECTOR_H

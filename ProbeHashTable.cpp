/*****************************************
 ** File:    ProbeHashTable.cpp
 ** Project: CMSC 341 Project 5 - Hash Table Strategies & Performance, Spring 2018
 ** Author:  Innocent Kironji
 ** Date:    05/15/18
 ** Section: 03      
 ** E-mail:  wambugu1@umbc.edu
 **
 **   This file contains the implementation of the ProbeHashTable and its
 **    helper class HashTableEntry.
 **   HashTableEntry contains information about the entries (to help with operations such
 **     as lazy deletion). ProbeHashTable is a hash table that deals with collision
 **     through linear probing
 ** 
 ***********************************************/
#ifndef PROBEHASHTABLE_CPP
#define PROBEHASHTABLE_CPP

#include "ProbeHashTable.h"
#include <stdexcept>
#include <iostream>

// Default Constructor for a hash table entry
template <typename T>
ProbeHashTable<T>::HashTableEntry::HashTableEntry(){
  m_flag = m_EMPTY;
}

// Overloaded constructor for probe hash table
// n has a default value of 101
template <typename T>
ProbeHashTable<T>::ProbeHashTable(unsigned int (*hashFunc)(const T& key), int n){
  m_hashFunc = hashFunc;
  m_capacity = n;
  m_size = 0;
  m_hashTab = new ProbeHashTable<T>::HashTableEntry[n];

  // Initializing all the items in hash table
  for (int i = 0; i < n; i++){
    ProbeHashTable<T>::HashTableEntry temp;
    m_hashTab[i] = temp;
  }
}

// Probe's Hash Table Destructor
template <typename T>  
ProbeHashTable<T>::~ProbeHashTable(){
  delete [] m_hashTab;
}

// Probe's Copy Constructor
template <typename T>
ProbeHashTable<T>::ProbeHashTable (ProbeHashTable& other){
  m_hashFunc = other.m_hashFunc;
  m_capacity = other.m_capacity;
  m_size = other.m_size;
  m_hashTab = new ProbeHashTable<T>::HashTableEntry[m_capacity];

  // Initializing all the items in hash table
  for (int i = 0; i < m_capacity; i++){
    m_hashTab[i] = other.m_hashTab[i];
  }
}

// Probe's Overloaded Assignment Operator
template <typename T>
const ProbeHashTable<T>& ProbeHashTable<T>::operator= (ProbeHashTable& rhs){
  // Checking for self-assignment
  if (this == &rhs)
    return *this;

  // Resetting member variables
  delete [] m_hashTab;
  m_hashFunc = rhs.m_hashFunc;
  m_capacity = rhs.m_capacity;
  m_size = rhs.m_size;
  m_hashTab = new ProbeHashTable<T>::HashTableEntry[m_capacity];

  // Initializing all the items in hash table
  for (int i = 0; i < m_capacity; i++){
    m_hashTab[i] = rhs.m_hashTab[i];
  }
  return *this;
}

// Inserts data into hash table
// Uses linear probing
// If duplicate data then returns false (no insertion occurs)
// Returns whether or not insertion occured
template <typename T>
bool ProbeHashTable<T>::insert(const T &data){
  bool added = false;
  int value = m_hashFunc(data) % m_capacity;
  int curr = (value + m_VAL) % m_capacity;

  // Exception thrown if adding to a full table
  if (m_size == m_capacity)
    throw std::out_of_range("Probe Hash Table is full. Cannot add new data");

  // No duplicates allowed
  if( (m_hashTab[value].m_flag == m_VAL) && (m_hashTab[value].m_data == data) )
    return added;

  // No Collision
  if (m_hashTab[value].m_flag != m_VAL){
    m_size++;
    m_hashTab[value].m_data = data;
    m_hashTab[value].m_flag = m_VAL;
    m_hashTab[value].m_hashVal = value;
    added = true;
    return added;
  }

  // If a collision occurs
  // Loops until an empty position is found or until circles back around to beginning
  while( (m_hashTab[curr].m_flag == m_VAL) && (curr != value)){
    
    // No duplicates allowed
    if (m_hashTab[curr].m_data == data)
      return added;

    curr = (curr + m_VAL) % m_capacity;
  }

  // Collision circled around to being
  if (curr != value){
    m_size++;
    m_hashTab[curr].m_data = data;
    m_hashTab[curr].m_flag = m_VAL;
    m_hashTab[curr].m_hashVal = value;
    added = true;
  }
  return added;
}

// Search Function
// Finds specified data in hash table
// Uses linear probing if not in correct position
template <typename T>
bool ProbeHashTable<T>::find(const T &data){
  bool found = false;
  int value = m_hashFunc(data) % m_capacity;
  int curr = (value + m_VAL) % m_capacity;

  // Data is found in the correct location
  if( (m_hashTab[value].m_flag == m_VAL) && (m_hashTab[value].m_data == data) ){
    found = true;
    return found;
  }

  // If a collision occurs
  // Loops until an empty position is found or until circles back around to beginning
  while( (m_hashTab[curr].m_flag != m_EMPTY) && (curr != value) ){

  // Data was found while looping
  if( (m_hashTab[curr].m_flag == m_VAL) && (m_hashTab[curr].m_data == data) ){
    found = true;
    return found;
  }
    curr = (curr + m_VAL) % m_capacity;
  }

  // If an empty index is reached then the data does not exist
  return found;
}

// Falsely removes data from table (lazy deletion)
// In actuality sets a flag that determines if data exists
// Returns the data that was "removed" and updates found if removal occured
template <typename T>
T ProbeHashTable<T>::remove(const T &data, bool &found){
  T removed;
  found = false;
  int value = m_hashFunc(data) % m_capacity;
  int curr = (value + m_VAL) % m_capacity;

  // Data is found in the correct location
  if( (m_hashTab[value].m_flag == m_VAL) && (m_hashTab[value].m_data == data) ){
    found = true;
    m_size--;
    m_hashTab[value].m_flag = m_DEL;
    removed = m_hashTab[value].m_data;
    return removed;
  }

  // If a collision occurs
  // Loops until an empty position is found or until circles back around to beginning
  while( (m_hashTab[curr].m_flag != m_EMPTY) && (curr != value) ){
    
    // Data was found while looping
    if( (m_hashTab[curr].m_flag == m_VAL) && (m_hashTab[curr].m_data == data) ){
      found = true;
      m_size--;
      m_hashTab[curr].m_flag = m_DEL;
      removed = m_hashTab[curr].m_data;
      return removed;
    }
    curr = (curr + m_VAL) % m_capacity;
  }
  
  // If an empty index is reached then the data does not exist
  return removed;
}

// Debugging Function that prints out the contents of the table
template <typename T>
void ProbeHashTable<T>::dump(){
  std::cout << "============================================================" << std::endl;
  std::cout << "ProbeHashTable dump; size: " << m_capacity << " buckets" << std::endl;

  // Loops through the entire array
  for (int i = 0; i < m_capacity; i++){
    std::cout << "[" << i << "]: ";

    // Certain information is output based on the flags
    switch (m_hashTab[i].m_flag)
      {
      case m_DEL:
	std::cout << "DELETED" << std::endl;
	break;
      case m_EMPTY:
	std::cout << "EMPTY" << std::endl;
	break;
      default:
	std::cout << m_hashTab[i].m_data << " (" 
		  << m_hashTab[i].m_hashVal << ")" << std::endl;
      }
  }
  std::cout << "Total items: " << m_size << std::endl;
  std::cout << "============================================================" << std::endl;
}

// Grading Function that checks if an index is occupied
// Returns the number of items found (either 0 or 1) and appends
//  that item to a given vector
template <typename T>
int ProbeHashTable<T>::at(int index, std::vector<T> &contents){
  int numItems = 0;

  // Checking for valid index
  if( (index >= m_capacity) || (index < 0) )
    throw std::out_of_range("Index does not exist in Heap");

  // Checks if the element ecists
  if (m_hashTab[index].m_flag == m_VAL){
    contents.push_back(m_hashTab[index].m_data);
    numItems++;
  }
  return numItems;
}
#endif

/*****************************************
 ** File:    ChainHashTable.cpp
 ** Project: CMSC 341 Project 5 - Hash Table Strategies & Performance, Spring 2018
 ** Author:  Innocent Kironji
 ** Date:    05/15/18
 ** Section: 03      
 ** E-mail:  wambugu1@umbc.edu
 **
 **   This file contains the implementation of ChainHashTable which is a hash table
 **     that handles collisions through chainning. Chainning is done using the STL's
 **     list class (treated as a forward or singlely-linked list)
 ** 
 ***********************************************/
#ifndef CHAINHASHTABLE_CPP
#define CHAINHASHTABLE_CPP

#include "ChainHashTable.h"
#include <stdexcept>
#include <iostream>

// Overloaded Constructor
// n has a default value of 101
template <typename T>
ChainHashTable<T>::ChainHashTable(unsigned int (*hashFunc)(const T& key), int n){

  m_capacity = n;
  m_total = 0;
  m_hashFunc = hashFunc;
  m_hashTab = new std::list<T>[n];
  
  // Initializing all the values of the array
  for (int i = 0; i < n; i++){
    std::list<T> temp;
    m_hashTab[i] = temp;
  }
}

// Destructor
template <typename T>
ChainHashTable<T>::~ChainHashTable(){
  delete [] m_hashTab;
}

// Copy Constructor
template <typename T>
ChainHashTable<T>::ChainHashTable (ChainHashTable& other){

  m_capacity = other.m_capacity;
  m_total = other.m_total;
  m_hashFunc = other.m_hashFunc;
  m_hashTab = new std::list<T>[m_capacity];
  
  // Initializing all the values of the array
  for (int i = 0; i < m_capacity; i++){
    std::list<T> temp;

    // Loops through other's linked list copying data
    for (typename std::list<T>::iterator it = other.m_hashTab[i].begin(); 
	 it != other.m_hashTab[i].end(); ++it){
      temp.push_back(*it);
    }

    // Adds the complete linked list to the hash table
    m_hashTab[i] = temp;
  }
}

// Overloaded Assignment Operator
template <typename T>
const ChainHashTable<T>& ChainHashTable<T>::operator= (ChainHashTable& rhs){

  // Checking for self-assignment
  if (this == &rhs)
    return *this; 

  // Resetting member variables
  delete [] m_hashTab; 
  m_capacity = rhs.m_capacity;
  m_total = rhs.m_total;
  m_hashFunc = rhs.m_hashFunc;
  m_hashTab = new std::list<T>[m_capacity];

  // Initializing all the values of the array
  for (int i = 0; i < m_capacity; i++){
    std::list<T> temp;

    // Loops through other's linked list copying data
    for (typename std::list<T>::iterator it = rhs.m_hashTab[i].begin(); 
	 it != rhs.m_hashTab[i].end(); ++it){
      temp.push_back(*it);
    }

    // Adds the complete linked list to the hash table
    m_hashTab[i] = temp;
  }
  return *this;
}

// Adds elements to the hash table
// Returns a boolean that says if the data was sucessfully added or not
template <typename T>
bool ChainHashTable<T>::insert(const T &data){
  const int EMPTY = 0;  
  bool added = false;
  int value = m_hashFunc(data) % m_capacity;

  // Looks for duplicate
  // Immediately returns if duplicate is found
  if( findAt(data, value) )  
    return added;

  // There was no duplicate
  added = true;
  m_total++;
  m_hashTab[value].push_front(data);
  return added;
}

// Finds a specified element in the hash table
// Returns a boolean determing if it was found
template <typename T>
bool ChainHashTable<T>::find(const T &data){
  bool found = false;
  int value = m_hashFunc(data) % m_capacity;

  // Looks for data in chained list
  if( findAt(data, value) )
    found = true;

  // Data was not found
  return found;
}

// Removes a specified data from the hash table
// Returns the data and updates the boolean
// in the parameters depening on if it was found or not
template <typename T>
T ChainHashTable<T>::remove(const T &data, bool &found){  
  T removed;
  found = false;
  int value = m_hashFunc(data) % m_capacity;

  // Checks if the data was removed properly
  if( removeAt(data, value, removed) ){
    found = true;
    m_total--;
  }
  return removed;
}

// Function to assist with debugging
// Prints out the entire hash table
template <typename T>
void ChainHashTable<T>::dump(){

  std::cout << "============================================================" << std::endl;
  std::cout << "ChainHashTable dump; size: " << m_capacity << " buckets" << std::endl;

  // Loops through all the elements in the hash table
  for (int i = 0; i < m_capacity; i++){

    std::cout << "[" << i << "]: ";

    // Checks if that element is holding any data
    if (m_hashTab[i].size() != 0){

      // Loops through the linked list (at an index in the table)
      for (typename std::list<T>::iterator it = m_hashTab[i].begin(); 
	   it != m_hashTab[i].end(); ++it){
	std::cout << *it << ", ";
      }
    }
    std::cout << "" << std::endl;
  }
  std::cout << "Total items: " << m_total << std::endl;
  std::cout << "============================================================" << std::endl;
}

// Grading function
// Stores all the chained elements at a specific index in hash table
template <typename T>
int ChainHashTable<T>::at(int index, std::vector<T> &contents){
  int numItems = 0;

  // Checks if valid index
  if( (index >= m_capacity) || (index < 0) )
    throw std::out_of_range("Index does not exist in Heap");

  // Loops through the linked list
  for (typename std::list<T>::iterator it = m_hashTab[index].begin(); 
       it != m_hashTab[index].end(); ++it){

    // Updates return values
    numItems++;
    contents.push_back(*it);
  }
  return numItems;
}

// Helper Function for insert and find
// Finds data within a specific linked list
template <typename T>
bool ChainHashTable<T>::findAt(const T& data, int loc){
  bool found = false;
  // Loops through the linked list
  for (typename std::list<T>::iterator it = m_hashTab[loc].begin(); 
       it != m_hashTab[loc].end(); ++it){

    // Data was found, exit function
    if (*it == data){
      found = true;
      return found;
    }
  }
  return found;
}

// Helper Function for removing a node within a specific linked list
template <typename T>
bool ChainHashTable<T>::removeAt(const T& data, int loc, T& removed_data){
  bool removed = false;

  // Loops through the linked list
  for (typename std::list<T>::iterator it = m_hashTab[loc].begin(); 
       it != m_hashTab[loc].end(); ++it){

    // Data was found, exit function
    if (*it == data){
      removed = true;

      // Data is deleted
      removed_data = *it;
      m_hashTab[loc].erase(it);

      return removed;
    }
  }
  return removed;
}

#endif

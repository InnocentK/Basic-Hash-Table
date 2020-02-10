/*****************************************
 ** File:    ChainHashTable.h
 ** Project: CMSC 341 Project 5 - Hash Table Strategies & Performance, Spring 2018
 ** Author:  Innocent Kironji
 ** Date:    05/15/18
 ** Section: 03      
 ** E-mail:  wambugu1@umbc.edu
 **
 **   This file contains the declaration for the ChainHashTable class
 **   This class is a hash table that handles collisions by using chainning
 ** 
 ***********************************************/
#ifndef CHAINHASHTABLE_H
#define CHAINHASHTABLE_H

#include "HashTable.h"
#include <list>

template <typename T>
class ChainHashTable : public HashTable<T> 
{
 public:

  // Required functions
  ChainHashTable(unsigned int (*hashFunc)(const T& key), int n=101);  
  virtual ~ChainHashTable();
  ChainHashTable (ChainHashTable& other);
  const ChainHashTable& operator= (ChainHashTable& rhs);
  virtual bool insert(const T &data);
  virtual bool find(const T &data);
  virtual T remove(const T &data, bool &found);  

  // Testing/ Debugging Functions
  virtual void dump();
  virtual int at(int index, std::vector<T> &contents);

 private:

  // Function pointer for hash function
  unsigned int (*m_hashFunc)(const T& key);

  // Hash table
  std::list<T>* m_hashTab;

  // Member variables
  int m_capacity;
  int m_total;

  // Helper Functions
  virtual bool findAt(const T& data, int loc);
  virtual bool removeAt(const T& data, int loc, T& removed_data);
};

#include "ChainHashTable.cpp"
#endif

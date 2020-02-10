/*****************************************
 ** File:    ProbeHashTable.h
 ** Project: CMSC 341 Project 5 - Hash Table Strategies & Performance, Spring 2018
 ** Author:  Innocent Kironji
 ** Date:    05/15/18
 ** Section: 03      
 ** E-mail:  wambugu1@umbc.edu
 **
 **   This file contains the declaration of the ProbeHashTable class and
 **    its helper class HashTableEntry. This class creates a hash table that deals
 **    with collision using linear probing
 ** 
 ***********************************************/
#ifndef PROBEHASHTABLE_H
#define PROBEHASHTABLE_H

#include "HashTable.h"

template <typename T>
class ProbeHashTable : public HashTable<T> 
{
 public:
  // Required Functions
  ProbeHashTable(unsigned int (*hashFunc)(const T&), int n=101);  
  virtual ~ProbeHashTable();
  ProbeHashTable (ProbeHashTable& other);
  const ProbeHashTable& operator= (ProbeHashTable& rhs);
  virtual bool insert(const T &data);
  virtual bool find(const T &data);
  virtual T remove(const T &data, bool &found);

  // Grading/ debugging functions
  virtual void dump();
  virtual int at(int index, std::vector<T> &contents);

  // Nested Class to store data on elements of the hash table
  class HashTableEntry{
  public:
    HashTableEntry();
    int m_flag;
    T m_data;
    int m_hashVal;
  };

 private:
  // Function pointer
  unsigned int (*m_hashFunc)(const T& key);

  // Hash Table
  HashTableEntry* m_hashTab;

  // Member variables
  int m_capacity;
  int m_size;

  // Constants to denote the different possible flags for a hash table entry
  static const int m_EMPTY = 0;
  static const int m_DEL = -1;
  static const int m_VAL = 1;
};

#include "ProbeHashTable.cpp"
#endif

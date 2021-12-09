// $Id: VectorClass.h,v 1.1 2011/07/09 08:16:21 kpettit1 Exp $

/***************************************************************
 *                FLU - FLTK Utility Widgets 
 *  Copyright (C) 2002 Ohio Supercomputer Center, Ohio State University
 *
 * This file and its content is protected by a software license.
 * You should have received a copy of this license with this file.
 * If not, please contact the Ohio Supercomputer Center immediately:
 * Attn: Jason Bryan Re: FLU 1224 Kinnear Rd, Columbus, Ohio 43212
 * 
 ***************************************************************/



#ifndef _FLU_VECTOR_CLASS_H
#define _FLU_VECTOR_CLASS_H

#define MakeVectorClass( T, C ) \
class C \
{ \
public: \
 \
  C() { _array = NULL; _size = 0; } \
 \
  ~C() { clear(); } \
 \
  inline void add( const T& item ) { insert( size(), item ); } \
 \
  inline T& operator [](int i) { return _array[i]; } \
 \
  inline T operator [](int i) const { return _array[i]; } \
 \
  inline unsigned int size() const { return _size; } \
 \
  C& operator =( const C &v ) \
  { \
    clear(); \
    if( v.size() ) \
      { \
	_array = new T[v.size()]; \
	for( unsigned int i = 0; i < v.size(); i++ ) \
	  _array[i] = v._array[i]; \
      } \
    return *this; \
  } \
 \
  void insert( unsigned int pos, const T &item ) \
  { \
    if( pos > _size ) \
      pos = _size; \
    if( _size == 0 ) \
      { \
	_array = new T[1]; \
      } \
    else \
      { \
	if( !( _size & (_size-1) ) ) \
	  { \
	    T* temp = new T[_size*2]; \
	    for( unsigned int i = 0; i < _size; i++ ) \
	      temp[i] = _array[i]; \
	    delete[] _array; \
	    _array = temp; \
	  } \
	for( unsigned int s = _size; s > pos; s-- ) \
	  _array[s] = _array[s-1]; \
      } \
    _size++; \
    _array[pos] = item; \
  } \
 \
  void erase( unsigned int pos ) \
  { \
    if( pos >= _size ) \
      return; \
    _size--; \
    if( _size == 0 ) \
      { \
	delete[] _array; \
	_array = NULL; \
      } \
    else \
      { \
	for( ; pos < _size; pos++ ) \
	  _array[pos] = _array[pos+1]; \
      } \
  } \
 \
  void clear() \
  { \
    if( _array ) \
      delete[] _array; \
    _size = 0; \
  } \
 \
protected: \
  T *_array; \
  unsigned int _size; \
}

#endif

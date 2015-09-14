#pragma once
#include "stdafx.h"

template<typename T>
class Iterator
{
protected:
	typename T::iterator _begin;
	typename T::iterator _end;
	typename T::iterator _current;
	Iterator(typename T::iterator begin, typename T::iterator end, typename T::iterator current);
public:
	Iterator();
	Iterator(const Iterator& value);
	virtual ~Iterator();
	Iterator& operator++();
	Iterator operator++(int);
	Iterator& operator--();
	Iterator operator--(int);
	virtual Iterator& operator=(const Iterator& value);
	virtual bool operator==(Iterator& value);
	virtual bool operator!=(Iterator& value);
};

template<typename T>
Iterator<T>::Iterator()
{}
template<typename T>
Iterator<T>::Iterator(typename T::iterator begin, typename T::iterator end, typename T::iterator current)
	: _begin(begin), _end(end), _current(current)
{}
template<typename T>
Iterator<T>::Iterator(const Iterator& value)
	: _begin(value._begin), _end(value._end), _current(value._current)
{}
template<typename T>
Iterator<T>::~Iterator()
{}
template<typename T>
Iterator<T>& Iterator<T>::operator++()
{
	++_current;
	return *this;
}
template<typename T>
Iterator<T> Iterator<T>::operator++(int)
{
	auto tmp = *this;
	++_current;
	return tmp;
}
template<typename T>
Iterator<T>& Iterator<T>::operator--()
{
	--_current;
	return *this;
}
template<typename T>
Iterator<T> Iterator<T>::operator--(int)
{
	auto tmp = *this;
	--_current;
	return tmp;
}
template<typename T>
Iterator<T>& Iterator<T>::operator=(const Iterator& value)
{
	this->_current = value._current;
	this->_begin = value._begin;
	this->_end = value._end;
	return *this;
}
template<typename T>
bool Iterator<T>::operator==(Iterator& value)
{
	if (this->_current != value._current
		|| this->_begin != value._begin
		|| this->_end != value._end)
	{
		return false;
	}
	else
	{
		return true;
	}
}
template<typename T>
bool Iterator<T>::operator!=(Iterator& value)
{
	return !(operator==(value));
}
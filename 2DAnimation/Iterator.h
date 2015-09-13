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
	Iterator(const Iterator& value);
	virtual ~Iterator();
	Iterator& operator++();
	Iterator operator++(int);
	Iterator& operator--();
	Iterator operator--(int);
	virtual Iterator& operator=(const Iterator& value);
	virtual bool operator==(PointIterator& value);
	virtual bool operator!=(PointIterator& value);
};

template<typename T>
Iterator<T>::Iterator(typename T::iterator begin, typename T::iterator end, typename T::iterator current)
	: _begin(begin), _end(end), _current(current)
{}
template<typename T>
Iterator<T>::Iterator(const Iterator& value)
	: _begin(value._begin), _end(value._end), _current(value._current)
{}
template<typename T>
virtual Iterator<T>::~Iterator()
{
	_begin = nullptr;
	_end = nullptr;
	_current = nullptr;
}
template<typename T>
virtual Iterator<T>& Iterator<T>::operator++()
{
	if (_current != _end)
	{
		do
		{
			++_current;
		} while (_current != _end && !qualifier(*_current));
	}
	return *this;
}
template<typename T>
virtual Iterator<T> Iterator<T>::operator++(int)
{
	auto tmp = *this;
	if (_current != _end)
	{
		do
		{
			++_current;
		} while (_current != _end && !qualifier(*_current));
	}
	return tmp;
}
template<typename T>
virtual Iterator<T>& Iterator<T>::operator--()
{
	if (_current != _begin)
	{
		--_current;
		while (!qualifier(*_current))
		{
			if (_current == _begin)
			{
				_current = _end;
				break;
			}
			--_current;
		}
	}
}
template<typename T>
virtual Iterator<T> Iterator<T>::operator--(int)
{
	auto tmp = *this;
	if (_current != _begin)
	{
		--_current;
		while (!qualifier(*_current))
		{
			if (_current == _begin)
			{
				_current = _end;
				break;
			}
			--_current;
		}
	}
	return tmp;
}
template<typename T>
virtual Iterator& Iterator<T>::operator=(const Iterator& value)
{
	return Iterator(value);
}
template<typename T>
virtual bool Iterator<T>::operator==(PointIterator& value)
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
virtual bool Iterator<T>::operator!=(PointIterator& value)
{
	return !(operator==(value));
}
#pragma once
#include <iostream>
#include <string>


class Tree {
public:
	virtual ~Tree() = default;
	
	virtual bool search(int value) = 0;

	virtual void insert(int value) = 0;

	virtual void remove(int value) = 0;

	virtual std::string toString() const = 0;
};
#pragma once

#include "DoubleNode.h"

template<class T>
class DoubleLinkedList {
private:
	DoubleNode<T>* head;
	DoubleNode<T>* tail;

	int length;

public:
	DoubleLinkedList() :
		head(nullptr), tail(nullptr), length(0)
	{}
	DoubleLinkedList(const DoubleLinkedList <T>& list) :
		DoubleLinkedList()
	{
		DoubleNode<T>* ptr = list.head;

		while (ptr != nullptr)
		{
			//Append

			ptr = ptr->Next();
		}
	}
public:
	void Append(T item)
	{
		DoubleNode<T>* newNode = new DoubleNode<T>(item, tail, nullptr);

		if (length == 0) {
			tail = newNode;
			head = tail;
		}
		else {
			tail->SetNext(newNode);
			tail = newNode;
		}
		++length;
	}
	void Prepend(T item)
	{
		DoubleNode<T>* newNode = new DoubleNode<T>(item, nullptr, head);

		if (length == 0)
			tail = newNode;
		else
			head->SetPrev(newNode);

		head = newNode;

		++length;
	}
public:
	DoubleNode<T>* Head()
	{
		return head;
	}
	DoubleNode<T>* Tail()
	{
		return tail;
	}
	int GetLength() const
	{
		return length;
	}
	int IsEmpty() const
	{
		return head == nullptr;
	}
public:
	void Remove(DoubleNode<T>* toRemove)
	{
		DoubleNode<T>* prev = toRemove->Prev();
		DoubleNode<T>* next = toRemove->Next();

		if (toRemove == head)
			head = next;

		if (toRemove == tail)
			tail = prev;

		if(prev != nullptr)
			prev->SetNext(next);
		if (next != nullptr)
			next->SetPrev(prev);

		toRemove->SetNext(nullptr);
		delete(toRemove);
	}
	void MoveToStart(DoubleNode<T>* toMove)
	{
		if (toMove != head)
		{
			DoubleNode<T>* prev = toMove->Prev();
			DoubleNode<T>* next = toMove->Next();

			if (toMove == tail)
				tail = prev;

			if (prev != nullptr)
				prev->SetNext(next);
			if (next != nullptr)
				next->SetPrev(prev);

			toMove->SetNext(head);
			toMove->SetPrev(nullptr);
			head->SetPrev(toMove);

			head = toMove;
		}
	}


};
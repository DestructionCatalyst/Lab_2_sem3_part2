#pragma once

template<class T> class DoubleNode {

private:
	T content;
	DoubleNode<T>* prev;
	DoubleNode<T>* next;

public:
	DoubleNode(T newContent) :
		content(newContent), prev(nullptr), next(nullptr)
	{
	}
	DoubleNode(T newContent, DoubleNode* newPrev, DoubleNode* newNext) :
		content(newContent), prev(newPrev), next(newNext)
	{
	}
	DoubleNode(DoubleNode& node) :
		DoubleNode(node.content, node.next)
	{
	}
public:
	T& GetContent()
	{
		return content;
	}
	DoubleNode<T>* Prev()
	{
		return prev;
	}
	DoubleNode<T>* Next()
	{
		return next;
	}
public:
	void SetContent(T newContent)
	{
		content = newContent;
	}
	void SetPrev(DoubleNode* newPrev)
	{
		prev = newPrev;
	}
	void SetNext(DoubleNode* newNext)
	{
		next = newNext;
	}
public:
	~DoubleNode()
	{
		delete(next);
	}
};
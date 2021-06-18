#include <stdio.h>
#include <vector>

class Node
{
public:
	int rank;
	int value;
	int hunters;
	Node* next;
public:
	Node(int r = 0, int v = 0, int h = 0)
	: rank(r), value(v), hunters(h) {}
};

class Queue
{
private:
	Node* mTop = nullptr;
	
private:
	void mAdd(Node data, Node*& node)
	{
		if (node == nullptr)
		{
			node = new Node;
			node->rank = data.rank;
			node->value = data.value;
			node->hunters = data.hunters;
			node->next = nullptr;
			return;
		}
		
		if ((node->value < data.value) || (node->value == data.value && node->rank < data.rank))
		{
			mAdd(data, node->next);
			return;
		}
		
		if ((node->value > data.value) || (node->value == data.value && node->rank > data.rank))
		{	
			int tempRank = node->rank;
			int tempValue = node->value;
			int tempHunters = node->hunters;
			Node* tempNext = node->next;
			
			node->rank = data.rank;
			node->value = data.value;
			node->hunters = data.hunters;
			node->next = new Node;
			node->next->rank = tempRank;
			node->next->value = tempValue;
			node->next->hunters = tempHunters;
			node->next->next = tempNext;
			return;
		}
	}
	
	void mRemove(int rank, Node*& node)
	{
		if (node == nullptr)
			return;
		
		if (node->rank == rank)
		{
			Node* shift = node->next;
			delete node;
			node = shift;
			return;	
		}
		
		mRemove(rank, node->next);
	}
	
	void mPrint(Node* node)
	{
		if (node == nullptr)
			return;
		printf("rank: %d, value: %d, hunters: %d\n", node->rank, node->value, node->hunters);
		mPrint(node->next);
	}
	
	void mAbove(int rank, Node* node, std::vector<Node>& result)
	{
		if (node == nullptr || node->rank == rank)
			return;
		
		result.push_back(Node(node->rank, node->value, node->hunters));
		
		mAbove(rank, node->next, result);
	}
	
public:
	void add(Node data)
	{
		mAdd(data, mTop);
	}
	
	void add(int rank, int value, int hunters = 0)
	{
		mAdd(Node(rank, value, hunters), mTop);
	}
	
	void remove(int rank)
	{
		mRemove(rank, mTop);
	}
	
	void print()
	{
		mPrint(mTop);
	}
	
	std::vector<Node> above(int rank)
	{
		std::vector<Node> result;
		mAbove(rank, mTop, result);
		return result;
	}
};
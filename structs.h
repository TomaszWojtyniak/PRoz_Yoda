#include <stdlib.h>
#include <stdio.h>
//#include <vector>


// struct Data
// {
// 	int id;
// 	int priority;
// };

// class Position
// {
// public:
// 	int id;
// 	int priority;
// 	Position* next;
	
// public:
// 	void set(int newId, int newPriority, Position* newNext = nullptr)
// 	{
// 		id = newId;
// 		priority = newPriority;
// 		next = newNext;
// 	}
	
// 	Position(int id = 0, int priority = 0, Position* next = nullptr)
// 	{
// 		this->id = id;
// 		this->priority = priority;
// 		this->next = next;
// 	}
// };

// class Queue
// {
// private:
// 	Position* first = nullptr;
	
// public:
// 	Position*& getFirst()
// 	{
// 		return first;	
// 	}
	
// 	void insert(int id, int priority, Position*& p)
// 	{
// 		if (!p)
// 		{
// 			p = new Position(id, priority, nullptr);
// 			return;
// 		}
		
// 		if ((p->priority < priority) || (p->priority == priority && p->id < id))
// 		{
// 			insert(id, priority, p->next);
// 			return;
// 		}
		
// 		if ((p->priority > priority) || (p->priority == priority && p->id > id))
// 		{	
// 			Position temp = *p;
// 			p->set(id, priority);
// 			p->next = new Position;
// 			*(p->next) = temp;
			
// 			return;
// 		}
// 	}
	
// 	void remove(int id, Position*& p)
// 	{
// 		if (!p)
// 			return;
		
// 		if (p->id == id)
// 		{
// 			Position* temp = p->next;
// 			delete p;
// 			p = temp;
// 			return;	
// 		}
		
// 		remove(id, p->next);
// 	}
	
// 	void removeAll(Position*& p)
// 	{
// 		if (!p)
// 			return;
		
// 		removeAll(p->next);
		
// 		Position* temp = p->next;
// 		delete p;
// 		p = temp;
// 	}
	
// 	void print(Position* p)
// 	{
// 		if (p == first)
// 			printf("--------------------\n");
		
// 		if (first == nullptr)
// 			printf("empty\n");
		
// 		if (!p)
// 		{
// 			printf("--------------------\n");
// 			return;
// 		}
		
// 		printf("id=%d | priority=%d\n", p->id, p->priority);
		
// 		print(p->next);
// 	}
	
// 	void getBetterVector(int id, std::vector<Data>& better, Position* p)
// 	{
// 		if (!p || p->id == id)
// 			return;
		
// 		Data data;
// 		data.id = p->id;
// 		data.priority = p->priority;
		
// 		better.push_back(data);
		
// 		getBetterVector(id, better, p->next);
// 	}
// };

// int main()
// {
// 	Queue queue;
	
// 	queue.insert(3, 100, queue.getFirst());
// 	queue.insert(4, 300, queue.getFirst());
// 	queue.insert(1, 200, queue.getFirst());
// 	queue.insert(3, 400, queue.getFirst());
// 	queue.insert(0, 400, queue.getFirst());
// 	queue.insert(4, 400, queue.getFirst());
	
// 	queue.print(queue.getFirst());
	
// 	queue.remove(1, queue.getFirst());
// 	queue.remove(4, queue.getFirst());
	
// 	queue.print(queue.getFirst());
	
// 	queue.removeAll(queue.getFirst());
	
// 	queue.print(queue.getFirst());
	
// 	queue.insert(3, 400, queue.getFirst());
// 	queue.insert(0, 400, queue.getFirst());
// 	queue.insert(4, 400, queue.getFirst());
// 	queue.insert(3, 100, queue.getFirst());
// 	queue.insert(4, 300, queue.getFirst());
// 	queue.insert(1, 200, queue.getFirst());
	
// 	queue.print(queue.getFirst());
	
// 	std::vector<Data> better;
// 	int id = 0;
// 	queue.getBetterVector(id, better, queue.getFirst());
// 	printf("ALL PROCESSES ABOVE PROCESS WITH ID %d:\n", id);
// 	for (int i = 0; i < better.size(); i++)
// 	{
// 		printf("id=%d, prio=%d\n", better[i].id, better[i].priority);
// 	}
	
// 	return 0;	
// }

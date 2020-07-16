#include <iostream>

using namespace std;

struct Node {
	int val;
	Node* next;
	Node(int v) {
		val = v;
		next = nullptr;
	}
};

Node* reverseList(Node* list, Node* head) {
	if (list == nullptr) {
		return head;
	}
	else if (list->next == nullptr) {
		list->next = head;
		return list;
	}
	else {
		Node* temp = list->next;
		list->next = head;
		Node* node = reverseList(temp, list);
		return node;
	}
}


Node* reverseList(Node* list)
{
	if (list == nullptr) return nullptr;
	Node* temp = list->next;
	list->next = nullptr;
	return reverseList(temp, list);
}


int main_rList()
{
	Node* three= new Node(3);
	Node* two = new Node(2);
	Node* one = new Node(1);
	three->next = two;
	two->next = one;
	one->next = nullptr;
	auto list = reverseList(three);

	while (list != nullptr) {
		cout << list->val << endl;
		list = list->next;
	}
	return 0;
}
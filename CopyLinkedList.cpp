/*

Result:

Original linked list:
this val = 3    random val = 5
this val = 10   random val = 3
this val = 5    random val = 10

Copied linked list:
this val = 3    random val = 5
this val = 10   random val = 3
this val = 5    random val = 10

*/

#include <iostream>
#include <map>

class Node {
public:
	Node(int v) : val(v) {}
	Node(Node* node);
	void SetNext(Node* n) { next = n; }
	void SetRandom(Node* r) { rand = r; }
	void Print(void);
	static Node* CopyList(Node* head);

private:
	int val;
	Node* next;
	Node* rand;
};

using node_map = std::map<Node*, Node*>;

Node::Node(Node* node) {
	val = node->val;
	next = nullptr;
	rand = node;	// reference; we'll fix this up later
}

Node* Node::CopyList(Node* head) {
	Node* result = nullptr;

	if (head) {
		node_map nodes;

		result = new Node(head);
		Node* copy_node = result;
	
		Node* org_node = head;
		nodes[org_node] = copy_node;	// map original node to its copy
		org_node = org_node->next;

		while (org_node) {
			copy_node->next = new Node(org_node);	// create the copy of the original node
			nodes[org_node] = copy_node->next;		// map original node to its copy
			org_node = org_node->next;
			copy_node = copy_node->next;
		}

		// fixup references to random node by finding mapped value of old nodes random node
		copy_node = result;
		while (copy_node) {
			Node* org_rand = copy_node->rand->rand;	// copy_node->rand was where we temporarily stashed the reference the copies original node
			Node* new_rand = nodes.find(org_rand)->second;	// get new node that was mapped to old node
			copy_node->rand = new_rand;
			copy_node = copy_node->next;
		}
	}

	return result;
}

void Node::Print(void) {
	for (Node* n = this; n; n = n->next)
		std::cout << "this val = " << n->val << "\trandom val = " << n->rand->val << std::endl;
}

int main()
{
	std::cout << "Original linked list:\n";
	Node a(3);
	Node b(10);
	Node c(5);

	Node* head = &a;

	a.SetNext(&b);
	b.SetNext(&c);
	a.SetRandom(&c);
	b.SetRandom(&a);
	c.SetRandom(&b);

	head->Print();

	std::cout << "\nCopied linked list:\n";
	Node* copy = Node::CopyList(head);
	copy->Print();
}

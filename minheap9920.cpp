/*	Haard Shah	-	cs610 9920 prp*/

#include<iostream>
#include<climits>
#include<map>
#include<vector>

using namespace std;

struct node {
	unsigned char word = 0;
	int freq = 0;

	node *left, *right;

	void init_node (char w, int f, node *l, node *r){
		// cout << "CUSTOM INITIALIZER USED." << endl;
		this->left = l;
		this->right = r;
		this->word = w;
		this->freq = f;
	}

	void print_node () {
		cout << "(int)char: " << (int)word << "\t: " << "freq: " << freq << endl;
	}
	
};

void swap(node **x, node **y);

class MinHeap
{
	node **harr; // pointer to array of elements in heap
	std::vector<node *> nvector;
	int capacity; // maximum possible size of min heap
	int heap_size; // Current number of elements in min heap
public:
	// Constructor
	MinHeap(int capacity);

	// to heapify a subtree with root at given index
	void MinHeapify(int );

	int getHeapSize() {
		return heap_size;
	}

	int parent(int i) { 
		if ((i-1)/2 >= 0) {
			return (i-1)/2;
		}
		else {
			// printf("Asking for root node's parent.\n");
			// printf("Returning itself.\n");
			return i;
		}
	}

	// to get index of left child of node at index i
	int left(int i) { 
		if ((2*i + 1) < heap_size) { 
			return (2*i + 1); 
		}
		else {
			// printf("Node has no child. Returning itself...\n");
			return i;
		}
	}

	// to get index of right child of node at index i
	int right(int i) { 
		int return_val = 2*i + 2;
		if (return_val < heap_size) {
			return return_val;
		}
		else {
			// cout << "Node has no right child. Returning itself..." << endl;
			return i;
		}
	}

	// to extract the root which is the minimum element
	node *extractMin();
	bool canExtractMin();

	void print_heap() {
		cout << "--------------------------Printing MinHeap-------------------------" << endl;
		// for (int i = 0; i < heap_size; i++) {
		// 	cout << harr[i] << ":\t"; // prints the address of the node
		// 	harr[i]->print_node();
		// }
		int nsize = nvector.size();
		for (int i = 0; i < nsize; i++) {
			cout << nvector[i] << ":\t"; // prints address
			nvector[i]->print_node();
		}
		cout << "-------------------------------------------------------------------" << endl;
	}
	
	//	insert new key 
	void insertKey(unsigned char data, int frequency, node *left, node *right){
		if (heap_size == capacity) {
			cout << "\nOverflow: Could not insertKey\n";
			return;
		}

		// cout << "Before inserting" << endl;
		// print_heap();

		// First insert the new key at the end
		heap_size++;
		int i = heap_size - 1;
		node *new_node;
		new_node = (node *) malloc (sizeof(node));
		// cout << "address: " << new_node << endl;

		new_node->init_node(data, frequency, left, right);
		nvector.push_back(new_node);

		// harr[i] = new_node;

		// cout << "Inserting node:" << endl;
		// harr[i]->print_node();

		// cout << "After inserting" << endl;
		// print_heap();

		// Fix the min heap property if it is violated
		// while (i != 0 && harr[parent(i)]->freq > harr[i]->freq) {
		// 	// cout << "Before swap" << endl;
		// 	// print_heap();
		// 	swap(&harr[i], &harr[parent(i)]);
		// 	// cout << "After swap" << endl;
		// 	// print_heap();
		// 	i = parent(i);
		// }
		while (i != 0 && nvector[parent(i)]->freq > nvector[i]->freq) {
			// swap
			node *tmp = nvector[parent(i)];
			nvector[parent(i)] = nvector[i];
			nvector[i] = tmp;
			i = parent(i);
		}

		// cout << "At the end" << endl;
		// print_heap();
	}
};
 
// Constructor: Builds a heap from a given array a[] of given size
MinHeap::MinHeap(int cap)
{
	heap_size = 0;
	capacity = cap; 
	// harr = (node **)new unsigned int[cap]; 
	nvector.reserve(cap);
}
 
// Method to remove minimum element (or root) from min heap
node *MinHeap::extractMin()
{
	if (heap_size <= 0) {
	// node err;
	// err.freq = INT_MAX;
		return NULL;
	}
	if (heap_size == 1) {
		heap_size--;
		// return harr[0];
		node *return_val = nvector[0];
		nvector.erase(nvector.begin());
		return return_val;
	}

	// Store the minimum value, and remove it from heap
	// swap(&harr[0], &harr[heap_size-1]);
	// node *return_val = harr[0];
	// harr[0] = harr[heap_size-1]; // replace root node with the last node
	node *return_val = nvector[0];
	nvector.front() = nvector.back();
	nvector.pop_back();

	heap_size--;                 // reduce size;
	MinHeapify(0);

	// return harr[heap_size]; // return last item which was previously first (therefore min)
	return return_val;
}
 
bool MinHeap::canExtractMin() {
	if (nvector.empty()) {
		return false;
	}
	return true;
}
 
// A recursive method to heapify a subtree with root at given index
// This method assumes that the subtrees are already heapified
void MinHeap::MinHeapify(int i)
{
	int l = left(i);
	int r = right(i);
	int smallest = i;
	if (l < nvector.size() && nvector[l]->freq < nvector[i]->freq)
		smallest = l;
	if (r < nvector.size() && nvector[r]->freq < nvector[smallest]->freq)
		smallest = r;
	if (smallest != i)
	{
		// swap(&harr[i], &harr[smallest]);
		node *tmp = nvector[i];
		nvector[i] = nvector[smallest];
		nvector[smallest] = tmp;
		
		MinHeapify(smallest);
	}
}
 
// A utility function to swap two elements
// void swap(node **x, node **y) {
// 	node *tmp = *x;
// 	*x = *y;
// 	*y = tmp;
// }

bool isLeaf(node *x) {
	return (!(x->left) && !(x->right));
}

void print_vector(vector<bool> *single) {
	cout << "------Printing Single:  "<< single << "-------------" << endl;

	for (int i = 0; i < single->size(); i++) {
		cout << single->at(i) << " ";
	}
	cout << endl;
	cout << "---------------------------------------------------" << endl;
}

// Huffman coding function
void getCodes(node* root, map<unsigned char, vector<bool>> *codes, vector<bool> *single) {
	// print_vector(single);

	if (isLeaf(root)) {
		codes->insert(std::pair<unsigned char, vector<bool>>(root->word, *single));
		single->pop_back();
		return;
	}

	if (root->left) {
		single->push_back(false);
		getCodes(root->left, codes, single);
	}

	if (root->right) {
		single->push_back(true);
		getCodes(root->right, codes, single);
	}
	single->pop_back();
}

int getTotalUnique(unsigned int *freq) {
	int count = 0;
	for (int i = 0; i < BYTE_MAX; i++) 
		if (freq[i]) count++;
	return count;
}

void getHuffmanCodes(unsigned int *freq, map<unsigned char, vector<bool>> *codes, vector<bool> *single) {
	//*	2. Build minHeap
#ifdef DEBUG_MODE
	// print_nonzero_freq(freq);
	cout << "Building MinHeap..." << endl;
#endif

	MinHeap heap(getTotalUnique(freq));
	for (int i = 0; i < BYTE_MAX; i++) {
		if (freq[i]) {
			// cout << "Before insert" << endl;
			// heap.print_heap();
			heap.insertKey((unsigned char)i, freq[i], NULL, NULL);
			// cout << "After insert" << endl;
			// heap.print_heap();
		}
	}

#ifdef DEBUG_MODE
	heap.print_heap();
#endif

//*	3. Run huffman
#ifdef DEBUG_MODE
	cout << "Running Huffman..." << endl;
#endif

	while (heap.getHeapSize() != 1) {
		node *left, *right;
		// heap.print_heap();
		if (heap.canExtractMin()) {
			left = heap.extractMin();
			right = heap.extractMin();
		}
		else {
#ifdef DEBUG_MODE
			cout << "Error: found empty heap" << endl;
#endif
			break;
		}
#ifdef DEBUG_MODE
		cout << "L Extracted:\t";
		left->print_node();
		cout << "R Extracted:\t";
		right->print_node();
#endif
		unsigned char data = (unsigned char)'$';
		int frequency = left->freq + right->freq;

		heap.insertKey(data, frequency, left, right);
	}

//*	4. Traverse binary tree
	node *root;
	if (heap.canExtractMin()) {
		root = heap.extractMin();
	} else {
		cout << "Unexpected: either input file was empty or heap could not be created." << endl;
		return;
	}

	getCodes(root, codes, single);
}

void printCodes(const map<unsigned char, vector<bool>> *codes) {
	for (auto& code: *codes) {
		cout << (int)code.first << "\t:\t" << code.second.size() << endl; 
		for (int i = 0; i < code.second.size(); i++) {
			cout << code.second[i];
		}
		cout << endl;
	}
}

void printDecodingCodes(const map<string, unsigned char> *codes) {
	for (auto& code: *codes) {
		cout << code.first << "\t\t: " << (int)code.second << endl;
	}
}

int getBiggestCodeSize(const map<string, unsigned char> *codes) {
	int max = 0;
	for (auto& code: *codes) {
		if (code.first.length() > max) max = code.first.length();
	}
	return max;
}

void swapCodeKeyValue(const map<unsigned char, vector<bool>> *codes, map<string, unsigned char> *newc) {
	for (auto& code: *codes) {
		string code_string = "";
		for (int i = 0; i < code.second.size(); i++) {
			if (code.second[i] == true) {
				code_string += "1";
			}
			else {
				code_string += "0";
			}
		}
		std::pair<string, unsigned char> tmp_pair(code_string, code.first);
		newc->insert(tmp_pair);
	}
}

void printCodes2(node *root, string str) {
	cout << "str: " << &str << ", " << str << endl;

	if (!root)
        return;
 
    if (root->word != '$')
        cout << (int)root->word << ": " << str << "\n";
 
    printCodes2(root->left, str + "0");
    printCodes2(root->right, str + "1");
}
























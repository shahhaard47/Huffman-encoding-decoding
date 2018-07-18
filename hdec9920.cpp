/*	Haard Shah	-	cs610 9920 prp*/

#include<fstream>
#include<map>
#include<vector>

using namespace std;

#define BYTE_MAX 256

// #define DEBUG_MODE

#include "minheap9920.cpp"

bool isHuffmanEncoded(string inputFile);

// assumes isHuffmanEncoded test is run before 
// therefore, also assumes string size is more than 4 
string removeExtension(string inputFile);

void read_data_into_vector(unsigned char data, vector<char> *byte_buffer);

void read_bits_into_vector(unsigned char data, vector<char> *byte_buffer, int numbits);

int main(int argc, char const *argv[])
{
	if (argc != 2) {
		printf("Usage: %s <filename>\n", argv[0]);
		return -1;
	}

	string inputFile = argv[1];
	if (!isHuffmanEncoded(inputFile)) {
		cout << "Error: Cannot decode file. Only .huf file extensions allowed as input." << endl;
		return -1;
	}
	string outputFile = /*"d_" +*/ removeExtension(inputFile);

	//open file
	ifstream inFile(inputFile, ifstream::in | ifstream::binary);
	if (!inFile) {
		cout << "Error: Could not open " << inputFile << endl;
		return -1;
	}

	// get frequencies 256 ints (1024 bytes)
	unsigned int freq[BYTE_MAX];
	for (int i = 0; i < BYTE_MAX; i++) {
		unsigned int temp = 0;
		inFile.read((char *) & temp, sizeof(temp));
		freq[i] = temp;
		// cout << "Read: " << temp << endl; // works
	}

	// get size of compressed data in bits
	unsigned int compressed_bit_size = 0;
	inFile.read((char *) & compressed_bit_size, sizeof(compressed_bit_size));

	// cout << "compressed size: " << compressed_bit_size << endl; // matches

	unsigned int full_bytes = compressed_bit_size / 8;
	unsigned int last_bits = compressed_bit_size % 8;

	// Run huffman and get codes
	map<unsigned char, std::vector<bool>> temp_codes;
	vector<bool> single;
	getHuffmanCodes(freq, &temp_codes, &single);

	// cout << "Size codes: " << temp_codes.size() << endl;
	// printCodes(&temp_codes); // working

	//switch key value of map codes and change vector representation to string representation
	map<string, unsigned char> codes;
	swapCodeKeyValue(&temp_codes, &codes);

	//check 
	// printDecodingCodes(&codes); //works

	int biggest_code = getBiggestCodeSize(&codes);
	if (!biggest_code) return -1; // should never happen

	//*	Now START decoding file
	ofstream outFile(outputFile, ofstream::out | ofstream::binary);
	int i = 0;
	std::vector<char> byte_buffer;
	string curr_code = "";
	int count = 0;
	while (i++ < full_bytes) {
		char buff;
		inFile.read(&buff, 1);
		unsigned char data = (unsigned char)buff;
		// cout << ++count << ". Read: " << (unsigned int) data << endl;
		read_data_into_vector(data, &byte_buffer);
		while (!byte_buffer.empty()) {
			curr_code += byte_buffer.front();
			auto it = codes.find(curr_code);
			if (it != codes.end()) { // found a match
				unsigned char c = it->second;
				outFile.write((char *) & c, sizeof(unsigned char));
				// cout << "Decoded character: " << (unsigned int) c << endl;
				curr_code = "";
			}
			else if (curr_code.length() > biggest_code) {
				cout << i << ". curr len: " << curr_code.length();
				cout << "; biggest_code: " << biggest_code << endl;
				cout << "curr_code: " << curr_code << endl;
				// cout << "Unexpected behavior occured while decoding file" << endl;
				return -1; // shouldn't happen
			}

			byte_buffer.erase(byte_buffer.begin());
		}
	}
	if (last_bits) {
		char buff;
		inFile.read(&buff, 1);
		unsigned char data = (unsigned char)buff;
		// cout << ++count << ". Read: " << (unsigned int) data << endl;
		read_bits_into_vector(data, &byte_buffer, last_bits);
		while (!byte_buffer.empty()) {
			curr_code += byte_buffer.front();
			auto it = codes.find(curr_code);
			if (it != codes.end()) { // found a match
				unsigned char c = it->second;
				outFile.write((char *) & c, sizeof(unsigned char));
				curr_code = "";
			}
			else if (curr_code.length() > biggest_code) {
				cout << i << ". curr len: " << curr_code.length();
				cout << "; biggest_code: " << biggest_code << endl;
				cout << "curr_code: " << curr_code << endl;
				// cout << "Unexpected behavior occured while decoding file" << endl;
				return -1; // shouldn't happen
			}

			byte_buffer.erase(byte_buffer.begin());
		}
		if (curr_code.length() != 0) {
			cout << "Unexpected error decoding the end of file" << endl;
		}
	}
	else { // if there are no remaining bits curr_code shouldn't have any bits
		if (curr_code.length() != 0 || !byte_buffer.empty()) {
			cout << "Unexpected error decoding the end of file" << endl;
			return -2;
		}
	}

	// close files
	if (inFile) inFile.close();

	cout << "Decoded file is stored as " << outputFile << endl;

	return 0;
}

void read_data_into_vector(unsigned char data, vector<char> *byte_buffer) {
	read_bits_into_vector(data, byte_buffer, 8);
}

void read_bits_into_vector(unsigned char data, vector<char> *byte_buffer, int numbits) {
	vector<char> tmp;
	int i = 0;
	while (i++ < 8) {
		if (data&1) tmp.insert(tmp.begin(), '1');
		else tmp.insert(tmp.begin(), '0');
		// tmp.insert(tmp.begin(), data&1);
		// cout << "data: " << (int)data << endl;
		data = data >> 1;
	}
	// insert tmp at the end of bytebuffer
	for (int i = 0; i < numbits; i++) {
		byte_buffer->push_back(tmp[i]);
	}
}

string removeExtension(string inputFile) {
	return inputFile.substr(0, inputFile.size() - 4);
}

bool isHuffmanEncoded(string inputFile) {
	int size = inputFile.length();
	if (size <= 4) return false;
	if (inputFile.substr(size - 4) == ".huf")
		return true;
	return false;
}




















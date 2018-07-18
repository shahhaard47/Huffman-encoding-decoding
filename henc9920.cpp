/*	Haard Shah	-	cs610 9920 prp*/

#include<fstream>
#include<map>
#include<vector>

using namespace std;

#define BYTE_MAX 256
#define uint unsigned int
// #define DEBUG_MODE

#include "minheap9920.cpp"

// returns bytes
int filesize(const string filename);

void print_nonzero_freq(const unsigned int *freq);

unsigned int get_compressed_size(const unsigned int *freq, map<unsigned char, vector<bool>> *codes);

void write_byte(vector<bool> *byte_buffer, ofstream *outFile);

int encodeFile(string fileName);

int main(int argc, char const *argv[]){
	if (argc != 2) {
		printf("Usage: %s <filename>\n", argv[0]);
		return -1;
	}

	int i = 0;
	while (i++ < 1) {
		string fileName = argv[1];
		encodeFile(fileName);
	}
}

int encodeFile(string fileName) {
	string inputFile = fileName;
	string outputFile = inputFile + ".huf";

	// frequencies - arrIdx is the character (0-255) and the value is frequency
	unsigned int freq[BYTE_MAX];

	//initialzie to zero
	for (int i = 0; i < BYTE_MAX; i++) freq[i] = 0;
	
	//open file
	int inputFileSize = filesize(inputFile);
	ifstream inFile(inputFile, ios::in | ios::binary);

#ifdef DEBUG_MODE
	cout << "Reading file and getting frequency..." << endl;
	cout << "inputFileSize: " << inputFileSize << endl;
#endif

//*	1. read file & get frequency 
	for (int i = 0; i < inputFileSize; i++){
		char buff;
		inFile.read(&buff, 1);
		unsigned char data = (unsigned char) buff;
		unsigned int byte_data = (unsigned int) data;
		// cout << "read byte: " << byte_data << endl;
		freq[byte_data]++;
	}
	inFile.close();


	map<unsigned char, vector<bool>> codes;
	vector<bool> single;
	// getCodes(root, &codes, &single);

//*	3.	Run huffman encoding
//*	4.	Traverse the binary tree and get binary codes for each nonzero "character"
	getHuffmanCodes(freq, &codes, &single);

	// cout << "Size codes: " << codes.size() << endl;
	// printCodes(&codes);
#ifdef DEBUG_MODE
	cout << "Size codes: " << codes.size() << endl;
	printCodes(&codes);
	// cout << "-----------------Other method print nodes---------------------" << endl;
	// printCodes2(root, "");
#endif

//* 5.	Create new file "argv[1].huf"
	ofstream outFile(outputFile, ofstream::out | ofstream::binary);
	inFile.open(inputFile, ifstream::in | ifstream::binary);

//*	6.	Write the frequencies to argv (BYTE_MAX, 4 byte integers OR array of only nonzero characters with size at the beginning)
	// first 1024 bytes (256*4)
	for (int i = 0; i < BYTE_MAX; i++) {
		unsigned int f = freq[i];
		outFile.write((char *) & f, sizeof(f)); // FIXME: writes in such a wierd way (may have problems reading this later. Check https://www.geeksforgeeks.org/readwrite-class-objects-fromto-file-c/ for help on how to read)
		// cout << "wrote: " << f << endl;
	}

//*	7.	Encode the rest of the data after
	// first write the total size of the compressed data in bytes (unsigned int) should be fine
	// next 4 bytes (unsigned int)
	unsigned int compressed_size = get_compressed_size(freq, &codes);
	// cout << "compressed_size: " << compressed_size << endl;
#ifdef DEBUG_MODE
	cout << "compressed_size: " << compressed_size << endl;
#endif
	outFile.write((char *) & compressed_size, sizeof(compressed_size));

	// now encode the data
	std::vector<bool> temp_byte_buffer;
	for (int i = 0; i < inputFileSize; i++){
		char buff;
		inFile.read(&buff, 1);
		unsigned char data = (unsigned char) buff;
		// cout << "Current character: " << (unsigned int) data << endl;
		auto it = codes.find(data);
		if (it != codes.end()) { // should always be true!
			int size = it->second.size();
#ifdef DEBUG_MODE
			cout << "code size: " << size << endl;
			cout << "buffer size: " << temp_byte_buffer.size() << endl;
#endif
			for (int j = 0; j < size; j++) {
				bool bit = it->second[j];
				temp_byte_buffer.push_back(bit);
				if (temp_byte_buffer.size() == 8) {
					write_byte(&temp_byte_buffer, &outFile);
					temp_byte_buffer.clear();
				}
			}
		} else {
			cout << "Unexpected error: cannot find encoding for " << (int)data << endl;
		}
	}
	inFile.close();
	if (!temp_byte_buffer.empty()) {
		write_byte(&temp_byte_buffer, &outFile);
	}
	temp_byte_buffer.clear();

	// close files
	if (inFile) inFile.close();
	if (outFile) outFile.close();

	cout << "Huffman encoding complete. Compressed file is stored in '" << outputFile << "'." << endl;

	return 0;
}

void write_byte(vector<bool> *byte_buffer, ofstream *outFile) {
	unsigned char data = 0;
	int size = byte_buffer->size();
	static unsigned int count = 0;
	for (int i = 0; i < 8; i++) {
		if (i < size) {
			if (byte_buffer->at(i)) {
				data = data | 1;
			}
		}
		if (i < 7)
			data = data << 1;
		// cout << i << ". data: " << (unsigned int) data << endl;
	}

	outFile->write((char *) & data, sizeof(data));

	// cout << "string word: " << write << endl;
	// cout << ++count << ". wrote to file: " << (int)data << endl;
#ifdef DEBUG_MODE 
	cout << ++count << ". wrote to file: " << (int)data << endl;
	// cout << "byte_buffer size: " << size << endl;
#endif
}

unsigned int get_compressed_size(const unsigned int *freq, map<unsigned char, vector<bool>> *codes) {
	unsigned int compressed_size = 0;
	for (int i = 0; i < BYTE_MAX; i++) {
		if (freq[i] != 0) {
			unsigned char c = (unsigned char) i;
			auto const& it = codes->find(c);
			if (it != codes->end()) {
				compressed_size += it->second.size() * freq[i];
			}
			else {
				cout << "Unexpected error: code for byte value " << i << " not found." << endl;
				cout << "Could not encode the file. Please try again." << endl;
				exit(-1);
			}
		}
	}
	return compressed_size;
}

void print_nonzero_freq(const unsigned int *freq) {
	for (int i = 0; i < BYTE_MAX; i++) {
		if (freq[i] != 0){
			char c = (char)i;
			printf("%c: %d\n", c, freq[i]);
		}
	}
}

int filesize(const string filename) {
	ifstream in(filename, ifstream::ate | ifstream::binary);
	int size = in.tellg();
	if (in) in.close();
	return size;
}

















	
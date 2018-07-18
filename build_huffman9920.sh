if g++ henc9920.cpp -o henc9920 -std=c++11 -O3; then
	echo "Huffman encoder compiled successfully. Executable name: 'henc9920'."
else
	echo "Could not compile huffman encoder."
fi
if g++ hdec9920.cpp -o hdec9920 -std=c++11 -O3; then
	echo "Huffman decoder compiled successfully. Executable name: 'hdec9920'."
else
	echo "Could not compile huffman decoder."
fi

//
//  main.cpp
//  Huffman Coding
//
//  Created by Tyler Franklin on 4/15/25.
//

#include <iostream>
#include <map>
#include <string>
#include <queue>
#include <vector>
#include <cstdint>
#include <algorithm>

using namespace std;

struct Node {
    char ch;
    int freq;
    int tiebreaker;
    
    Node* left;
    Node* right;
    
    Node(char c, int f, int t) : ch(c), freq(f), tiebreaker(t), left(nullptr), right(nullptr){}
    Node(Node* l, Node* r, int t) : ch('\0'), freq(l->freq + r->freq), tiebreaker(t), left(l), right(r) {}
};

struct Compare {
    bool operator()(Node* a, Node* b) {
        if(a->freq == b->freq){
            return a->tiebreaker > b->tiebreaker;
        }
        else {
            return a->freq > b->freq;
        }
    }
};

struct Codes {
    char ch;
    string bits;
};

void generateCodes(Node* node, string path, map<char, string>& codeMap){
    if(!node) return;
    
    if(!node->left && !node-> right) {
        codeMap[node->ch] = path;
        return;
    }
    
    generateCodes(node->left, path + "0", codeMap);
    generateCodes(node->right, path + "1", codeMap);
}

bool compareCodes(const Codes& a, const Codes& b) {
    if(a.bits.size() == b.bits.size()){
        return a.ch < b.ch;
    }
    else{
        return a.bits.size() < b.bits.size();
    }
}

void clearTree(Node* node) {
    if(!node){
        return;
    }
    clearTree(node->left);
    clearTree(node->right);
    delete node;
}

int main() {
    string text = "Once there were brook trouts in the streams in the mountains. You could see them standing in the amber current where the white edges of their fins wimpled softly in the flow. They smelled of moss in your hand. Polished and muscular and torsional. On their backs were vermiculate patterns that were maps of the world in its becoming. Maps and mazes. Of a thing which could not be put back. Not be made right again. In the deep glens where they lived all things were older than man and they hummed of mystery.";
    
    map<char, int> freq;
    for(int i = 0; i < text.size(); ++i) {
        char ch = text[i];
        freq[ch]++;
    }
    
    priority_queue<Node*, vector<Node*>, Compare> pq;
    int tiebreaker = 0;
    
    for(map<char, int>::iterator it = freq.begin(); it != freq.end(); ++it) {
        Node* node = new Node(it->first, it->second, tiebreaker++);
        pq.push(node);
    }
    
    while(pq.size() > 1) {
        Node* left = pq.top();
        pq.pop();
        Node* right = pq.top();
        pq.pop();
        
        Node* parent = new Node(left, right, tiebreaker++);
        pq.push(parent);
    }
    Node* root = pq.top();
    
    map<char, string> codeMap;
    generateCodes(root, "", codeMap);
    
    vector<Codes> sortedCodes;
        
    for(map<char, string>::iterator it = codeMap.begin(); it != codeMap.end(); ++it) {
        Codes codes;
        codes.ch = it->first;
        codes.bits = it->second;
        sortedCodes.push_back(codes);
    }
    sort(sortedCodes.begin(), sortedCodes.end(), compareCodes);
    
    cout << "\nCharacter Codes:\n";
    for (size_t i = 0; i < sortedCodes.size(); i++) {
        char c = sortedCodes[i].ch;
        if(c == ' '){
            cout << "[Space]: ";
        }
        else {
            cout << " " << c << ": ";
        }
        cout << sortedCodes[i].bits << "\n";
    }
    
    int compressedBits = 0;

    vector<uint8_t> bytes;
    uint8_t curr = 0, bitCounter = 0;
    for(size_t i = 0; i < text.size(); ++i){
        string bits = codeMap[text[i]];
        for(size_t j = 0; j < bits.size(); ++j){
            curr = (curr << 1) | (bits[j] == '1');
            
            if(++bitCounter == 8) {
                bytes.push_back(curr);
                curr = 0;
                bitCounter = 0;
            }
            ++compressedBits;
        }
    }
    
    if(bitCounter > 0){
        curr = curr << (8 - bitCounter);
        bytes.push_back(curr);
    }
    
    cout << "\nFirst 8 bytes of encoded text in binary: \n";
    for(size_t i = 0; i < 8 && i < bytes.size(); ++i){
        for(int j = 7; j >= 0; j--){
            cout << ((bytes[i] >> j) & 1);
        }
        if(i + 1 < 8 && i + 1 < bytes.size()){
            cout << ' ';
        }
    }
    
    int compressedBytes = bytes.size();
    int originalBits = text.size() * 8;
    
    cout << "\n\nEncoded size: " << compressedBits << " bits (" << compressedBytes << " bytes)\n";
    cout << "Original size: " << originalBits << " bits (" << text.size() << " bytes)" << endl;
    
    double ratio = double(compressedBits) / double(originalBits);
    cout << "Compression ratio: " << ratio << endl;
    
    clearTree(root);
    
    return 0;
}

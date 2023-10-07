#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <openssl/sha.h>

class Block {
public:
    int index;
    std::string previousHash;
    std::string data;
    std::string hash;
    time_t timestamp;

    Block(int idx, const std::string &prevHash, const std::string &data)
        : index(idx), previousHash(prevHash), data(data) {
        timestamp = std::time(nullptr);
        hash = calculateHash();
    }

    std::string calculateHash() {
        std::string dataToHash = std::to_string(index) + previousHash + std::to_string(timestamp) + data;
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256((const unsigned char*)dataToHash.c_str(), dataToHash.length(), hash);

        std::string hashStr = "";
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            char hex[3];
            sprintf(hex, "%02x", hash[i]);
            hashStr += hex;
        }
        return hashStr;
    }
};

class Blockchain {
private:
    std::vector<Block> chain;

public:
    Blockchain() {
        chain.emplace_back(0, "0", "Genesis Block");
    }

    Block createBlock(const std::string &data) {
        const Block &previousBlock = chain.back();
        int newIndex = previousBlock.index + 1;
        return Block(newIndex, previousBlock.hash, data);
    }

    void addBlock(Block &block) {
        if (isValidNewBlock(block)) {
            chain.push_back(block);
        }
    }

    bool isValidNewBlock(const Block &newBlock) {
        const Block &previousBlock = chain.back();
        return (previousBlock.index + 1 == newBlock.index) &&
               (previousBlock.hash == newBlock.previousHash) &&
               (newBlock.hash == newBlock.calculateHash());
    }

    bool isChainValid() {
        for (int i = 1; i < chain.size(); i++) {
            const Block &currentBlock = chain[i];
            const Block &previousBlock = chain[i - 1];

            if (currentBlock.hash != currentBlock.calculateHash() ||
                currentBlock.previousHash != previousBlock.hash) {
                return false;
            }
        }
        return true;
    }
};

int main() {
    Blockchain myBlockchain;

    Block block1 = myBlockchain.createBlock("Transaction 1");
    myBlockchain.addBlock(block1);

    Block block2 = myBlockchain.createBlock("Transaction 2");
    myBlockchain.addBlock(block2);

    std::cout << "Blockchain is valid: " << (myBlockchain.isChainValid() ? "Yes" : "No") << std::endl;

    return 0;
}

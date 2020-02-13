//
//  bpTree.h
//  DatabaseDesignProject
//
//  Created by Dayuan Chen on 11/9/19.
//  Copyright © 2019 N33MO. All rights reserved.
//

#ifndef bpTree_h
#define bpTree_h

#include "bpNode.h"
#include <vector>
using namespace std;

enum COMPARE_OPERATOR {LT, LE, EQ, BE, BT, BETWEEN};
const int INVALID_INDEX = -1;
struct SelectResult
{
    int keyIndex;
    CLeafNode* targetNode;
};

class CBPlusTree {
public:
    CBPlusTree();
    ~CBPlusTree();
    
    bool insert(KeyType key, const DataType& data);
//    bool remove(KeyType key);
    bool update(KeyType oldKey, KeyType newKey);
    vector<DataType> select(KeyType compareKey, int compareOperator);
    vector<DataType> select(KeyType smallKey, KeyType largeKey);
    bool search(KeyType key);
    void clear();
    void print() const;
    void printData() const;
private:
    void recursive_insert(CNode* parentNode, KeyType key, const DataType& data);
//    void recursive_remove(CNode* parentNode, KeyType key);
    void printInConcavo(CNode *pNode, int count) const;
    bool recursive_search(CNode *pNode, KeyType key) const;
    void changeKey(CNode *pNode, KeyType oldKey, KeyType newKey); // //
    void search(KeyType key, SelectResult& result);
    void recursive_search(CNode* pNode, KeyType key, SelectResult& result);
//    void remove(KeyType key, DataType& dataValue);
//    void recursive_remove(CNode* parentNode, KeyType key, DataType& dataValue);
private:
    CNode* m_Root;
    CLeafNode* m_DataHead;
    KeyType m_MaxKey;
};

#endif /* bpTree_h */

//
//  bpFunc.h
//  DatabaseDesignProject
//
//  Created by Dayuan Chen on 11/9/19.
//  Copyright © 2019 N33MO. All rights reserved.
//

#ifndef bpFunc_h
#define bpFunc_h

#include <string>
using namespace std;

typedef string KeyType;
typedef string DataType;
enum NODE_TYPE {INTERNAL, LEAF};

const int ORDER = 5;
const int ORDER_HALF = 3;

class bpNode {
public:
    bpNode();
    virtual ~bpNode();
    
    NODE_TYPE getType() const {return m_Type;}
    void setType(NODE_TYPE type) {m_Type = type;}
    
    int getKeyNum() const {return m_KeyNum;}
    void setKeyNum(int i) {m_KeyNum = i;}
    KeyType getKeyValue(int i) const {return m_KeyValue[i];}
    void setKeyValue(int i, KeyType key) {m_KeyValue[i] = key;}
    int getKeyIndex(KeyType key) const;
    
    virtual void split(bpNode* parentNode, int childIndex) = 0;
    virtual int getChildIndex(KeyType key, int keyIndex) const = 0;
    
protected:
    NODE_TYPE m_Type;
    int m_KeyNum;
    KeyType m_KeyValue[ORDER];
};

class internalNode : public bpNode {
public:
    internalNode();
    virtual ~internalNode();
    
    bpNode* getChild(int i) const {return m_Child[i];}
    void setChild(int i, bpNode* child) {m_Child[i] = child;}
    void insert(int keyIndex, int childIndex, KeyType key, bpNode childNode);
    
    virtual void split(bpNode* parentNode, int childIndex);
    virtual int getChildIndex(KeyType key, int keyIndex) const;
    
private:
    bpNode* m_Child[ORDER];
};

class leafNode : public bpNode {
public:
    leafNode();
    virtual ~leafNode();
    
    leafNode* getNextSibling() const {return m_nextSibling;}
    void setNextSibling(leafNode* node) {m_nextSibling = node;}
    DataType getData(int i) const {return m_Data[i];}
    void setData(int i, const DataType& data) {m_Data[i] = data;}
    void insert(KeyType key, const DataType& data);
    
    virtual void splic(bpNode* parentNode, int childIndex);
    virtual int getChildIndex(KeyType key, int keyIndex) const;
    
private:
    leafNode* m_nextSibling;
    DataType m_Data[ORDER];
};

#endif /* bpFunc_h */

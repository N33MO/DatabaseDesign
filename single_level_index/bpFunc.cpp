//
//  bpFunc.cpp
//  DatabaseDesignProject
//
//  Created by Dayuan Chen on 11/9/19.
//  Copyright © 2019 N33MO. All rights reserved.
//

#include "bpFunc.h"

bpNode::bpNode() {
    setType(LEAF);
    setKeyNum(0);
}

bpNode::~bpNode() {
    setKeyNum(0);
}

int bpNode::getKeyIndex(KeyType key) const
{
    return -1;
}

// leaf node
leafNode::leafNode() {
    setType(LEAF);
    setNextSibling(NULL);
}

leafNode::~leafNode() {
    
}

void leafNode::insert(KeyType key, const DataType &data)
{
    setKeyValue(<#int i#>, key);
    setData(<#int i#>, data);
}


// internal node
internalNode::internalNode() {
    setType(INTERNAL);
}

internalNode::~internalNode() {
    
}



void internalNode::split(bpNode *parentNode, int childIndex)
{
    internalNode* newNode = new internalNode();
    newNode -> setKeyNum(ORDER_HALF);
}

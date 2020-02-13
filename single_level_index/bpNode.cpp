//
//  bpNode.cpp
//  DatabaseDesignProject
//
//  Created by Dayuan Chen on 11/8/19.
//  Copyright © 2019 N33MO. All rights reserved.
//

#include "bpNode.h"

// CNode
CNode::CNode(){
    setType(LEAF);
    setKeyNum(0);
}

CNode::~CNode(){
    setKeyNum(0);
}

// //
int CNode::getKeyIndex(KeyType key) const
{
    int left = 0;
//    int right = getKeyNum() - 1;
    int right = getKeyNum();
    int current;
    while (left != right) {
        current = (left + right) / 2;
        KeyType currentKey = getKeyValue(current);
        if (key > currentKey) {
            left = current + 1;
        }
        else {
            right = current;
        }
    }
    return left;
}

// CInternalNode
CInternalNode::CInternalNode(){
    setType(INTERNAL);
}

CInternalNode::~CInternalNode(){
    
}

void CInternalNode::clear()
{
    for (int i = 0; i <= m_KeyNum; i++) {
        m_Childs[i] -> clear();
        delete m_Childs[i];
        m_Childs[i] = NULL;
    }
}

void CInternalNode::split(CNode *parentNode, int childIndex)
{
    CInternalNode* newNode = new CInternalNode();
    newNode -> setKeyNum(MINIMUM_KEY);
    for (int i = 0; i < MINIMUM_KEY; i++) {
        newNode -> setKeyValue(i, m_KeyValues[i + MINIMUM_KEY]);
    }
    for (int i = 0; i < MINIMUM_CHILD; i++) {
        newNode -> setChild(i, m_Childs[i + MINIMUM_CHILD]);
    }
    setKeyNum(MINIMUM_KEY);
    ((CInternalNode*)parentNode) -> insert(childIndex, childIndex+1, m_KeyValues[MINIMUM_KEY], newNode);
}

void CInternalNode::insert(int keyIndex, int childIndex, KeyType key, CNode *childNode)
{
    int i;
    for (i = getKeyNum(); i > keyIndex; i--) {
        setChild(i+1, m_Childs[i]);
        setKeyValue(i, m_KeyValues[i-1]);
    }
    if (i == childIndex) {
        setChild(i+1, m_Childs[i]);
    }
    setChild(childIndex, childNode);
    setKeyValue(keyIndex, key);
    setKeyNum(m_KeyNum+1);
}

//void CInternalNode::mergeChild(CNode *parentNode, CNode *childNode, int keyIndex)
//{
//    insert(MINIMUM_KEY, MINIMUM_KEY+1, parentNode->getKeyValue(keyIndex), ((CInternalNode*)childNode)->getChild(0));
//    for (int i = 1; i <= childNode->getKeyNum(); i++) {
//        insert(MINIMUM_KEY+i, MINIMUM_KEY+i+1, childNode->getKeyValue(i-1), ((CInternalNode*)childNode)->getChild(i));
//    }
//    parentNode->removeKey(keyIndex, keyIndex+1);
//    delete ((CInternalNode*)parentNode)->getChild(keyIndex+1);
//}

//void CInternalNode::removeKey(int keyIndex, int childIndex)
//{
//    for (int i = 0; i < getKeyNum()-keyIndex-1; i++) {
//        setKeyValue(keyIndex+i, getKeyValue(keyIndex+i+1));
//        setChild(childIndex+i, getChild(childIndex+i+1));
//    }
//    setKeyNum(getKeyNum()-1);
//}

//void CInternalNode::borrowFrom(CNode *siblingNode, CNode *parentNode, int keyIndex, SIBLING_DIRECTION d)
//{
//    switch (d) {
//        case LEFT:
//        {
//            insert(0, 0, parentNode->getKeyValue(keyIndex), ((CInternalNode*)siblingNode)->getChild(siblingNode->getKeyNum()));
//            parentNode->setKeyValue(keyIndex, siblingNode->getKeyNum()-1);
//            siblingNode->removeKey(siblingNode->getKeyNum()-1, siblingNode->getKeyNum());
//        }
//            break;
//        case RIGHT:
//        {
//            insert(getKeyNum(), getKeyNum()+1, parentNode->getKeyValue(keyIndex), ((CInternalNode*)siblingNode)->getChild(0));
//            parentNode->setKeyValue(keyIndex, siblingNode->getKeyValue(0));
//            siblingNode->removeKey(0, 0);
//        }
//            break;
//        default:
//            break;
//    }
//}

int CInternalNode::getChildIndex(KeyType key, int keyIndex) const
{
    if (key == getKeyValue(keyIndex)) {
        return keyIndex + 1;
    }
    else
    {
        return keyIndex;
    }
}

// CLeafNode
CLeafNode::CLeafNode(){
    setType(LEAF);
    setLeftSibling(NULL);
    setRightSibling(NULL);
}

CLeafNode::~CLeafNode(){
    
}

void CLeafNode::clear()
{
    for (int i = 0; i < m_KeyNum; i++) {
        // if type of m_Datas is pointer
        // delete m_Datas[i]
        // m_Datas[i] = NULL
    }
}

void CLeafNode::insert(KeyType key, const DataType &data)
{
    for (int i = m_KeyNum; i >= 1 && m_KeyValues[i-1] > key; i--) {
        setKeyValue(i, m_KeyValues[i-1]);
        setData(i, data);
        setKeyNum(m_KeyNum+1);
    }
}

void CLeafNode::split(CNode *parentNode, int childIndex)
{
    CLeafNode* newNode = new CLeafNode();
    setKeyNum(MINIMUM_LEAF);
    newNode -> setKeyNum(MINIMUM_LEAF+1);
    newNode -> setRightSibling(getRightSibling());
    setRightSibling(newNode);
    newNode -> setLeftSibling(this);
    int i;
    for (i = 0; i < MINIMUM_LEAF+1; i++) {
        newNode -> setKeyValue(i, m_KeyValues[i+MINIMUM_LEAF]);
    }
    for (i = 0; i < MINIMUM_LEAF+1; i++) {
        newNode ->setData(i, m_Datas[i+MINIMUM_LEAF]);
    }
    ((CInternalNode*)parentNode)->insert(childIndex, childIndex+1, m_KeyValues[MINIMUM_LEAF], newNode);
}

//void CLeafNode::mergeChild(CNode *parentNode, CNode *childNode, int keyIndex)
//{
//    for (int i = 0; i < childNode->getKeyNum(); i++) {
//        insert(childNode->getKeyValue(i), ((CLeafNode*)childNode)->getData(i));
//    }
//    setRightSibling(((CLeafNode*)childNode)->getRightSibling());
//    parentNode->removeKey(keyIndex, keyIndex+1);
//}

//void CLeafNode::removeKey(int keyIndex, int childIndex)
//{
//    for (int i = keyIndex; i < getKeyNum()-1; i++) {
//        setKeyValue(i, getKeyValue(i+1));
//        setData(i, getData(i+1));
//    }
//    setKeyNum(getKeyNum()-1);
//}

//void CLeafNode::borrowFrom(CNode *siblingNode, CNode *parentNode, int keyIndex, SIBLING_DIRECTION d)
//{
//    switch (d) {
//        case LEFT:
//        {
//            insert(siblingNode->getKeyValue(siblingNode->getKeyNum()-1), ((CLeafNode*)siblingNode)->getData(siblingNode->getKeyNum()-1));
//            siblingNode->removeKey(siblingNode->getKeyNum()-1, siblingNode->getKeyNum()-1);
//            parentNode->setKeyValue(keyIndex, getKeyValue(0));
//        }
//            break;
//        case RIGHT:
//        {
//            insert(siblingNode->getKeyValue(0), ((CLeafNode*)siblingNode)->getData(0));
//            siblingNode->removeKey(0, 0);
//            parentNode->setKeyValue(keyIndex, siblingNode->getKeyValue(0));
//        }
//            break;
//            
//        default:
//            break;
//    }
//}

int CLeafNode::getChildIndex(KeyType key, int keyIndex) const
{
    return keyIndex;
}

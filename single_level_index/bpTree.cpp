//
//  bpTree.cpp
//  DatabaseDesignProject
//
//  Created by Dayuan Chen on 11/9/19.
//  Copyright © 2019 N33MO. All rights reserved.
//

#include "bpTree.h"
#include <iostream>
#include <algorithm>
using namespace std;

CBPlusTree::CBPlusTree() {
    m_Root = NULL;
    m_DataHead = NULL;
}

CBPlusTree::~CBPlusTree() {
    clear();
}

bool CBPlusTree::insert(KeyType key, const DataType &data)
{
    if (search(key)) {
        return false;
    }
    if (m_Root == NULL) {
        m_Root = new CLeafNode();
        m_DataHead = (CLeafNode*)m_Root;
        m_MaxKey = key;
    }
    if (m_Root->getKeyNum() >= MAXIMUM_KEY) {
        CInternalNode* newNode = new CInternalNode();
        newNode->setChild(0, m_Root);
        m_Root->split(newNode, 0);
        m_Root = newNode;
    }
    if (key > m_MaxKey) {
        m_MaxKey = key;
    }
    recursive_insert(m_Root, key, data);
    return true;
}

void CBPlusTree::recursive_insert(CNode *parentNode, KeyType key, const DataType &data)
{
    if (parentNode->getType() == LEAF) {
        ((CLeafNode*)parentNode)->insert(key, data);
    }
    else
    {
        int keyIndex = parentNode->getKeyIndex(key);
        int childIndex = parentNode->getChildIndex(key, keyIndex);
        CNode* childNode = ((CInternalNode*)parentNode)->getChild(childIndex);
        if (childNode->getKeyNum() >= MAXIMUM_LEAF) {
            childNode->split(parentNode, childIndex);
            if (parentNode->getKeyValue(childIndex) <= key) {
                childNode = ((CInternalNode*)parentNode)->getChild(childIndex+1);
            }
        }
        recursive_insert(childNode, key, data);
    }
}

void CBPlusTree::clear()
{
    if (m_Root != NULL) {
//        m_Root->clear();
//        delete m_Root;
//        m_Root = NULL;
//        m_DataHead = NULL;
    }
}

bool CBPlusTree::search(KeyType key)
{
    return recursive_search(m_Root, key);
}

bool CBPlusTree::recursive_search(CNode *pNode, KeyType key) const
{
    if (pNode == NULL) {
        return false;
    }
    else
    {
        int keyIndex = pNode->getKeyIndex(key);
        int childIndex = pNode->getChildIndex(key, keyIndex);
        if (keyIndex < pNode->getKeyNum() && key == pNode->getKeyValue(keyIndex)) {
            return true;
        }
        else
        {
            if (pNode->getType() == LEAF) {
                return false;
            }
            else
            {
                return recursive_search(((CInternalNode*)pNode)->getChild(childIndex), key);
            }
        }
    }
}

void CBPlusTree::print() const
{
    printInConcavo(m_Root, 10);
}

void CBPlusTree::printInConcavo(CNode *pNode, int count) const
{
    if (pNode != NULL) {
        int i, j;
        for (i = 0; i < pNode->getKeyNum(); i++) {
            if (pNode->getType() != LEAF) {
                printInConcavo(((CInternalNode*)pNode)->getChild(i), count-2);
            }
            for (j = count; j >= 0; j--) {
                cout << "-";
            }
            cout << pNode->getKeyValue(i) << endl;
        }
        if (pNode->getType() != LEAF) {
            printInConcavo(((CInternalNode*)pNode)->getChild(i), count-2);
        }
    }
}

void CBPlusTree::printData() const
{
    CLeafNode* itr = m_DataHead;
    cout << "list index: " << endl; // //
    while (itr != NULL) {
        for (int i = 0; i < itr->getKeyNum(); i++) {
            cout << itr->getData(i) << " ";
        }
        cout << endl;
        itr = itr -> getRightSibling();
    }
}

//bool CBPlusTree::remove(KeyType key)
//{
//    if (!search(key)) {
//        return false;
//    }
//    if (m_Root->getKeyNum() == 1) {
//        if (m_Root->getType() == LEAF) {
//            clear();
//            return true;
//        }
//        else
//        {
//            CNode *pChild1 = ((CInternalNode*)m_Root)->getChild(0);
//            CNode *pChild2 = ((CInternalNode*)m_Root)->getChild(1);
//            if (pChild1->getKeyNum() == MINIMUM_KEY && pChild2->getKeyNum() == MINIMUM_KEY) {
//                pChild1->mergeChild(m_Root, pChild2, 0);
//                delete m_Root;
//                m_Root = pChild1;
//            }
//        }
//    }
//    recursive_remove(m_Root, key);
//    return true;
//}

//void CBPlusTree::recursive_remove(CNode *parentNode, KeyType key)
//{
//    int keyIndex = parentNode->getKeyIndex(key);
//    int childIndex = parentNode->getChildIndex(key, keyIndex);
//    if (parentNode->getType() == LEAF) {
//        if (key == m_MaxKey && keyIndex > 0) {
//            m_MaxKey = parentNode->getKeyValue(keyIndex-1);
//        }
//        parentNode->removeKey(keyIndex, childIndex);
//        if (childIndex == 0 && m_Root->getType() != LEAF && parentNode != m_DataHead) {
//            changeKey(m_Root, key, parentNode->getKeyValue(0));
//        }
//    }
//    else
//    {
//        CNode *pChildNode = ((CInternalNode*)parentNode)->getChild(childIndex);
//        if (pChildNode->getKeyNum() == MINIMUM_KEY) {
//            CNode *pLeft = childIndex > 0 ? ((CInternalNode*)parentNode)->getChild(childIndex-1) : NULL;
//            CNode *pRight = childIndex < parentNode->getKeyNum() ? ((CInternalNode*)parentNode)->getChild(childIndex+1) : NULL;
//            if (pLeft && pLeft->getKeyNum() > MINIMUM_KEY) {
//                pChildNode->borrowFrom(pLeft, parentNode, childIndex-1, LEFT);
//            }
//            else if (pRight && pRight->getKeyNum() > MINIMUM_KEY) {
//                pChildNode->borrowFrom(pRight, parentNode, childIndex, RIGHT);
//            }
//            else if (pLeft) {
//                pLeft->mergeChild(parentNode, pChildNode, childIndex-1);
//                pChildNode = pLeft;
//            }
//            else if (pRight) {
//                pChildNode->mergeChild(parentNode, pRight, childIndex);
//            }
//        }
//        recursive_remove(pChildNode, key);
//    }
//}

void CBPlusTree::changeKey(CNode *pNode, KeyType oldKey, KeyType newKey)
{
    if (pNode != NULL && pNode->getType() != LEAF) {
        int keyIndex = pNode->getKeyIndex(oldKey);
        if (keyIndex < pNode->getKeyNum() && oldKey == pNode->getKeyValue(keyIndex)) {
            pNode->setKeyValue(keyIndex, newKey);
        }
        else
        {
            changeKey(((CInternalNode*)pNode)->getChild(keyIndex), oldKey, newKey);
        }
    }
}

//bool CBPlusTree::update(KeyType oldKey, KeyType newKey)
//{
//    if (search(newKey)) {
//        return false;
//    }
//    else
//    {
//        int dataValue;
//        remove(oldKey, dataValue);
//        if (dataValue == INVALID_INDEX) {
//            return false;
//        }
//        else
//        {
//            return insert(newKey, dataValue);
//        }
//    }
//}

//void CBPlusTree::remove(KeyType key, DataType& dataValue)
//{
//    if (!search(key))  //不存在
//    {
//        dataValue = INVALID_INDEX;
//        return;
//    }
//    if (m_Root->getKeyNum() == 1)//特殊情况处理
//    {
//        if (m_Root->getType() == LEAF)
//        {
//            dataValue = ((CLeafNode*)m_Root)->getData(0);
//            clear();
//            return;
//        }
//        else
//        {
//            CNode *pChild1 = ((CInternalNode*)m_Root)->getChild(0);
//            CNode *pChild2 = ((CInternalNode*)m_Root)->getChild(1);
//            if (pChild1->getKeyNum()==MINIMUM_KEY && pChild2->getKeyNum()==MINIMUM_KEY)
//            {
//                pChild1->mergeChild(m_Root, pChild2, 0);
//                delete m_Root;
//                m_Root = pChild1;
//            }
//        }
//    }
//    recursive_remove(m_Root, key, dataValue);
//}

//void CBPlusTree::recursive_remove(CNode* parentNode, KeyType key, DataType& dataValue)
//{
//    int keyIndex = parentNode->getKeyIndex(key);
//    int childIndex= parentNode->getChildIndex(key, keyIndex); // 孩子结点指针索引
//    if (parentNode->getType() == LEAF)// 找到目标叶子节点
//    {
//        if (key == m_MaxKey && keyIndex > 0)
//        {
//            m_MaxKey = parentNode->getKeyValue(keyIndex-1);
//        }
//        dataValue = ((CLeafNode*)parentNode)->getData(keyIndex);
//        parentNode->removeKey(keyIndex, childIndex);  // 直接删除
//        // 如果键值在内部结点中存在，也要相应的替换内部结点
//        if (childIndex==0 && m_Root->getType() != LEAF && parentNode != m_DataHead)
//        {
//            changeKey(m_Root, key, parentNode->getKeyValue(0));
//        }
//    }
//    else // 内结点
//    {
//        CNode *pChildNode = ((CInternalNode*)parentNode)->getChild(childIndex); //包含key的子树根节点
//        if (pChildNode->getKeyNum() == MINIMUM_KEY)                       // 包含关键字达到下限值，进行相关操作
//        {
//            CNode *pLeft = childIndex > 0 ? ((CInternalNode*)parentNode)->getChild(childIndex-1) : NULL;                       //左兄弟节点
//            CNode *pRight = childIndex < parentNode->getKeyNum() ? ((CInternalNode*)parentNode)->getChild(childIndex+1) : NULL;//右兄弟节点
//            // 先考虑从兄弟结点中借
//            if (pLeft && pLeft->getKeyNum() > MINIMUM_KEY)// 左兄弟结点可借
//            {
//                pChildNode->borrowFrom(pLeft, parentNode, childIndex-1, LEFT);
//            }
//            else if (pRight && pRight->getKeyNum() > MINIMUM_KEY)//右兄弟结点可借
//            {
//                pChildNode->borrowFrom(pRight, parentNode, childIndex, RIGHT);
//            }
//            //左右兄弟节点都不可借，考虑合并
//            else if (pLeft)                    //与左兄弟合并
//            {
//                pLeft->mergeChild(parentNode, pChildNode, childIndex-1);
//                pChildNode = pLeft;
//            }
//            else if (pRight)                   //与右兄弟合并
//            {
//                pChildNode->mergeChild(parentNode, pRight, childIndex);
//            }
//        }
//        recursive_remove(pChildNode, key, dataValue);
//    }
//}

vector<DataType> CBPlusTree::select(KeyType compareKey, int compareOpeartor)
{
    vector<DataType> results;
    if (m_Root != NULL)
    {
        if (compareKey > m_MaxKey)   // 比较键值大于B+树中最大的键值
        {
            if (compareOpeartor == LE || compareOpeartor == LT)
            {
                for(CLeafNode* itr = m_DataHead; itr != NULL; itr = itr->getRightSibling())
                {
                    for (int i = 0; i < itr->getKeyNum(); i++)
                    {
                        results.push_back(itr->getData(i));
                    }
                }
            }
        }
        else if (compareKey<m_DataHead->getKeyValue(0))  // 比较键值小于B+树中最小的键值
        {
            if (compareOpeartor == BE || compareOpeartor == BT)
            {
                for(CLeafNode* itr = m_DataHead; itr != NULL; itr = itr->getRightSibling())
                {
                    for (int i = 0; i < itr->getKeyNum(); i++)
                    {
                        results.push_back(itr->getData(i));
                    }
                }
            }
        }
        else  // 比较键值在B+树中
        {
            SelectResult result;
            search(compareKey, result);
            switch(compareOpeartor)
            {
            case LT:
            case LE:
                {
                    CLeafNode* itr = m_DataHead;
                    int i;
                    while (itr != result.targetNode)
                    {
                        for (i = 0; i < itr->getKeyNum(); i++)
                        {
                            results.push_back(itr->getData(i));
                        }
                        itr = itr->getRightSibling();
                    }
                    for (i = 0; i < result.keyIndex; i++)
                    {
                        results.push_back(itr->getData(i));
                    }
                    if (itr->getKeyValue(i) < compareKey ||
                        (compareOpeartor == LE && compareKey == itr->getKeyValue(i)))
                    {
                        results.push_back(itr->getData(i));
                    }
                }
                break;
            case EQ:
                {
                    if (result.targetNode->getKeyValue(result.keyIndex) == compareKey)
                    {
                        results.push_back(result.targetNode->getData(result.keyIndex));
                    }
                }
                break;
            case BE:
            case BT:
                {
                    CLeafNode* itr = result.targetNode;
                    if (compareKey < itr->getKeyValue(result.keyIndex) ||
                        (compareOpeartor == BE && compareKey == itr->getKeyValue(result.keyIndex))
                        )
                    {
                        results.push_back(itr->getData(result.keyIndex));
                    }
                    int i;
                    for (i = result.keyIndex+1; i < itr->getKeyNum(); i++)
                    {
                        results.push_back(itr->getData(i));
                    }
                    itr = itr->getRightSibling();
                    while (itr != NULL)
                    {
                        for (i = 0; i < itr->getKeyNum(); i++)
                        {
                            results.push_back(itr->getData(i));
                        }
                        itr = itr->getRightSibling();
                    }
                }
                break;
            default:  // 范围查询
                break;
            }
        }
    }
    sort<vector<DataType>::iterator>(results.begin(), results.end());
    return results;
}

vector<DataType> CBPlusTree::select(KeyType smallKey, KeyType largeKey)
{
    vector<DataType> results;
    if (smallKey <= largeKey)
    {
        SelectResult start, end;
        search(smallKey, start);
        search(largeKey, end);
        CLeafNode* itr = start.targetNode;
        int i = start.keyIndex;
        if (itr->getKeyValue(i) < smallKey)
        {
            i++;
        }
        if (end.targetNode->getKeyValue(end.keyIndex) > largeKey)
        {
            end.keyIndex--;
        }
        while (itr != end.targetNode)
        {
            for (; i < itr->getKeyNum(); i++)
            {
                results.push_back(itr->getData(i));
            }
            itr = itr->getRightSibling();
            i = 0;
        }
        for (; i <= end.keyIndex; i++)
        {
            results.push_back(itr->getData(i));
        }
    }
    sort<vector<DataType>::iterator>(results.begin(), results.end());
    return results;
}

void CBPlusTree::search(KeyType key, SelectResult &result)
{
    recursive_search(m_Root, key, result);
}

void CBPlusTree::recursive_search(CNode *pNode, KeyType key, SelectResult &result)
{
    int keyIndex = pNode->getKeyIndex(key);
    int childIndex = pNode->getChildIndex(key, keyIndex);
    if (pNode->getType() == LEAF) {
        result.keyIndex = keyIndex;
        result.targetNode = (CLeafNode*)pNode;
        return;
    }
    else
    {
        return recursive_search(((CInternalNode*)pNode)->getChild(childIndex), key, result);
    }
}


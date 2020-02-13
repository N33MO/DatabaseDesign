//
//  main.cpp
//  DatabaseDesignProject
//
//  Created by Dayuan Chen on 11/7/19.
//  Copyright © 2019 N33MO. All rights reserved.
//

#include "bpNode.h"
#include "bpTree.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
//#include "singleLevelIndex.hpp"

using namespace std;

static bool func(string str);
static bool isDigital(string str);
static void fileOP(string fileIN, string fileOUT, string len);
static string int2bin(int intValue);
static CBPlusTree createIndex(CBPlusTree index, string key, int rowsize, int totallen);
static void listIndex();

int main(int argc, const char * argv[]) {
    
    argc = 5;
    argv[0] = "0";
    argv[1] = "-c";
    argv[2] = "/Users/n33mo/MyLib/XcodeProjects/DatabaseDesignProject/DatabaseDesignProject/single_level_index/TestDataM.txt";
    argv[3] = "/Users/n33mo/MyLib/XcodeProjects/DatabaseDesignProject/DatabaseDesignProject/single_level_index/output.txt";
    argv[4] = "4";
    
    if (argc != 5) {
        cout << "wrong input." << endl;
        return -1;
    }
    
    if (!func(argv[1])) {
        cout << "wrong func input." << endl;
        return -1;
    }
    
    if (!isDigital(argv[4])) {
        cout << "wrong len input." << endl;
        return -1;
    }
    
    ofstream touch(argv[2], ios::in);
    if (!touch) {
        cout << "wrong input file." << endl;
        return -1;
    }
    touch.close();
    
    
    // Ready to indexing
//    SingleLevelIndex index;
    if (strncmp(argv[1], "-c", 2) == 0) {
//        index.create(argv[2],argv[3],argv[4]);
        fileOP(argv[2],argv[3],argv[4]);
        
    }
    else if (strncmp(argv[1], "-l", 2) == 0) {
//        index.list(argv[2],argv[3],argv[4]);
    }
    
    cout << "Hello World." << endl;
    
    return 0;
}



bool func(string str) {
    return strncmp(str.c_str(), "-c", 2) == 0 || strncmp(str.c_str(), "-l", 2) == 0;
}

bool isDigital(string str) {
    int len = int(str.size());
    if (str.at(0) == '0') {
        return false;
    }
    for (int i = 0; i < len; i++) {
        if (str.at(i) > '9' || str.at(i) < '0')
            return false;
    }
    return true;
}

void fileOP(string fileIN, string fileOUT, string len) {
    
    ifstream fin(fileIN, ios::in);
    ofstream fout(fileOUT, ios::out);
    string temp, keyValue;
    CBPlusTree index;
    int rowSize = 0;
    int totalLen = 0;
    while (getline(fin, temp)) {
        rowSize = int(temp.size());
//        cout << "row length: " << rowSize << endl;
        keyValue = temp.substr(0,atoi(len.c_str()));
//        cout << "row key: " << keyValue << endl;
//        index = createIndex(index, keyValue, rowSize, totalLen);
        string data = int2bin(totalLen);
        index.insert(keyValue, data);
        totalLen = totalLen + rowSize;
    }
    
    index.printData();
    
    fin.close();
    fout.close();
    cout << "Finished!" << endl;
}

string int2bin(int intValue) {
    string a;
    for (int i = 7; i >= 0; i--) {
//        cout << ((intValue>>i)&1);
        a = a + to_string(((intValue>>i)&1));
    }
//    cout << a;
    return a;
}

CBPlusTree createIndex(CBPlusTree index, string key, int rowsize, int totallen) {
    string binlen;
//    cout << "'createIndex' called, recieved data: "
//    << "key: " << key << " rowsize: " << rowsize << " totallen: " << totallen << endl;
    
    binlen = int2bin(totallen);
//    cout << "int totallen to binary: " << binlen;
//    cout << endl;
    
    
    index.insert(key, binlen);
    
    return index;
    
}

void listIndex() {
    
}



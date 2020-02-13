//
//  singleLevelIndex.cpp
//  DatabaseDesignProject
//
//  Created by Dayuan Chen on 11/9/19.
//  Copyright © 2019 N33MO. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

static bool funcPara(string str);   // determine '-c' and '-l'
static bool isDigital(string str);  // determine key length para is integer
static string int2bin(long int intValue);   // make integer offset to binary and stored as string
static void makeIndex(string dataIN, string indexOUT, string len);   // create index
static void extendIndex(string dataIN, string indexIN, string len); // read index to reload file

string res_key[2048] = {""};    // save key - in 'makeIndex'
string res_val[2048] = {""};    // save val - in 'makeIndex'
long int res_val_bin[2048] = {0};    // save val as integer

int main(int argc, const char * argv[]) {
    
//    // test input
//    argc = 5;
//    argv[0] = "0";
//    argv[1] = "-c";
//    argv[2] = "/Users/n33mo/MyLib/XcodeProjects/DatabaseDesignProject/DatabaseDesignProject/singleLevelIndex_noBPlusTree/TestDataA.txt";
//    argv[3] = "/Users/n33mo/MyLib/XcodeProjects/DatabaseDesignProject/DatabaseDesignProject/singleLevelIndex_noBPlusTree/output.dat";
//    argv[4] = "15";
    
    // some restrictions
    if (argc != 5) {            // number of para
        cout << "wrong input." << endl;
        return -1;
    }
    if (!funcPara(argv[1])) {   // '-c' or '-l'
        cout << "wrong funcPara input." << endl;
        return -1;
    }
    if (!isDigital(argv[4])) {  // last input should be an integer
        cout << "wrong len input." << endl;
        return -1;
    }
    // data file detection
    ofstream touch(argv[2], ios::in);
    if (!touch) {
        cout << "wrong input file." << endl;
        return -1;
    }
    touch.close();
    
    
    // Ready to indexing
    if (strncmp(argv[1], "-c", 2) == 0) {
        makeIndex(argv[2],argv[3],argv[4]);
        
    }
    else if (strncmp(argv[1], "-l", 2) == 0) {
        extendIndex(argv[2],argv[3],argv[4]);
    }
    return 0;
}

bool funcPara(string str) {
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
//    if(stoi(str) < 1 || stoi(str) > 24)   // extra condition: key length between [1,24]
//        return false;
    return true;
}

string int2bin(long int intValue) {
    string a;
    for (int i = 63; i >= 0; i--) {
        a = a + to_string(((intValue>>i)&1));
    }
    return a;
}

void makeIndex(string dataIN, string indexOUT, string len) {
    
    string temp;            // temprarily save each line in data file
    string keyValue;        // temprarily save key in each line
    int rowSize = 0;        // length of each line in data file
    long int totalLen = 0;  // total length -> SAVE AS OFFSET
    int cnt = 0;            // count total lines
    
    // read data from file and generate index in two array
    ifstream fdata(dataIN, ios::in);
    while (getline(fdata, temp)) {
        rowSize = int(temp.size());
        keyValue = temp.substr(0,atoi(len.c_str()));
        string data = int2bin(totalLen);    // get binary offset as data for the index
        res_key[cnt] = keyValue;
        res_val[cnt] = data;
        res_val_bin[cnt] = totalLen;
        totalLen += rowSize; cnt++;
    }
    fdata.close();
    
    // Bubble sort the index
    string tmpStr;
    long int tmpLen;
    for (int i = 0; i < cnt-1; i++) {
        for (int j = 0; j < cnt-i-1; j++) {
            int r = strcmp(res_key[j].c_str(), res_key[j+1].c_str());   // compare two string
            if (r > 0) {
                tmpStr = res_key[j];
                res_key[j] = res_key[j+1];
                res_key[j+1] = tmpStr;
                tmpStr = res_val[j];
                res_val[j] = res_val[j+1];
                res_val[j+1] = tmpStr;
                tmpLen = res_val_bin[j];
                res_val_bin[j] = res_val_bin[j+1];
                res_val_bin[j+1] = tmpLen;
            }
        }
    }
    
//    // save sorted index in a file
//    //string indexOUTstr = "/Users/n33mo/MyLib/XcodeProjects/DatabaseDesignProject/DatabaseDesignProject/singleLevelIndex_noBPlusTree/out.txt";
//    ofstream findex(indexOUT, ios::out);
//    for (int i = 0; i < cnt; i++) {
//        findex << res_key[i] << res_val[i] << endl;
//    }
//    findex.close();
    
    // save sorted index as BINARY
    ofstream findex_b(indexOUT, ios::binary);
    for (int i = 0; i < cnt; i++) {
        findex_b << res_key[i];
        findex_b.write((const char*)&res_val_bin[i], sizeof(long int));
    }
    findex_b.close();

    cout << "CREATE INDEX DONE!!!" << endl;
}

void extendIndex(string dataIN, string indexIN, string len) {
    // read original data file into a string
    ifstream fdata(dataIN, ios::in);
    string data = "";
    string temp;
    while (getline(fdata, temp)) {
        data += temp;
    }
    fdata.close();
    
    // read index and print corresponding data file via offset
    cout << "\n RELOADING FILE BY THIS INDEX...\n" << endl;
    
//     // open string index
//    ifstream findex(indexIN, ios::in);
//    string keyValue, offset;
//    while (getline(findex, temp)) {
//        keyValue = temp.substr(0,atoi(len.c_str()));
//        offset = temp.substr(atoi(len.c_str()));
//        int index = 0;
//        int exp = 1;
//        for (int i = 0; i < offset.size(); i++) {
//            index += (offset[offset.size()-i-1]-'0') * exp;
//            exp *= 2;
//        }
//        int ptr = index;
//        while (data[ptr] != '\r') {
//            cout << data[ptr];
//            ptr++;
//        }
//        cout << endl;
//    }
//    findex.close();
    
    // open binary
    ifstream findex_b(indexIN, ios::binary);
    // get total length (bytes) of the index as 'pos' and return the pointer to start point
    findex_b.seekg(0, ios::end);
    streampos pos = findex_b.tellg();
    findex_b.seekg(0);

    // read whole index into 'buffer'
    char buffer[int(pos)];
    findex_b.read(buffer, pos);
    int rowLen = (stoi(len) + 8);   // length for each record in index
    int rows = int(pos / rowLen);   // numbers of records

    for (int i = 0; i < rows; i++) {    // read each record of index and print original data according to the offset as 'ofs'
        long int ofs = 0;
        long int exp = 1;       // exponent for calculate total offset which is stored in 8 char(s)
        for (int j = stoi(len); j < stoi(len) + 8; j++) {   // calculate total offset as 'ofs'
            ofs += int((unsigned char)buffer[i * rowLen + j]) * (exp);
            exp *= 256;
        }
        long int ptr = ofs;
        while (data[ptr] != '\r') {     // print data record of each line, '\r' denotes line break
            cout << data[ptr];
            ptr++;
        }
        cout << endl;
    }
    findex_b.close();
    
    cout << "\n RELOAD FILE DONE!!!" << endl;
    
}

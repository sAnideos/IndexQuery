//============================================================================
// Name        : InfRetProject.cpp
// Author      : John, Sitaras
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <pthread.h>
#include <sstream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <unordered_map>
#include <tgmath.h>
#include <vector>

using namespace std;

static const int num_threads = 2;
int documentsNumber = 7;
int queriesNumber = 7;


class Element {

    public:

        int id;
        string doc;


        Element(int i, string d){

            id = i;
            doc = d;
        }

};


typedef struct list {
    
    int docID; // first element will be the number of docs plz
    struct list *next; // the next document

}list;


class Thing {
    
    public:
        
        int crowd;
        list *firstNode;
        list *head = new list;
        vector<float> vectorArray;


        Thing() {
            crowd = 1;
            //head = NULL;
            firstNode = head;
            //vectorArray = new vector<float>(size, 0.0);
            /*
            vectorArray = new float[documentsNumber];
            for(int i = 0; i < documentsNumber; i++)
            {
                vectorArray[i] = 0.0;
            }*/
        }
        
        void initializeVector() {
            vectorArray.resize(documentsNumber);
            //cout << vectorArray.size() << endl;
            //vectorArray = new vector<float>(documentsNumber);
        }
        
        void incrementCrowd() {
            crowd++;
        }
    
};


unordered_map <string, Thing> InvertedIndex; // the Inverted Index
int *queryTopK;

list *addToList(list *old_node, int num){

    list *element = new list;
    element->next = NULL;
    element->docID = num;
    //cout << "test" << endl;
    old_node->next = element;
    
    
    
    return element;

}


void printList(list *head) {
    
    list *node = head;
    while (node->next != NULL) {
        cout << node->next->docID << "  ";
        node = node->next;
    }
    cout << endl;
}


//dat inverted Index
void invertedIndex() {

}

void printMap() {
    
    for ( auto it = InvertedIndex.begin(); it != InvertedIndex.end(); ++it ) {
        cout << it->first << " " << it->second.crowd << " : ";
        printList(it->second.firstNode);
    }
    cout << endl;
    
}

//prints something
void printList(){

        
        for ( auto it = InvertedIndex.begin(); it != InvertedIndex.end(); ++it ) 
        {
            
            cout << it->first << " ";
            cout << "     ";
            for(int i = 0; i < it->second.vectorArray.size(); i++)
            {
                cout << it->second.vectorArray.at(i) << " ";
            }
            cout << endl;
        }
        cout << endl;
        

}


//This function will be called from a thread
void *call_from_thread(void *a) {
	
    Element *argz = (Element *) a;

    istringstream iss(argz->doc);
    string word;

    

    while(iss >> word)
    {

        //cout << word << endl;
        
        if (InvertedIndex.find(word) != InvertedIndex.end()) {
            
            InvertedIndex[word].incrementCrowd();
            InvertedIndex[word].head = addToList(InvertedIndex[word].head, argz->id);
            InvertedIndex[word].vectorArray.at(argz->id - 1) = 1.0;
            
        }
        else {
                
            Thing thing;// = new Thing(documentsNumber);
            thing.initializeVector();
            InvertedIndex[word] = thing;
            
            // add first docID
            InvertedIndex[word].head = addToList(InvertedIndex[word].head, argz->id);
            InvertedIndex[word].vectorArray.at(argz->id - 1) = 1.0;
            
            //cout << "else" << endl;
        }

    }
    return NULL;
} 

int queryCounter = 0;
//This function will be called from a thread
void *call_from_thread_query(void *a) {
	
    Element *argz = (Element *) a;

    istringstream iss(argz->doc);
    string word;

    

    while(iss >> word)
    {

        //cout << word << endl;
        
        if (InvertedIndex.find(word) != InvertedIndex.end()) {
            
            InvertedIndex[word].vectorArray.at(documentsNumber + queryCounter) = 1.0;
            
            
        } /* if word of query doesnt exist in map then dont add it
        else {
                
            Thing thing;
            InvertedIndex[word] = thing;
            
            // add first docID
            InvertedIndex[word].head = addToList(InvertedIndex[word].head, argz->id);
            InvertedIndex[word].vectorArray[argz->id - 1] = 1.0;
            
            //cout << "else" << endl;
        } */


    }
    queryCounter++;
    return NULL;
}


void readFile(char *filename, int tn) {
	ifstream file(filename);
	string str;


        pthread_t thread[tn];

	getline(file, str);
	int docNumber = atoi(str.c_str());
        documentsNumber = docNumber;
	// do sth with number of docs

        int thread_counter = 0; // in which thread the string goes
	// read first sentence from txt
	while (getline(file, str))
	{
		// Process str
		//istringstream iss(str);
		string wordId;

                // word is the doc's id
                // found the doc's id, do stuff with it
                int id = atoi(str.c_str());
                
                // convert id to string to check length
                wordId = to_string(id);
                
                //cout << "id is: " << wordId << endl;
                
                str = str.substr(wordId.length(), str.length());
                cout << id << str << endl;
                
                Element *args = new Element(id, str);

                //Launch a thread
                pthread_create(&thread[thread_counter], NULL, call_from_thread, (void *)args);
                

                //Join the thread with the main thread
                pthread_join(thread[thread_counter], NULL);

                //reset counter if xyzs thread number
                thread_counter++;
                if(thread_counter == tn)
                {
                    thread_counter = 0;
                }
            
		cout << endl;
	}
} 

void readQueryFile(char *filename, int tn) {
        ifstream file(filename);
	string str;


        pthread_t thread[tn];

	getline(file, str);
	int docNumber = atoi(str.c_str());
        queriesNumber = docNumber;
        
        for ( auto it = InvertedIndex.begin(); it != InvertedIndex.end(); ++it ) 
        {

            it->second.vectorArray.resize(documentsNumber + queriesNumber); 

        }
        
        
	// do sth with number of docs
        queryTopK = new int[queriesNumber];
        int topKcounter = 0;

        int thread_counter = 0; // in which thread the string goes
	// read first sentence from txt
	while (getline(file, str))
	{
            
            
		// Process str
		//istringstream iss(str);
		string idString;
                string topkString;

                // found the query's id, do stuff with it
                int id = atoi(str.c_str());
                // convert id to string to check length
                idString = to_string(id);
                cout << "id of query is: " << id << endl;
                str = str.substr(idString.length(), str.length());
                //cout << id << str << endl;
                
                
                // found the query's topk, do stuff with it
                int topk = atoi(str.c_str());
                // convert topk to string to check length
                topkString = to_string(topk);
                cout << "topk of query is: " << topk << endl;
                str = str.substr(topkString.length(), str.length());
                //cout << id << str << endl;
                queryTopK[topKcounter] = topk;
                
                
                Element *args = new Element(id, str);

                //Launch a thread
                pthread_create(&thread[thread_counter], NULL, call_from_thread_query, (void *)args);
                

                //Join the thread with the main thread
                pthread_join(thread[thread_counter], NULL);

                //reset counter if xyzs thread number
                thread_counter++;
                if(thread_counter == tn)
                {
                    thread_counter = 0;
                }
            
		cout << endl;
                
                 //cout << "test" << endl;
                 
	}
}



void cosDist(int v1, int v2) {
    
        float sum = 0, metr1 = 0, metr2 = 0;
        for ( auto it = InvertedIndex.begin(); it != InvertedIndex.end(); ++it ) 
        {        
            sum = sum + (it->second.vectorArray.at(v1) * it->second.vectorArray.at(v2));
            metr1 = metr1 + (it->second.vectorArray.at(v1) * it->second.vectorArray.at(v1));
            metr2 = metr2 + (it->second.vectorArray.at(v2) * it->second.vectorArray.at(v2));
        }
        
        float distance = 0;
        distance = sum / (sqrt (metr1) * sqrt (metr2));
        
        
        cout << "Cosine Distance for " << v1+1 << "," << v2+1 << " is " << distance << endl;

}


/*
 an to query exei mia leksh pou dn yparxei sto map ti kanoume?
 isws ti vazoume sto map kai petame midenika sto vectorArray
 
 prepei se kapoia fash na doume sygxronismo stis call from threads OPOSDIPOTE
 */


int main() {
    
    int threads_num  = 3;
    
    readFile("Data.txt", threads_num);
    readQueryFile("Queries.txt", threads_num);

    printList();
    //cosDist(0,7);
    


    cout << "Man with boobssssss" << endl;

    return 0;
}

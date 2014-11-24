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

using namespace std;

static const int num_threads = 2;


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
        
        Thing() {
            crowd = 1;
            //head = NULL;
            firstNode = head;
        }
        
        void incrementCrowd() {
            crowd++;
        }
    
};


unordered_map <string, Thing> InvertedIndex;


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
            
        }
        else {
            
            Thing thing;
            InvertedIndex[word] = thing;
            
            // add first docID
            InvertedIndex[word].head = addToList(InvertedIndex[word].head, argz->id);
            
            //cout << "else" << endl;
        }

    }
    return NULL;
}


void readFile(char *filename, int tn) {
	ifstream file(filename);
	string str;


        pthread_t thread[tn];

	getline(file, str);
	int docNumber = atoi(str.c_str());
	// do sth with number of docs

        int thread_counter = 0; // in which thread the string goes
	// read first sentence from txt
	while (getline(file, str))
	{
		// Process str
		istringstream iss(str);
		string word;

                // word is the doc's id
                // found the doc's id, do stuff with it
                int id = atoi(str.c_str());
                str = str.substr(1, str.length());
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



int main() {
    
    int threads_num  = 3;
    
    readFile("Data.txt", threads_num);

    printMap();

    cout << "Man with boobssssss" << endl;

    return 0;
}

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
    
    int doc; // first element will be the number of docs plz
    struct list *next; // the next document

}list;

unordered_map <string, list*> InvertedIndex;


void addToList(list *old_node, int num){

    list *element = new list;
    element->next = NULL;
    element->doc = num;
    old_node->next = element;

}


//dat inverted Index
void invertedIndex() {

}



//This function will be called from a thread
void *call_from_thread(void *a) {
	
    Element *argz = (Element *) a;

    istringstream iss(argz->doc);
    string word;


    while(iss >> word)
    {

        cout << word << endl;

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
        cout << "Testing Git!" << endl;

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
    

    long a = 10;
    
    int threads_num  = 5;

    unordered_map <string, list*> m;
    list *el = new list;
    el->doc = 1;
    el->next=NULL;


    /*
    list *el2 = new list;
    el2->doc = 250;
    el2->next = NULL;
    el->next = el2;
    */
    addToList(el, 250);
    m["test"] = el;
    cout << m["test"]->next->doc << endl;
    


    

    cout << "Man with boobssssss" << endl;




    return 0;
}

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
int queriesNumber = 3;


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


class Numbers {
    
    public:
        
        int exists;
        float weight;
    
};


class Thing {
    
    public:
        
        int crowd;
        list *firstNode;
        list *head = new list;
        vector<Numbers> vectorArray;


        Thing() {
            crowd = 0;
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
unordered_map <int, int> term_freq; // how many terms in a document and the queries of course
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
                cout << it->second.vectorArray.at(i).exists << " ";
            }
            cout << endl;
        }
        cout << endl;
        
        for(int i = 0; i < 14; i++)
        {
            cout << term_freq[i] << " ";
        }
        

}


//This function will be called from a thread
void *call_from_thread(void *a) {
	
    Element *argz = (Element *) a;

    istringstream iss(argz->doc);
    string word;

    if(term_freq.find(argz->id - 1) != term_freq.end())
    {
        term_freq[argz->id - 1] = 0;
    }
    

    while(iss >> word)
    {

        //cout << word << endl;

        
        if (InvertedIndex.find(word) != InvertedIndex.end())
        {
            InvertedIndex[word].head = addToList(InvertedIndex[word].head, argz->id);
             //            
        }
        else {
                
            Thing thing;// = new Thing(documentsNumber);
            thing.initializeVector();
            InvertedIndex[word] = thing;
            
            // add first docID
            InvertedIndex[word].head = addToList(InvertedIndex[word].head, argz->id);
            
            //cout << "else" << endl;
        }
        
        if(InvertedIndex[word].vectorArray.at(argz->id - 1).exists == 0)
        {
            InvertedIndex[word].incrementCrowd();
        }
        
        InvertedIndex[word].vectorArray.at(argz->id - 1).exists ++;
        /*
        if(term_freq[argz->id - 1] < InvertedIndex[word].vectorArray.at(argz->id - 1).exists)
        {
            term_freq[argz->id - 1] = InvertedIndex[word].vectorArray.at(argz->id - 1).exists;
        }
        */
        term_freq[argz->id - 1]++;
        


    }
 // kali cout! cout << "term frequency: " << term_freq[argz->id - 1] << endl;
    return NULL;
} 



void compute_word_weight(string word, int start, int end)
{
    for(int i = start; i < end; i++)
    {
        int id = i;
        if(InvertedIndex[word].vectorArray.at(id).exists != 0)
        {
            //computing the final weight
            double td;

            double idf;

            double weight;
            
            td = (double)InvertedIndex[word].vectorArray.at(id).exists / (double)term_freq[id];

            idf = log((double)documentsNumber / (double)InvertedIndex[word].crowd);

            weight = td * idf;
            InvertedIndex[word].vectorArray.at(id).weight = weight;
        }
        else
        {
            InvertedIndex[word].vectorArray.at(id).weight = 0.0;
        }
    }
    

    
    
}



vector<Numbers> compute_union(vector<Numbers> v1, vector<Numbers> v2)
{
    
    vector<Numbers> univector;
    univector.resize(documentsNumber);
    
    
    for(int i = 0; i < documentsNumber; i++)
    {
        if((v1.at(i).exists != 0) || (v2.at(i).exists != 0))
        {
            univector.at(i).exists = 1;
        }
    }
    
    return univector;
}



// Cosine Distance
void cosDist(int v1, int v2) {
    
        double sum = 0, metr1 = 0, metr2 = 0;
        for ( auto it = InvertedIndex.begin(); it != InvertedIndex.end(); ++it ) 
        {        
            sum = sum + (it->second.vectorArray.at(v1).weight * it->second.vectorArray.at(v2).weight);
            metr1 = metr1 + (it->second.vectorArray.at(v1).weight * it->second.vectorArray.at(v1).weight);
            metr2 = metr2 + (it->second.vectorArray.at(v2).weight * it->second.vectorArray.at(v2).weight);
        }
        
        double distance = 0;
        if(metr1 != 0 && metr2 != 0)
        {
            distance = sum / (sqrt (metr1) * sqrt (metr2));
        }
        
        
        cout << "Cosine Distance for " << v1+1 << "," << v2+1 << " is " << distance << endl;

}





int queryCounter = 0;
//This function will be called from a thread adds query terms to the Inverted Index
// to issstring na fygei na mpei mia metavliti sti thesi tou oxi 3
void *call_from_thread_query(void *a) {
	
    Element *argz = (Element *) a;

    // adding query into the Inverted Index
    istringstream iss(argz->doc);
    string word;

    int column_id = documentsNumber + queryCounter;
    if(term_freq.find(column_id) != term_freq.end())
    {
      term_freq[column_id] = 0;
    }

    while(iss >> word)
    {

        
        //cout << word << endl;

        if (InvertedIndex.find(word) != InvertedIndex.end()) {
            
            InvertedIndex[word].vectorArray.at(column_id).exists ++;
            compute_word_weight(word, 0, documentsNumber);
 
        }
        term_freq[column_id] ++;
        


    }
    
    int wordcount = 0;
    // Computing weight of every term in the query
    istringstream idd(argz->doc);
    while(idd >> word)
    {
        if (InvertedIndex.find(word) != InvertedIndex.end()) {
            compute_word_weight(word, documentsNumber + queryCounter, column_id +1);
        }
        wordcount ++;
    }
    cout <<endl;
    
    // Computing cosine distance of the query and the relative documents
    // na apofeugoume apostaseis pou exoume idi ypologisei
    
    istringstream iff(argz->doc);
    string word2;
    
    // count tis lekseis stin panw while gia ti for
    int while_count = 0;
    int temp_wordcount = wordcount;
    iff >> word;
    vector<Numbers> univector;
    
    while (InvertedIndex.find(word) == InvertedIndex.end() && while_count < temp_wordcount) {
        
        iff >> word;
        wordcount --;
    }
    
    if(InvertedIndex.find(word) != InvertedIndex.end())
    {
        univector = InvertedIndex[word].vectorArray;

        if(wordcount > 1)
        {
            for(int count = 0; count < wordcount - 1; count++)
            {
                iff >> word2;

                if (InvertedIndex.find(word2) != InvertedIndex.end()) {

                    univector = compute_union(univector, InvertedIndex[word2].vectorArray);

                }
            }
        }

        for(int i = 0; i < documentsNumber; i++)
        {
            if(univector.at(i).exists != 0)
            {
                cosDist(i, column_id);
            }
        }

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
               // kali cout! cout << id << str << endl;
                
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
                str = str.substr(idString.length() + 1, str.length());
                //cout << id << str << endl;
                
                // found the query's topk, do stuff with it
                int topk = atoi(str.c_str());
                // convert topk to string to check length
                topkString = to_string(topk);
                cout << "topk of query is: " << topk << endl;
                str = str.substr(topkString.length() + 1, str.length());
                //cout << id << str << endl;
                queryTopK[topKcounter] = topk;
                
                cout << "after party: " << str << endl;
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






/*
 an to query exei mia leksh pou dn yparxei sto map ti kanoume?
 isws ti vazoume sto map kai petame midenika sto vectorArray
 
 prepei se kapoia fash na doume sygxronismo stis call from threads OPOSDIPOTE
 */


int main() {
    
    int threads_num  = 1;
    
    readFile("Data.txt", threads_num);
    readQueryFile("Queries.txt", threads_num);

    //printList();
    //cosDist(0,7);
    //cosDist(10, 11);

    /*
    vector<Numbers> vec;
    vec = compute_union(InvertedIndex["first"].vectorArray, InvertedIndex["document"].vectorArray);
    for(int i = 0; i < documentsNumber; i++)
    {
        cout << vec.at(i).exists << "  ";
    }
    cout << endl << endl;
    */
    
    cout << "Man with boobssssss" << endl;

    return 0;
}

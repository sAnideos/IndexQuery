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
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <sys/time.h>  
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>

using namespace std;



int test_var = 0;

static const int num_threads = 0;
int documentsNumber = 0;
int queriesNumber = 0;
int count_docs = 0;
int count_queries = 0;

double index_duration = 0.0;
double queries_duration = 0.0;
double reading_duration = 0.0;
pthread_mutex_t mutexInverIndex;
pthread_mutex_t mutexInverIndex2;
pthread_mutex_t mutexQuery;
pthread_mutex_t mutexQuery1;
pthread_mutex_t mutexQuery2;
pthread_mutex_t mutexQuery3;
pthread_mutex_t mutexQuery4;
pthread_mutex_t mutexQuery5;
pthread_mutex_t mutexQuery6;




class Element {

    public:

        int id;
        int topk;
        string doc;


        Element(int i, int t, string d){

            id = i;
            doc = d;
            topk = t;
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
unordered_map <string, int> stopwords; // stopwords

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

bool searchList(list *head, int id) {
    list *node = head;
    while (node->next != NULL) {
        if (node->next->docID == id) {
            return true;
        }
        node = node->next;
    }
    return false;
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

double checking_duration = 0.0;

string checkWord(string word) {
    struct rusage usage;
//        struct timeval start, end;
//        getrusage(RUSAGE_SELF, &usage);
//        start = usage.ru_stime;
    
    
        // remove punctuation
        char ch = word.back();
        if (ispunct(ch)) {
            word = word.substr(0, word.size()-1);
        }
        // toLower case for all words in index
        transform(word.begin(), word.end(), word.begin(), ::tolower);
        // remove stopwords
        if (stopwords.find(word) != stopwords.end()) {
            word = "";
        }
        
        
//        getrusage(RUSAGE_SELF, &usage);
//        end = usage.ru_stime;
//
//        double timer_spent = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/1000000.0;
//        checking_duration = checking_duration + timer_spent*1000.0;
        
        
        return word;
}


//This function will be called from a thread
void *call_from_thread(void *a) {
    
    
    pthread_t self;
    self = pthread_self();

    struct rusage usage;
    struct timeval start, end;
    getrusage(RUSAGE_SELF, &usage);
    start = usage.ru_stime;

	
    Element *argz = (Element *) a;

    istringstream iss(argz->doc);
    string word;

    if(term_freq.find(argz->id - 1) != term_freq.end())
    {
        term_freq[argz->id - 1] = 0;
    }
    
    unordered_map <string, int> map;
    int count = 0;
    
    while(iss >> word)
    {
        word = checkWord(word);
        if (word == "") {
            continue;
        }
        
        //pthread_mutex_lock (&mutexInverIndex);
        
        
        count++;
        
        
        
        
        
        
//        if (map.find(word) != map.end()) // an yparxei word sto index
//        {
////            if (!searchList(InvertedIndex[word].firstNode, argz->id)) {
////                InvertedIndex[word].head = addToList(InvertedIndex[word].head, argz->id);
////            }
//                     
//        }
        if (map.find(word) == map.end()){
            
            //pthread_mutex_lock (&mutexInverIndex);
            
            map[word] = 1;
            
            //pthread_mutex_unlock (&mutexInverIndex);
            
            //cout << "else" << endl;
        }
        else {
            map[word]++;
        }
        
//        if(InvertedIndex[word].vectorArray.at(argz->id - 1).exists == 0)
//        {
//            //pthread_mutex_lock (&mutexInverIndex2);
//            InvertedIndex[word].incrementCrowd();
//            //pthread_mutex_unlock (&mutexInverIndex2);
//        }
//        
//        InvertedIndex[word].vectorArray.at(argz->id - 1).exists ++;
//
//        /*
//        if(term_freq[argz->id - 1] < InvertedIndex[word].vectorArray.at(argz->id - 1).exists)
//        {
//            term_freq[argz->id - 1] = InvertedIndex[word].vectorArray.at(argz->id - 1).exists;
//        }
//        */
//        term_freq[argz->id - 1]++;
        
        //pthread_mutex_unlock (&mutexInverIndex);


    }
    // kali cout! cout << "term frequency: " << term_freq[argz->id - 1] << endl;
    
    pthread_mutex_lock (&mutexInverIndex);
    
    for ( auto it = map.begin(); it != map.end(); ++it ) {
        
        word = it->first;
        if (InvertedIndex.find(word) != InvertedIndex.end()) // an yparxei word sto index
        {
            if (!searchList(InvertedIndex[word].firstNode, argz->id)) {
                InvertedIndex[word].head = addToList(InvertedIndex[word].head, argz->id);
            }
                     
        }
        else {
                
            Thing thing;
            thing.initializeVector();
            
            //pthread_mutex_lock (&mutexInverIndex);
            
            InvertedIndex[word] = thing;
            
            // add first docID
            InvertedIndex[word].head = addToList(InvertedIndex[word].head, argz->id);
            
            //pthread_mutex_unlock (&mutexInverIndex);
            
            //cout << "else" << endl;
        }

        InvertedIndex[word].incrementCrowd();
 
        
        InvertedIndex[word].vectorArray.at(argz->id - 1).exists = it->second;

        /*
        if(term_freq[argz->id - 1] < InvertedIndex[word].vectorArray.at(argz->id - 1).exists)
        {
            term_freq[argz->id - 1] = InvertedIndex[word].vectorArray.at(argz->id - 1).exists;
        }
        */
        
        
        
    }
    term_freq[argz->id - 1] = count;
    pthread_mutex_unlock (&mutexInverIndex);
    
    getrusage(RUSAGE_SELF, &usage);
    end = usage.ru_stime;
    
    double timer_spent = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/1000000.0;
    index_duration = index_duration + timer_spent*1000.0;
    //printf("Started at: %ld.%lds\n", start.tv_sec, start.tv_usec);
    //printf("Ended at: %ld.%lds\n", end.tv_sec, end.tv_usec);
    //cout << "Time spent: " << timer_spent << " ms" << endl;

    
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
double cosDist(int v1, int v2) {
    
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
        
        
        //cout << "Cosine Distance for " << v1+1 << "," << v2+1 << " is " << distance << endl;
        
        return distance;

}





//int queryCounter = 0;
//This function will be called from a thread adds query terms to the Inverted Index
// to issstring na fygei na mpei mia metavliti sti thesi tou oxi 3
void *call_from_thread_query(void *a) {
    

    struct rusage usage;
    struct timeval start, end;
    getrusage(RUSAGE_SELF, &usage);
    start = usage.ru_stime;
    
    
    
    ofstream outFile;
    Element *argz = (Element *) a;
    
    
    
    int topk = argz->topk;
    Numbers *topkArray = new Numbers[topk]; // to be sorted so as to get top K results
    
    if (!outFile.is_open()) {
        //remove("out.txt");
        outFile.open("out.txt", ios::app);
    }
    
    // adding query into the Inverted Index
    istringstream iss(argz->doc);
    string word;

    int column_id = documentsNumber + argz->id - 1;
    if(term_freq.find(column_id) != term_freq.end())
    {
      term_freq[column_id] = 0;
    }

    
    while(iss >> word)
    {
        
        
        pthread_mutex_lock (&mutexQuery);

        if (InvertedIndex.find(word) != InvertedIndex.end()) {
            InvertedIndex[word].vectorArray.at(column_id).exists ++;
            compute_word_weight(word, 0, documentsNumber);
        }
        term_freq[column_id] ++;

        
        pthread_mutex_unlock (&mutexQuery);       


    }
    
    
    int wordcount = 0;
    // Computing weight of every term in the query
    istringstream idd(argz->doc);
    
    while(idd >> word)
    {
        
        if (InvertedIndex.find(word) != InvertedIndex.end()) {
            pthread_mutex_lock (&mutexQuery1);
            compute_word_weight(word, column_id, column_id +1);
            pthread_mutex_unlock (&mutexQuery1);
        }
        else
        {
            
        }
        wordcount ++;
    }

    
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
        temp_wordcount --;
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

        int topK_counterino = 0;
        for (int i = 0; i < topk; i++) {
            topkArray[i].weight = 0;
            topkArray[i].exists = -1;
        }
        for(int i = 0; i < documentsNumber; i++) // i variable is document id
        {
            if(univector.at(i).exists != 0)
            {
                if (topK_counterino == topk) {
                    float similarity = (float)cosDist(i, column_id);
                    if (similarity > topkArray[topk-1].weight) {
                        
                        topkArray[topk-1].exists = i;
                        topkArray[topk-1].weight = similarity;
                        
                        sort(topkArray, topkArray + topk,
                            [](Numbers const & a, Numbers const & b) -> bool
                            { return a.weight > b.weight; } 
                        );
                        
                    }
                }
                else {
                    topkArray[topK_counterino].exists = i;
                    topkArray[topK_counterino].weight = (float)cosDist(i, column_id);
                    if (topK_counterino == topk-1) {// an topk < valid docs tote pote 8a kanei sort?
                        // if array is full, sort array
                        sort(topkArray, topkArray + topk,
                            [](Numbers const & a, Numbers const & b) -> bool
                            { return a.weight > b.weight; } 
                        );
                    }
                    topK_counterino++;
                }
                
            }
        }
        sort(topkArray, topkArray + topk,
                            [](Numbers const & a, Numbers const & b) -> bool
                            { return a.weight > b.weight; } 
                        );
        
        outFile << argz->id << ". ";// <<"\n";
        cout << "---------------- PRINTING FOR QUERY " << argz->id << " (topk=" << argz->topk << ")" << "----------------------------" << endl;
        for (int i = 0; i < topK_counterino; i++) {
            cout << "Doc " << topkArray[i].exists+1 << " with cosDist " << topkArray[i].weight << endl;
            outFile << topkArray[i].exists+1;// << ", ";
            if (i < topK_counterino-1) {
                outFile << ", ";
            }
        }
        outFile << "\n";
        cout << "---------------- ENDOF PRINTING FOR THIS QUERY ----------------------------" << endl;
        
    }
    
    
    getrusage(RUSAGE_SELF, &usage);
    end = usage.ru_stime;
    
    double timer_spent = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/1000000.0;
    queries_duration = queries_duration + timer_spent*1000.0;
    
    
    return NULL;
}







void readFile(char *filename, int tn) {
    
        struct rusage usage;
        struct timeval start, end;
        getrusage(RUSAGE_SELF, &usage);
        start = usage.ru_stime;
    
    
	ifstream file(filename);
	string str;


        pthread_t thread[tn];

	getline(file, str);
	int docNumber = atoi(str.c_str());
        documentsNumber = docNumber;
        count_docs = documentsNumber;
	// do sth with number of docs

        int thread_counter = 0; // in which thread the string goes
	// read first sentence from txt
	while (getline(file, str))
	{
		// Process str
		string wordId;

                // word is the doc's id
                // found the doc's id, do stuff with it
                int id = atoi(str.c_str());
                
                // convert id to string to check length
                wordId = to_string(id);
                
                
                str = str.substr(wordId.length(), str.length());
                
                Element *args = new Element(id, 0, str);

                //Launch a thread
                pthread_create(&thread[thread_counter], NULL, call_from_thread, (void *)args);
                

                //Join the thread with the main thread

                //reset counter if xyzs thread number
                thread_counter++;
                if(thread_counter == tn && count_docs >= tn)
                {
                    for(int i = 0; i < tn; i++)
                    {
                        pthread_join(thread[i], NULL);
                    }
                    thread_counter = 0;
                    count_docs = count_docs - tn;
                }
                if(count_docs < tn) {
                    for(int i = 0; i < count_docs; i++)
                    {
                        pthread_join(thread[i], NULL);
                    }
                }
                
	}
        
        getrusage(RUSAGE_SELF, &usage);
        end = usage.ru_stime;

        double timer_spent = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/1000000.0;
        reading_duration = reading_duration + timer_spent*1000.0;
        
        
} 

void readQueryFile(char *filename, int tn) {
    
        struct rusage usage;
        struct timeval start, end;
        getrusage(RUSAGE_SELF, &usage);
        start = usage.ru_stime;
    
    
        ifstream file(filename);
	string str;


        pthread_t thread[tn];


	getline(file, str);
	int docNumber = atoi(str.c_str());
        queriesNumber = docNumber;
        count_queries = queriesNumber;
        
        for ( auto it = InvertedIndex.begin(); it != InvertedIndex.end(); ++it ) 
        {

            it->second.vectorArray.resize(documentsNumber + queriesNumber); 

        }
        

	// do sth with number of docs
        int topKcounter = 0;

        int thread_counter = 0; // in which thread the string goes
	// read first sentence from txt
	while (getline(file, str))
	{
                       
		// Process str
		string idString;
                string topkString;

                // found the query's id, do stuff with it
                int id = atoi(str.c_str());
                // convert id to string to check length
                idString = to_string(id);
                
                str = str.substr(idString.length() + 1, str.length());
                
                // found the query's topk, do stuff with it
                int topk = atoi(str.c_str());
                // convert topk to string to check length
                topkString = to_string(topk);
                
                str = str.substr(topkString.length() + 1, str.length());
                
                Element *args = new Element(id, topk, str);
                 

                //Launch a thread
                pthread_create(&thread[thread_counter], NULL, call_from_thread_query, (void *)args);
                //cout<< "thread_counter: " << thread_counter << endl; 
                
                //Join the thread with the main thread
                //pthread_join(thread[thread_counter], NULL);

                //reset counter if xyzs thread number
                thread_counter++;
                if(thread_counter == tn && count_queries >= tn)
                {
                    for(int i = 0; i < tn; i++)
                    {
                        pthread_join(thread[i], NULL);
                    }
                    thread_counter = 0;
                    count_queries = count_queries - tn;
                }
                if(count_queries < tn) {
                    for(int i = 0; i < count_queries; i++)
                    {
                        pthread_join(thread[i], NULL);
                    }
                }
               
                 
	}
        
        for(int i = 0; i < 1; i++)
        {
            //cout << "Before join" << endl;
            pthread_join(thread[i], NULL);
            
        }
        getrusage(RUSAGE_SELF, &usage);
        end = usage.ru_stime;

        double timer_spent = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/1000000.0;
        reading_duration = reading_duration + timer_spent*1000.0;
}


void createStopwords() {
    ifstream file("stopwords.txt");
    string word;
    
//    struct rusage usage;
//        struct timeval start, end;
//        getrusage(RUSAGE_SELF, &usage);
//        start = usage.ru_stime;
    
    while (getline(file, word)) {
        
        stopwords[word] = 0;
        
    }
//    getrusage(RUSAGE_SELF, &usage);
//        end = usage.ru_stime;
//
//        double timer_spent = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/1000000.0;
//        reading_duration = reading_duration + timer_spent*1000.0;
//        cout << "time spend reading stopwords: " << reading_duration << endl;
}



/*
 an to query exei mia leksh pou dn yparxei sto map ti kanoume?
 isws ti vazoume sto map kai petame midenika sto vectorArray
 
 prepei se kapoia fash na doume sygxronismo stis call from threads OPOSDIPOTE
 */


int main() {
    
    
    int threads_num;
    cout << "Please give number of threads: ";
    cin >> threads_num;
        
    struct timeval tim;
    
    auto t_start = chrono::high_resolution_clock::now();
    
    gettimeofday(&tim, NULL);  
    double t1=tim.tv_sec+(tim.tv_usec/1000000.0);  
    createStopwords();
    
    pthread_mutex_init(&mutexInverIndex, NULL);
    pthread_mutex_init(&mutexInverIndex2, NULL);
    pthread_mutex_init(&mutexQuery, NULL);
    pthread_mutex_init(&mutexQuery1, NULL);
    
    
    readFile("Data.txt", threads_num);
    cout << "Duration time for index creation passed: " << index_duration << " ms\n";
    
    readQueryFile("Queries.txt", threads_num);


    //printMap();
    
    auto t_end = chrono::high_resolution_clock::now();
    gettimeofday(&tim, NULL);  
    double t2=tim.tv_sec+(tim.tv_usec/1000000.0);
    
    pthread_mutex_destroy(&mutexInverIndex);
    pthread_mutex_destroy(&mutexInverIndex2);
    pthread_mutex_destroy(&mutexQuery);
    pthread_mutex_destroy(&mutexQuery1);
    
    cout << "Wall clock time passed: "
              << std::chrono::duration<double, std::milli>(t_end-t_start).count()
              << " ms\n";
    cout << "Pure time passed: " << (t2-t1)*1000 << " ms\n";
    cout << "Duration time for reading files passed: " << reading_duration << " ms\n";
    cout << "Duration time for index creation passed: " << index_duration << " ms\n";
    cout << "Duration time for queries passed: " << queries_duration << " ms\n";
    cout << "Duration time overall in threads passed: " << index_duration+queries_duration << " ms\n";
    cout << "Duration time overall in checking words for stuff: " << checking_duration << " ms\n";
    
    
    cout << "Man with boobssssss" << endl;
    
    

    return 0;
    
}
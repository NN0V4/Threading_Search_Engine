//SARAH QASIM 143884
//YAQEEN JEHAD 148110
// Code sample without race condition

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <pthread.h>
#include <algorithm>
#include <cstdlib>
using namespace std;


// Global variables
int TotalFound = 0;
int AboveThreshold = 0;
int EqualsThreshold = 0;
int BelowThreshold = 0;
int numthreads ;
char CHAR;
int TH;
vector <string> filenames;  // List to store filenames and their counts

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  // Mutex for synchronization

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

// Function to count occurrences of CHAR in a file
int countOccurrences(const string& filename, char CHAR) {
    ifstream file(filename);
    
    int count = 0;
    char c;
    while (file.get(c)) {
        if (c == CHAR) {
            count++;
        }
    }

    file.close();
    return count;
}


/************************************************************************************/
/************************************************************************************/

void* DoNothing(void* arg){
    long Tid = (long)arg;
 cout << "TID" << Tid <<"--> I Have Nothing To Do "<<endl;
 pthread_exit(NULL);
 
}

// Function for each worker thread
void* threadFunction(void* arg) {
    long Tid = (long)arg;

    // Calculate the range of files for this thread
    int filesforThread = (filenames.size() % numthreads==0)? filenames.size() / numthreads : 1  ; 
    int first = Tid * filesforThread;
    int end = (Tid + 1 == numthreads ) ? filenames.size() : (Tid + 1) * filesforThread;

    cout << "TID" << Tid << " --> Starting thread firstItem=" << first << ", lastItem=" << end <<endl;

    // Process files in the assigned range
    for (int i = first; i < end; i++) {
        string filename = filenames[i];
        int occurrences = countOccurrences(filename, CHAR);

         pthread_mutex_lock(&mutex);  // Lock the critical section
        // Update global variables based on occurrences
        TotalFound += occurrences;
        if (occurrences > TH) {
            AboveThreshold++;
        } else if (occurrences == TH) {
            EqualsThreshold++;
        } else {
            BelowThreshold++;
        }
        // Update the list of filenames and their counts
        filenames[i] = " (found = "+ to_string(occurrences)  + ")" + " [" + filename + "]";
        pthread_mutex_unlock(&mutex);  // Unlock the critical section

        // Print statistics for each file
        cout << "TID" << Tid << " --> File: " << filename << ", ("<<CHAR<<") found=" << occurrences << endl;
    }

    cout << "TID" << Tid << " --> Ending thread firstItem=" << first << ", lastItem=" << end <<endl;

    pthread_exit(NULL);
}

/************************************************************************************/
/************************************************************************************/
/************************************************************************************/

int main(int argc, char* argv[]) {
	
    	int T = strtol(argv[1],NULL,10) ; // the number of threads
    	 char c =argv[2][0];// the char to search for
    	
    	
	ifstream iFile;
	ofstream oFile;
	iFile.open("in.txt");
	oFile.open("out.txt");
	
	numthreads=T;
	CHAR = c;
	
int N;// N is the number of files & TH is the threshold

iFile>>N>>TH; 


    
    // Pop filenames vector
    string filename;
    while (iFile >> filename) {
        filenames.push_back(filename);
    }

    // Create an array of pthreads
    pthread_t *thr = new pthread_t [T];

if (N %T ==0){
cout <<"Main --> Search Engine searching for ("<<CHAR<<") in "<<N<<" files, using "<<T<<" threads (with threshold= "<<TH<<")"<<endl;
    // Create threads
    for (long i=0;i<T;i++){
	pthread_create(&thr[i],NULL,threadFunction,(void*)i);
    }
    
    // Wait for all threads to finish
    for (int i = 0; i < T; i++) {
        pthread_join(thr[i], NULL);
    }


/***************************/

} else if (N<T){
cout <<"Main --> Search Engine searching for ("<<CHAR<<") in "<<N<<" files, using "<<T<<" threads (with threshold= "<<TH<<")"<<endl;
    // Create threads
    for (long i=0;i<N;i++){
	pthread_create(&thr[i],NULL,threadFunction,(void*)i);
    }
    for (long i=N ; i< T;i++){
    	pthread_create(&thr[i],NULL,DoNothing,(void*)i);
    }
    
    // Wait for all threads to finish
    for (int i = 0; i < T; i++) {
        pthread_join(thr[i], NULL);
    }}else if (N % T != 0) {
        cerr << "Error: number of files must divide number of threads" <<endl;
        return 1;
    }
 

   
    // Print final statistics
     cout << "Main --> TotalFound=" <<TotalFound<< ", AboveThreshold=" << AboveThreshold
              << ", EqualsThreshold=" <<EqualsThreshold<< ", BelowThreshold=" <<BelowThreshold<<endl;
    cout << "Main --> Sorted list of files in out.txt"<<endl;

    // Sort filenames based on occurrences in descending order
    sort(filenames.rbegin(), filenames.rend());

    // Print sorted list of files to out.txt
   
   
         int count=0;
    oFile << "Sorted list of files:" << endl;
    for (const auto& entry : filenames) {
        count++;
        oFile <<count<<"."<<entry <<endl;
    }

    oFile.close();
    iFile.close();
    return 0;
    
}

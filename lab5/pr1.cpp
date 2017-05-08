#include <unistd.h>
#include <pthread.h>
#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;

void* run(void* arg);

const int BUF_SIZE = 256;
int result;

int main() {
	int* error;
	pthread_t tid;
	char buffer[BUF_SIZE];

	cin.getline(buffer, BUF_SIZE);

	pthread_create(&tid, NULL, run, (void*) buffer);
	pthread_join(tid, (void**) &error);
	
	cout << (*error ? "Failure" : "Successfully") << "\n";
}

void* run(void* arg) {
	cout << "Thread ID: " << pthread_self() << "; process ID: " << getpid() << ";\n";
	result = 0;

	stringstream fileName;
	fileName << "file" << pthread_self() << ".dat";
	ofstream file;
	file.open(fileName.str().c_str());
	
	if (file.is_open()) {
		file << (char*) arg;
		file.close();
	} else {
		result = 1;	
	}
	
	return (void*) &result;
}

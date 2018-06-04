#include <iostream>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

//Struct with all the info for a student
struct Student {
  char* fname = new char[100];
  char* lname = new char[100];
  int id;
  float gpa;
  Student* next = NULL;
  Student* previous = NULL;
};

//prototypes
bool hashTable(Student* table[], int &size, Student* student);
void print(Student* table[], int size);

//variables
bool running = true;
bool rehash  = false;

int main() {

  Student* *table = new Student*[100];
  int size = 100;
  //initialize every spot in the table to be NULL
  for(int i = 0; i < 100; i++) {
    table[i] = NULL;
  }
  
  while(running != false) {
    cout << "You can 'add' 'delete' 'print' 'randomize' or 'quit'" << endl;
    char answer[10];
    cin.getline(answer, 10);
    
    if(strcmp(answer, "add") == 0) {
      //Most of this is the same as student list other than the rehash
      Student* student = new Student;
      
      //read in all the data
      cout << "What is the students first name?" << endl;
      cin >> student->fname;
      cin.ignore(10, '\n');
      cout << "What is the students last name?" << endl;
      cin >> student->lname;
      cin.ignore(10, '\n');
      cout << "What is the students id number?" << endl;
      cin >> student->id;
      cin.ignore(10, '\n');
      cout << "What is the studetns GPA?" << endl;
      cin >> student->gpa;
      cin.ignore(10, '\n');

      rehash = hashTable(table, size, student);
      
    }
    else if(strcmp(answer, "print") == 0) {
      print(table, size);
    }
    else if(strcmp(answer, "randomize") == 0) {
      int inputNum = 0;
      char* input = new char[200];
      char* fnames[200];
      char* lnames[200];
      bool empty = false;
      int index;
      
      cout << "How many students are you adding?" << endl;
      cin.getline(input, 100);
      inputNum = atoi(input);

      for(int i = 0; i < inputNum; i++) {
	ifstream file;
	file.open("Fnames.txt");
	if(file.is_open()) {
	  for(int i = 0; file.eof() == false; i++) {
	    file.getline(input, 200);
	    fnames[i] = new char[200];
	    strcpy(fnames[i], input);
	  }
	}
	else {
	  cout << "could not find the file..." << endl;
	}
	
	ifstream file2("Lnames.txt");
	if(file.is_open()) {
	  for(int i = 0; file2.eof() == false; i++) {
	    file2.getline(input, 200);
	    lnames[i] = new char[200];
	    strcpy(lnames[i], input);
	  }
	}
	else {
	  cout << "could not find the file..." << endl;
	}

	Student* student = new Student;
	srand(time(NULL)/(i+1));
	index = rand() % 100;
	student->fname = fnames[index];
	srand(time(NULL)/(i+1));
	index =rand() % 100;
	student->lname = lnames[index];
	index = rand();
	student->gpa = index % 4;
	student->id = index % 1000000;

	rehash = hashTable(table, size, student);
      }
    }
    else if(strcmp(answer, "delete") == 0) {
      cout << "What is the id of the student?" << endl;
      int id;
      cin >> id;
      cin.ignore();

      int index= id % size;
      delete table[index];
      table[index] = NULL;
    }
    else if(strcmp(answer, "quit") == 0) {
      running = false;
    }
    else {
      cout << "That wasn't an option..." << endl;
    }

    //If this is true then you need to make the table bigger
    if(rehash == true) {
      Student* old[size];
      for(int i = 0; i < size; i++) {
	old[i] = table[i];
      }
      size = size * 2; //make the table double the size
      delete[] table;
      table = new Student*[size];
      for(int i = 0; i < size; i++) {
	table[i] = NULL;
      }
      for(int i = 0; i < size/2; i++) {
	if(old[i] != NULL) {
	  if(old[i]->next != NULL) {
	    if(old[i]->next->next != NULL) {
	      if(old[i]->next->next->next != NULL) {
		hashTable(table, size, old[i]->next->next->next);
		old[i]->next->next->next = NULL;
	      }
	      hashTable(table, size, old[i]->next->next);
	      old[i]->next->next = NULL;
	    }
	    hashTable(table, size, old[i]->next);
	    old[i]->next = NULL;
	  }
	  hashTable(table, size, old[i]);
	}
      }
      rehash = false;
    }
  }
  return 0;
}

//size is by reference incase you need to make the table larger
bool hashTable(Student* table[], int &size, Student* student) { 
  int index;
  //the id number modulus the size of the table gives you a unique index spot
  index = student->id % size;
  
  //if there is no student in that slot, just plop them there
  if(table[index] == NULL) {
    table[index] = student;
  }
  else { //that slot is null
    //if the student at index's next is null then place new student there
    if(table[index]->next == NULL) {
      table[index]->next = student;
      student->previous = table[index]->next;
    }
    //keep on checking next for a null
    else if(table[index]->next->next == NULL) {
      table[index]->next->next = student;
      student->previous = table[index]->next->next;
    }
    else {
      table[index]->next->next->next = student;
      return true;
    }
  }

  int num = 0;
  for(int i = 0; i < size; i++) {
    if(table[i] != NULL) {
      num++;
    }
  }
  if(num >= size/2) {
    return true;
  }
}


//print all of the students in the table
void print(Student* table[], int size) {
  for(int i = 0; i < size; i++) {
    if(table[i] != NULL) {
      cout << table[i]->fname << " ";
      cout << table[i]->lname << ": ";
      cout << table[i]->id << ", ";
      cout << table[i]->gpa << endl;
      if(table[i]->next != NULL) {
	cout << table[i]->fname << " ";
	cout << table[i]->lname << ": ";
	cout << table[i]->id << ", ";
	cout << table[i]->gpa << endl;
	if(table[i]->next->next != NULL) {
	  cout << table[i]->fname << " ";
	  cout << table[i]->lname << ": ";
	  cout << table[i]->id << ", ";
	  cout << table[i]->gpa << endl;
	}
      }
    }
  }
}

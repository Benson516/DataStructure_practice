#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <math.h>

using std::string;
using std::vector;
using std::list;
using std::cin;
using std::cout;

// This is the content (type: mapping) of the hash table
struct Contact{
	Contact(){
	}
	Contact(int number, string name): number(number), name(name)
	{}
	
	int number;
	string name;
	// Contact* next_ptr;
};

class Hashing{
private:
	long long a,b;
	int p;

public:	
	int m;
	// Chain
	vector<list<Contact>> hash_table;
	int numberOfKeys; // number of content
	double loadFactor;
	
	Hashing(){
		m = 100000;
		hash_table.resize(m);
		//
		p = findNextPrime(10000000); // 10^7
		a = rand()%p + 1;
		b = rand()%(p+1) + 0;
		//
		numberOfKeys = 0;
		loadFactor = 0;
	}
	Hashing(int m): m(m) {
		hash_table.resize(m);
		//
		p = findNextPrime(10000000); // 10^7
		a = rand()%p + 1;
		b = rand()%(p+1) + 0;
		//
		numberOfKeys = 0;
		loadFactor = 0;
	}
	
	// hash number
	int hash(int number){
		// return (number % m);
		return ((a*number + b)%p)%m;
	}
	
	// Find the next prime number
	//////////////////////////////////////
	int findNextPrime(int num){		
		if ( (num%2) == 0) // even
			num ++;
		
		int i = 0;
		int num_1 = num;
		while ( i<= 100){
			// std::cout << num_1 << " ";
			if (isPrime(num_1)){
				return num_1;
			}
			i++;
			num_1 += 2;
		}
		return -1; // not found
	}
	bool isPrime(int num){
		int num_sqrt = (int)sqrt((double)num);
		
		if ((num%2) == 0)
			return false;
		for (int i=3; i<=num_sqrt; i+=2){
			if (num%i==0)
				return false;
		}
		return true;
	}
	//////////////////////////////////////
	
	// Functions for Chain
	///////////////////////////////
	bool HasKey(int number){
		list<Contact>::iterator list_it; // pointer to the element of a list
		list<Contact> &L = hash_table[hash(number)];
	
		for (list_it = L.begin(); list_it != L.end(); ++list_it){
			if (list_it->number == number)
				return true;
		}
		return false;
	}
	string Get(int number){
		list<Contact>::iterator list_it; // pointer to the element of a list
		list<Contact> &L = hash_table[hash(number)];
	
		for (list_it = L.begin(); list_it != L.end(); ++list_it){
			if (list_it->number == number){
				return list_it->name;
			}
		}
		return "not found";
	}
	void Set(int number, string name){
		list<Contact>::iterator list_it; // pointer to the element of a list
		list<Contact> &L = hash_table[hash(number)];
	
		for (list_it = L.begin(); list_it != L.end(); ++list_it){
			if (list_it->number == number){
				list_it->name = name;
				return;
			}
		}	
		L.push_back(Contact(number,name));
		numberOfKeys++;
		loadFactor = ((double)numberOfKeys)/((double)m); 
	}
	void Set(Contact contact_in){
		list<Contact>::iterator list_it; // pointer to the element of a list
		list<Contact> &L = hash_table[hash(contact_in.number)];
	
		for (list_it = L.begin(); list_it != L.end(); ++list_it){
			if (list_it->number == contact_in.number){
				list_it->name = contact_in.name;
				return;
			}
		}	
		L.push_back(contact_in);
		numberOfKeys++;
		loadFactor = ((double)numberOfKeys)/((double)m); 
	}
	void Delete(int number){
		list<Contact>::iterator list_it; // pointer to the element of a list
		list<Contact> &L = hash_table[hash(number)];
	
		for (list_it = L.begin(); list_it != L.end(); ++list_it){
			if (list_it->number == number){
				L.erase(list_it);
				numberOfKeys--;
				loadFactor = ((double)numberOfKeys)/((double)m); 
				return;
			}
		}
	}
	// End Chain
	///////////////////////////////
};

void rehash(Hashing*& hashing_ptr){ // reference the pointer, so that it can be redirected
	// int m_new = 2*hashing.m;
	// double loadFactor = hashing.loadFactor;
	if (hashing_ptr->loadFactor > 0.9){
		Hashing* hashing_ptr_new = new Hashing(2*hashing_ptr->m);
		for (int i = 0; i < hashing_ptr->m; ++i){
			list<Contact>::iterator list_it; // pointer to the element of a list
			list<Contact> &L = hashing_ptr->hash_table[i];
			
			for (list_it = L.begin(); list_it != L.end(); ++list_it){
				hashing_ptr_new->Set(*list_it);
			}
		}
		// Change the pointer to the new one
		Hashing* temp_ptr = hashing_ptr;
		hashing_ptr = hashing_ptr_new;
		delete temp_ptr;		
	}
}

int main(){
  int m = 10;
  Hashing* hashing_ptr = new Hashing(m);
  
  int number;
  string name;
  int n;
  
  cin >> n;
  for (int i = 0; i < n; i++){
  	cin >> number >> name;
  	hashing_ptr->Set(number,name);
  	rehash(hashing_ptr);
  }
  while(1){
  	cin >> number;
	cout << hashing_ptr->Get(number) << "\n";  	
  }
  return 0;
}

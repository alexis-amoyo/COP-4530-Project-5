#include <vector>
#include <list>
#include <string>     // string.c_str(), string();
#include <algorithm>  // std::find
#include <functional> // std::hash
#include <iostream>   // std::cout, std::cerr
#include <fstream>    // ifstream and ofstream
#include <string.h>   // strcpy, strtok, 
#include <unistd.h>   // crypt()
#include <stdlib.h> 

#include "hashtable.h"
#include "pass_server.h"

using namespace std;
using namespace cop4530;
 
template <typename K, typename V>
size_t PassServer<K,V>::size()
{
   return HashTable<K,V>::size();
}

template <typename K, typename V>
PassServer<K,V>::PassServer(size_t size) : HashTable<K,V>{size}{} //setting reg doesnt work what

template <typename K, typename V>
PassServer<K,V>::~PassServer(){
   HashTable<K,V>::clear();
}

template <typename K, typename V>
bool PassServer<K,V>::load(const char *filename){
   //I rewrote it here to encrypt it
   ifstream in;
   in.clear();
   in.open(filename);

   if(!in || !in.is_open()){
      return false;
   }

   string line = " ";
//	in.ignore(1, '\n');
   while(!in.eof()) {
      pair<K,V> p;
      in >> p.first >> p.second;
	//in.ignore(100, '\n');
	cout << "\nUser: " << p.first << "\tPassword: " << p.second << endl;
      addUser(p); //skips 1? how to fix...
   }
	//HashTable<K,V>::load(filename);
   in.close();
   return true;
}

template <typename K, typename V>
bool PassServer<K,V>::write_to_file(const char *filename){
   return HashTable<K,V>::write_to_file(filename);
}

template <typename K, typename V>
bool PassServer<K,V>::addUser(std::pair<string, string> & kv){
   //encrypt pass only, copy variant
   kv.second = encrypt(kv.second);  //must be pre-encrypted for encryptions to occur
   return HashTable<K,V>::insert(kv);
}

template <typename K, typename V>
bool PassServer<K,V>::addUser(std::pair<string, string> && kv){
   //encrypt pass only, move variant
   kv.second = encrypt(kv.second);  //must be pre-encrypted for encryptions to occur
   return HashTable<K,V>::insert(std::move(kv));
}

template <typename K, typename V>
bool PassServer<K,V>::removeUser(const string & k){
   return HashTable<K,V>::remove(k); //just k?
}

//change an existing user's password. Note that both passwords passed in are in plaintext. They should be
//encrypted before you interact with the hash table. If the user is not in the hash table, return
//false. If p.second does not match the current password, return false. Also return false if the new
//password and the old password are the same (i.e., we cannot update the password).
template <typename K, typename V>
bool PassServer<K,V>::changePassword(const pair<string, string> &p, const string & newpassword){
   if(!find(p.first))   //If the user is not in the hash table, return false
      return false;
   if(HashTable<K,V>::match(make_pair(p.first, encrypt(p.second))))  //If p.second does not match the current password, return false
      return false;
   if(encrypt(p.second) == encrypt(newpassword))   //Also return false if the new password and the old password are the same
      return false;
   HashTable<K,V>::remove(p.first); //to replace
   HashTable<K,V>::insert(make_pair(p.first, encrypt(newpassword)));
   return true;
   //work on
}

template <typename K, typename V>
bool PassServer<K,V>::find(const string & user){
   return HashTable<K,V>::contains(user);
}

template <typename K, typename V>
void PassServer<K,V>::dump(){
   HashTable<K,V>::dump(); //finally she works
}

/*
   // Encrypt the parameter str and return the encrypted string.
   // For this project, we shall use the GNU C Library's crypt() method to encrypt the password. The
   // algorithm for the crypt() method shall be MD5-based. The salt shall be the character stream
   // "$1$########". The resulting encrypted character stream is the
   // "$1$########" + ‘$' + 22 characters = 34 characters in total.
   // A user password is the sub string containing the last 22 characters, located after the 3rd ‘$'.
*/
template <typename K, typename V>
string PassServer<K,V>::encrypt(const string & str){
   //work on
   char *pass = new char [69];  //is 69 too much/little? is there a limit
   char salt[] = "$1$########";  //from project pdf

   strcpy(pass, str.c_str());   //recommended to use c_string for convenience in lecture, use string.h lib

   //pass = crypt(pass, salt);
   string cryptPass = crypt(pass, salt);  //key, salt parameter format, new variable for string
   return cryptPass.substr(12);   //gives last 22 char (34-22 = 12)
} 


/* XXX: Please keep this line at the very end */
template class PassServer<string, string>;

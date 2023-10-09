using namespace cop4530;

template <typename K, typename V>
HashTable<K, V>::HashTable(size_t size){
   currentSize = 0;  //no hash yet so 0 NOT 1
   theHash.resize(prime_below(size));
}

template <typename K, typename V>
HashTable<K, V>::~HashTable(){
   clear();
}

template <typename K, typename V>
bool HashTable<K, V>::contains(const K &x){
   //for template with one typename
   auto & hashAt = theHash[myhash(x)];
   //return find(begin(hashAt), end(hashAt), x) != end(hashAt)
   for(auto itr = hashAt.begin(); itr != hashAt.end(); itr++){   //list has begin & end iterator
      if(itr->first == x)
         return true;
   }
   return false;
}

template <typename K, typename V>
bool HashTable<K, V>::match(const std::pair<K, V> &kv){
   auto & hashAt = theHash[myhash(kv.first)];
   return find(begin(hashAt), end(hashAt), kv) != end(hashAt);  //from lecture & algorithm library
}

template <typename K, typename V>
bool HashTable<K, V>::insert(const std::pair<K, V> &kv){
   //from lecture
   auto & hashAt = theHash[myhash(kv.first)];

   if(find(begin(hashAt), end(hashAt), kv) != end(hashAt))  //find fr/ algorithm lib
      return false;
   else if(contains(kv.first)){  //if or else if? does it matter
      remove(kv.first);
   }

   hashAt.push_back(kv);
   int temp = currentSize + 1;   //i dont want it to increment size yet so hypothetical

   if(temp > theHash.size())
      rehash();
   currentSize++;
   return true;
}

template <typename K, typename V>
bool HashTable<K, V>::insert(std::pair<K, V> &kv){
   auto & hashAt = theHash[myhash(kv.first)];

   if(find(begin(hashAt), end(hashAt), kv) != end(hashAt))  //find fr/ algorithm lib
      return false;
   else if(contains(kv.first)){
      remove(kv.first);
   }
   
   hashAt.push_back(std::move(kv));
   int temp = currentSize + 1;

   if(temp > theHash.size())
      rehash();
   currentSize++;
   return true;
}

template <typename K, typename V>
bool HashTable<K, V>::remove(const K &k){
   //from lecture, not for pair template since only key not key+value pair
   /*
   auto & hashAt = theHash[myhash(k)];
   auto itr = find(begin(hashAt), end(hashAt), k);

   if(itr == end(hashAt)){
      return false;
   }

   hashAt.erase(itr);
   --currentSize;
   return true;
   */
   
  auto & hashAt = theHash[myhash(k)];
  for(auto itr = hashAt.begin(); itr != hashAt.end(); itr++){   //list has begin & end iterator
      if(itr->first == k){
         hashAt.erase(itr);
         currentSize--;
         return true;
      }
   }
   return false;
}

template <typename K, typename V>
void HashTable<K, V>::clear(){
   makeEmpty();   //calls makeEmpty
}

//Load the content of the file with name filename into the hash
//table. In the file, each line contains a single pair of key and value, separated by a white space.
template <typename K, typename V>
bool HashTable<K, V>::load(const char * filename){
   ifstream in;
   in.clear();
   in.open(filename);
   //string s;

   if(!in || !in.is_open()){
      return false;
   }

   string line;
   while(getline(in, line)) {
      //stringstream ss(line);
      pair<K,V> p;
      in >> p.first >> p.second;
	//cout << p.first << " " << p.second << endl;
      insert(p);

      //Check
      //cout << "User: " << p.first << ", Password: " << p.second << endl;
   }

   /*doesn't account for empty lines
   for(string s; getline(in, s);){
      stringstream ss(s);
      if(!s.empty()){
         std::pair<K,V> p;
         in >> p.first >> p.second;
         insert(p);
      }
      else
         cout << "\nHUHS\n" << s << endl;
   }
   */

   in.close();
   return true;
}

//display all entries in the hash table. If an entry contains multiple key-value pairs,
//separate them by a vertical bar (|) (see the provided executable for the exact output format).
template <typename K, typename V>
void HashTable<K, V>::dump(){
   //cout << theHash.size() << endl;
   for(int i = 0; i < theHash.size(); i++){
      auto hashAt = theHash[i];  //the way that it's still annoying to type this even with auto
      for(auto itr = hashAt.begin(); itr != hashAt.end(); itr++){
            if(itr != hashAt.begin())
               cout << "|"; //however you print, involve iterators
            cout << itr->first << " " << itr->second;
      }
      if(!hashAt.empty())
         cout << endl;   //should only be size endls...so needs to not be empty. this took longer than expected to figure out
   }
}

//write all elements in the hash table into a file with
//name filename. Similar to the file format in the load function, each line contains a pair of key-
//value pair, separated by a white space
template <typename K, typename V>
bool HashTable<K, V>::write_to_file(const char * filename){
   ofstream out;
   out.clear();
   out.open(filename);

   if(!out || !out.is_open()){
      return false;
   }

   for(int i = 0; i < theHash.size(); i++){
      auto itr = theHash[i].begin();
      auto & hashAt = theHash[i];

      if(!hashAt.empty()){
         out << itr->first << " " << itr->second << endl;
         while(++itr != hashAt.end()){
            out << itr->first << " " << itr->second << endl;   //for hash with more than one entry
         }
      }
   }
   out.close();
   return true;
}

// returns largest prime number <= n or zero if input is too large
// This is likely to be more efficient than prime_above(), because
// it only needs a vector of size n
template <typename K, typename V>
unsigned long HashTable<K, V>::prime_below (unsigned long n)
{
   if (n > max_prime)
   {
      std::cerr << "** input too large for prime_below()\n";
      return 0;
   }
   if (n == max_prime)
   {
      return max_prime;
   }
   if (n <= 1)
   {
      std::cerr << "** input too small \n";
      return 0;
   }

   // now: 2 <= n < max_prime
   std::vector <unsigned long> v (n+1);
   setPrimes(v);
   while (n > 2)
   {
      if (v[n] == 1)
         return n;
      --n;
   }

   return 2;
}

//Sets all prime number indexes to 1. Called by method prime_below(n) 
template <typename K, typename V>
void HashTable<K, V>::setPrimes(std::vector<unsigned long>& vprimes)
{
   int i = 0;
   int j = 0;

   vprimes[0] = 0;
   vprimes[1] = 0;
   int n = vprimes.capacity();

   for (i = 2; i < n; ++i)
      vprimes[i] = 1;

   for( i = 2; i*i < n; ++i)
   {
      if (vprimes[i] == 1)
         for(j = i + i ; j < n; j += i)
            vprimes[j] = 0;
   }
}

template <typename K, typename V>
size_t HashTable<K,V>::size() const
{
   return currentSize;
}

template <typename K, typename V>
void HashTable<K,V>::makeEmpty(){
   //clears all elements in hash table, called by clear
   for(auto & thisList : theHash)
      thisList.clear();
   currentSize = 0;  //dont forgetti spaghetti
}

template <typename K, typename V>
void HashTable<K,V>::rehash(){
   //rehash function, called when # of elements is greater than vector size

   //from book
   auto oldHash = theHash;
   // Create new double-sized, empty table
   theHash.resize(prime_below(2 * theHash.size()));
   clear();
   // Copy table over
   for(auto & thisHash : oldHash){
      for(auto & x : thisHash)
         insert(std::move(x));
   }
}

template <typename K, typename V>
size_t HashTable<K,V>::myhash(const K &x){
   //edited from lecture/book
   static hash<K> hf;
   //cout << "\nHEY" << hf(x) % theHash.size() << endl;
   return hf(x) % theHash.size();
}

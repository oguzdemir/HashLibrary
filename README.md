# HashLibrary
Thread-safe hash library implementation with header and c file.

There is 4 functions available in this library.These are initialization with number of hash table rows, insertion, deletion and get. 

For making it thread-safe, mutexes are used for each hash table row. By that way, an operation in a hash line does not block the other lines' operations. 


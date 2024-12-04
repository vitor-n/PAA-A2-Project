#ifndef HashTablesH
#define HashTablesH

template <typename T>
struct HTNode {
    unsigned key;
    T value;
    HTNode* next;
    HTNode* previous;
};

template <typename T>
class HashTable {
    private:
        HTNode<T>** m_table; //An array with the respective lists for each hash
        int m_size; //The size of the hash function image set;

        unsigned hash(unsigned);

    public:
        HashTable(int);
        ~HashTable();

        void set(unsigned, T);
        HTNode<T>* get(unsigned);
        void remove(unsigned);
};

#endif

#include <bits/stdc++.h>
#define For(i, n) for (int i = 0; i < n; i++)
#define SIZE 26
#define ctoi(c) ((int)c - (int)'A') // converts characters A-Z into integer 0-25
#define bmat vector<vector<bool>>
#define cmat vector<vector<char>>
using namespace std;

// we use a set to store the total number of words found to avoid printing multiple recurrences of the same word
set<string> found_words;

// struct of the the node of Trie
typedef struct TrieNode
{
    TrieNode *child[SIZE];
    bool leaf;
} node;

// function to allocate memory for a new node
node *get_node()
{
    node *newnode = new node;
    newnode->leaf = false;
    for (int i = 0; i < SIZE; i++)
        newnode->child[i] = NULL;
    return newnode;
}

// function to insert a string to the trie
void insert(node *root, string key)
{
    int n = key.length();
    node *pchild = root; // child pointer
    For(i, n)
    {
        int index = ctoi(key[i]); // index is the integer representing the alphabet key[i]
        if (pchild->child[index] == NULL)
            pchild->child[index] = get_node(); // we create a new node at index, this link counts as alphebaet key[i]

        pchild = pchild->child[index]; // move the pointer to the next node
    }
    pchild->leaf = true; // to mark the end of a word in TRIE
}

// function to check whether the boggle cell we are accessing is safe to access
bool issafe(int i, int j, bmat visited, int M, int N)
{
    // checks that the address i,j exist inside the boggle and the cell is not visited already
    return (i >= 0 && i < M && j >= 0 && j < N && !visited[i][j]);
}

void search_word(TrieNode *root, cmat boggle, int M, int N, int i, int j, bmat visited, string str)
{
    // leaf = true depicts the end of a word
    if (root->leaf == true) // we insert the word we complete to the set of found_words
        found_words.insert(str);

    // if the cell of the boggle is safe to visit
    if (issafe(i, j, visited, M, N))
    {
        // mark the cell as visited
        visited[i][j] = true;

        // traverse all childs of current root
        For(k, SIZE)
        {
            if (root->child[k] != NULL) // if we find some non NULL child of the current trie node
            {
                char ch = k + 'A'; // next alphabet to be searched
                // we check the neighbouring cells for the next alphabet
                for (int m = -1; m <= 1; m++)
                    for (int n = -1; n <= 1; n++)
                        if (m || n) // check all cells other than the current cell for the next alphabet
                        {
                            if (issafe(i + m, j + n, visited, M, N) && (boggle[i + m][j + n] == ch)) // check whether the neighbouring cell is safe to visit or not
                            {
                                search_word(root->child[k], boggle, M, N, i + m, j + n, visited, str + ch); // search for the next alphabet in the neighbouring cells
                            }
                        }
            }
        }
        // make current cell unvisited
        visited[i][j] = false;
    }
}

void find_words(cmat boggle, node *root, int M, int N)
{
    // initialise a 2d boolean array visited which keeps track of the visited cells while finding words to avoid revisiting cells
    bmat visited(M, vector<bool>(N, false));
    node *pchild = root; // make a pointer to the root of the TRIE
    string str = "";

    // iterate over the entire boggle
    For(i, M)
    {
        For(j, N)
        {
            if (pchild->child[ctoi(boggle[i][j])]) // if the current alphabet in the boggle is also the start of any word in TRIE
            {
                str = str + boggle[i][j];                                                         // append the alphabet to the string
                search_word(pchild->child[ctoi(boggle[i][j])], boggle, M, N, i, j, visited, str); // search all the possible words starting from the current alphabet
                str = "";                                                                         // empty the string for the new word
            }
        }
    }
}

// function to create a randomised boggle of size M x N
cmat create_boggle(int M, int N)
{
    // initialise a boggle of size M x N
    cmat boggle(M, vector<char>(N));

    srand(time(0));
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
        {
            int random = (rand() % 26);       // get a random integer from 0-25
            char value = (char)(random + 65); // convert the integer to alphabet
            boggle[i][j] = value;
        }

    return boggle;
}

// function to accept a boggle entered by the user
cmat input_boggle(int M, int N)
{
    // initialise a boggle of size M x N
    cmat boggle(M, vector<char>(N));
    cout<<"Enter "<<M*N<<" alphabets in UPPERCASE : ";
    // input M x N characters from the user
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            cin >> boggle[i][j];
    return boggle;
}

// function to display the boggle
void display_boggle(cmat boggle, int M, int N)
{
    cout << "The BOGGLE is" << endl;
    For(i, M)
    {
        For(j, N) cout << boggle[i][j] << " ";
        cout<<endl;
    }
    cout << endl;
}

int main()
{
    //BOGGLE
    int M, N;
    cout << "Enter the Values of M(Rows) and N(Columns): ";
    cin >> M >> N;
    printf("0 - Enter a boggle of size %d x %d\n1 - Let us create a randomised boggle of size %d x %d\nEnter Choice: ", M, N, M, N);
    bool choice;
    cin >> choice;
    cmat boggle;
    if (choice)
        boggle = create_boggle(M, N);
    else
        boggle = input_boggle(M, N);
    display_boggle(boggle, M, N);


    //DICTIONARY
    vector<string> dictionary;
    // Reading the words from the dictionary and creating an array of words
    string text;
    ifstream dict("dictionary1000.txt");
    while (getline(dict, text))
        dictionary.push_back(text);
    dict.close();
    // Asking user if they want to add some from of their own
    cout<<"Do you wish to add some new words to the Dictionary (1 - YES / 0 - NO): ";
    cin>>choice;
    if(choice){
        cout<<"Input a word in UPPERCASE to add\nInput \"end\" to stop adding words\n";
        string word;
        cin>>word;
        while(word!="end"){
            dictionary.push_back(word);
            cin>>word;
        }
    }

    // Making TRIE out of the words in the dictionary
    node *root = get_node();
    For(i, dictionary.size()) insert(root, dictionary[i]);

    // Searching the words in ddictionary inside the BOGGLE
    find_words(boggle, root, M, N);

    // Printing the found words
    cout<<"Total Number of Words Found : "<<found_words.size()<<endl<<"The Words Are : "<<endl;
    for (auto itr : found_words)
        cout << itr <<"\t";
    cout<<endl;
    return 0;
}
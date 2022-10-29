#include <bits/stdc++.h>
#define For(i, n) for (int i = 0; i < n; i++)
#define SIZE 26
#define ctoi(c) ((int)c - (int)'A') //converts characters A-Z into integer 0-25
#define bmat vector<vector<bool>>
#define cmat vector<vector<char>>
using namespace std;

//we use a set to store the total number of words found to avoid printing multiple recurrences of the same word
set<string> found_words;

//struct of the the node of Trie
typedef struct TrieNode
{
    TrieNode *child[SIZE];
    bool leaf;
} node;

//function to allocate memory for a new node 
node *get_node()
{
    node *newnode = new node;
    newnode->leaf = false;
    for (int i = 0; i < SIZE; i++)
        newnode->child[i] = NULL;
    return newnode;
}

//function to insert a string to the trie 
void insert(node *root, string key)
{
    int n = key.length();
    node *pchild = root;
    For(i, n)
    {
        int index = ctoi(key[i]);
        if (pchild->child[index] == NULL)
            pchild->child[index] = get_node();

        pchild = pchild->child[index];
    }
    pchild->leaf = true;
}

bool issafe(int i, int j, bmat visited, int M, int N)
{
    return (i >= 0 && i < M && j >= 0 && j < N && !visited[i][j]);
}

void search_word(TrieNode *root, cmat boggle, int M, int N, int i, int j, bmat visited, string str)
{
    // if we found word in trie / dictionary
    if (root->leaf == true)
        found_words.insert(str);

    // If both I and j in  range and we visited
    // that element of matrix first time
    if (issafe(i, j, visited, M, N))
    {
        // make it visited
        visited[i][j] = true;

        // traverse all childs of current root
        For(k, SIZE)
        {
            if (root->child[k] != NULL)
            {
                // current character
                char ch = k + 'A';
                for (int m = -1; m <= 1; m++)
                    for (int n = -1; n <= 1; n++)
                        if (m || n)
                        {
                            if (issafe(i + m, j + n, visited, M, N) && (boggle[i + m][j + n] == ch))
                            {
                                search_word(root->child[k], boggle, M, N, i + m, j + n, visited, str + ch);
                            }
                        }
            }
        }
        // make current element unvisited
        visited[i][j] = false;
    }
}

void find_words(cmat boggle, node *root, int M, int N)
{
    bmat visited(M, vector<bool>(N, false));
    node *pchild = root;
    string str = "";

    For(i, M)
    {
        For(j, N)
        {
            if (pchild->child[ctoi(boggle[i][j])])
            {
                str = str + boggle[i][j];
                search_word(pchild->child[ctoi(boggle[i][j])], boggle, M, N, i, j, visited, str);
                str = "";
            }
        }
    }
}

cmat create_boggle(int M, int N)
{
    cmat boggle(M, vector<char>(N));

    srand(time(0));
    for (int h = 0; h < M; h++)
    {
        for (int w = 0; w < N; w++)
        {
            int random = 1 + (rand() % 26);
            char value = (char)(random + 64);
            boggle[h][w] = value;
        }
    }
    return boggle;
}

void display_boggle(cmat boggle, int M, int N)
{
    For(i, M)
    {
        For(j, N)
        {
            cout << boggle[i][j] << " ";
        }
        cout << endl;
    }
}

int main()
{
    vector<string> dictionary;
    string text;
    ifstream dict("dictionary500.txt");
    while(getline(dict,text))dictionary.push_back(text);
    dict.close();

    int M,N;
    cout<<"Enter the Values of M(Rows) and N(Columns): ";
    cin>>M>>N;
    cmat boggle = create_boggle(M, N);
    display_boggle(boggle, M, N);

    node *root = get_node();
    For(i, dictionary.size()) insert(root, dictionary[i]);

    find_words(boggle, root, M, N);
    for(auto itr:found_words)cout<<itr<<endl;

    return 0;
}
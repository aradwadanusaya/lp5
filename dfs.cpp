#include<iostream>
#include<stack>
#include<omp.h>

using namespace std;

class node {
public:
    node *left, *right;
    int data;
};

class DFS {
public:
    node *insert(node *, int);
    void dfs(node *);
};

node* DFS::insert(node *root, int data) {
    if(!root) {
        root = new node;
        root->left = NULL;
        root->right = NULL;
        root->data = data;
        return root;
    }
    stack<node*> s;
    s.push(root);
    while (!s.empty()) {
        node* currNode = s.top();
        s.pop();
        if(currNode->left == NULL) {
            currNode->left = new node;
            currNode->left->left = NULL;
            currNode->left->right = NULL;
            currNode->left->data = data;
            return root;
        } else {
            s.push(currNode->left);
        }
        if(currNode->right == NULL) {
            currNode->right = new node;
            currNode->right->left = NULL;
            currNode->right->right = NULL;
            currNode->right->data = data;
            return root;
        } else {
            s.push(currNode->right);
        }
    }
}

void DFS::dfs(node *head) {
    if (!head) {
        return;
    }
    stack<node*> s;
    s.push(head);
    #pragma omp parallel
    {
        while (!s.empty()) {
            node* currNode;
            #pragma omp critical
            {
                if (!s.empty()) {
                    currNode = s.top();
                    s.pop();
                }
            }
            if (currNode) {
                cout << "\t" << currNode->data;
                // Push the right child first to ensure left child is processed first
                if (currNode->right) {
                    #pragma omp critical
                    {
                        s.push(currNode->right);
                    }
                }
                if (currNode->left) {
                    #pragma omp critical
                    {
                        s.push(currNode->left);
                    }
                }
            }
        }
    }
}

int main() {
    node *root = NULL;
    int data;
    char ans;
    DFS dfsTree;

    do {
        cout << "\n enter data: ";
        cin >> data;
        root = dfsTree.insert(root, data);
        cout << "do you want insert one more node?";
        cin >> ans;
    } while(ans == 'y' || ans == 'Y');

    cout << "\nDepth-First Search Traversal:" << endl;
    dfsTree.dfs(root);

    return 0;
}

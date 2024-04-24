#include<iostream>
#include<queue>
#include<omp.h>

using namespace std;

class node {
public:
    node *left, *right;
    int data;
};

class BFS {
public:
    node *insert(node *, int);
    void bfs(node *);
};

node* BFS::insert(node *root, int data) {
    if(!root) {
        root = new node;
        root->left = NULL;
        root->right = NULL;
        root->data = data;
        return root;
    }
    queue<node *> q;
    q.push(root);
    while(!q.empty()) {
        node *temp = q.front();
        q.pop();
        if(temp->left == NULL) {
            temp->left = new node;
            temp->left->left = NULL;
            temp->left->right = NULL;
            temp->left->data = data;
            return root;
        } else {
            q.push(temp->left);
        }
        if(temp->right == NULL) {
            temp->right = new node;
            temp->right->left = NULL;
            temp->right->right = NULL;
            temp->right->data = data;
            return root;
        } else {
            q.push(temp->right);
        }
    }
}

void BFS::bfs(node *head) {
    queue<node*> q;
    q.push(head);
    while (!q.empty()) {
        int qSize = q.size();
        #pragma omp parallel for
        for (int i = 0; i < qSize; i++) {
            node* currNode;
            #pragma omp critical
            {
                currNode = q.front();
                q.pop();
            }
            if (currNode) {
                cout << "\t" << currNode->data;
                if(currNode->left) {
                    #pragma omp critical
                    {
                        q.push(currNode->left);
                    }
                }
                if(currNode->right) {
                    #pragma omp critical
                    {
                        q.push(currNode->right);
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
    BFS bfsTree;

    do {
        cout << "\n enter data: ";
        cin >> data;
        root = bfsTree.insert(root, data);
        cout << "do you want insert one more node?";
        cin >> ans;
    } while(ans == 'y' || ans == 'Y');

    cout << "\nBreadth-First Search Traversal:" << endl;
    bfsTree.bfs(root);

    return 0;
}



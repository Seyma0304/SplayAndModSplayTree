//Þeyma Özek 150122027
//Fatma Zeynep Kök 150122060
/*This program creates splay tree and mod-splay tree by reading the entries in the file.
 It calculates the costs and prints them to the console as pre-order.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    int key;
    struct Node* left;
    struct Node* right;
    struct Node* parent;
    int frequency;
} Node;

Node* createNode(int key);
void rightRotate(Node** root, Node* x, int* rotation);
void leftRotate(Node** root, Node* x, int* rotation);
void splay(Node** root, Node* x, int* rotation);
void splayInsert(Node** root, int key, int* comparison, int* rotation);
Node* findMaxFrequencyNode(Node* root);
void preOrderSplay(Node* root);
void preOrderModSplay(Node* root);
void readFromFileAndInsert(Node** root, const char* filename, int* comparison, int* rotation, int mode);
void freeTree(Node* root);
void modSplayInsert(Node** root, int key, int* comparison, int* rotation);



//We created a new node with the createNode function and assigned the initial value then initialized the elements of the node.
Node* createNode(int key) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->key = key;
    newNode->frequency = 0;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = NULL;
    return newNode;
}

/*Here, we made a right rotation (zig) on ??the entered node and increased the number of rotations.
 As a result of this rotation, the left child of the node becomes the parent node.*/
void rightRotate(Node** root, Node* x, int* rotation) {
    (*rotation)++;
    Node* y = x->left;
    if (!y) return;
    x->left = y->right;
    if (y->right) y->right->parent = x;

    y->parent = x->parent;
    if (!x->parent)
        *root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->right = x;
    x->parent = y;
}

/*Here, we made a left rotation (zag) on ??the entered node and increased the number of rotations.
 As a result of this rotation, the right child of the node becomes the parent node.*/
void leftRotate(Node** root, Node* x, int* rotation) {
    (*rotation)++;
    Node* y = x->right;
    if (!y) return;

    x->right = y->left;
    if (y->left) y->left->parent = x;

    y->parent = x->parent;
    if (!x->parent)
        *root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}

/*We moved the given node to the root of the tree with the splay method.
 We controlled the zig, zag, zig-zig, zig-zag, zag-zig and zag-zag situations with if else blocks.*/
void splay(Node** root, Node* x, int* rotation) {
    while (x->parent) {
        if (!x->parent->parent) {
            if (x == x->parent->left)
                rightRotate(root, x->parent, rotation);
            else
                leftRotate(root, x->parent, rotation);
        } else if (x == x->parent->left && x->parent == x->parent->parent->left) {
            rightRotate(root, x->parent->parent, rotation);
            rightRotate(root, x->parent, rotation);
        } else if (x == x->parent->right && x->parent == x->parent->parent->right) {
            leftRotate(root, x->parent->parent, rotation);
            leftRotate(root, x->parent, rotation);
        } else if (x == x->parent->right && x->parent == x->parent->parent->left) {
            leftRotate(root, x->parent, rotation);
            rightRotate(root, x->parent, rotation);
        } else {
            rightRotate(root, x->parent, rotation);
            leftRotate(root, x->parent, rotation);
        }
    }
}

/*We added a key to the splay tree with the splayInsert method.
 In this method, the splay method is called every time an addition is made.*/
void splayInsert(Node** root, int key, int* comparison, int* rotation) {
    Node* newNode = createNode(key);
    Node* parent = NULL;
    Node* current = *root;

    while (current) {
        parent = current;
        (*comparison)++;
        if (key < current->key)
            current = current->left;
        else if (key > current->key)
            current = current->right;
        else {
            current->frequency++;
            free(newNode);
            splay(root, current, rotation);
            return;
        }
    }

    newNode->parent = parent;
    if (!parent)
        *root = newNode;
    else if (key < parent->key)
        parent->left = newNode;
    else
        parent->right = newNode;

    splay(root, newNode, rotation);
}

//With this method, we found the node with the highest frequency.
Node* findMaxFrequencyNode(Node* root) {
    if (!root) return NULL;

    Node* maxNode = root;
    if (root->left) {
        Node* leftMax = findMaxFrequencyNode(root->left);
        if (leftMax && leftMax->frequency > maxNode->frequency)
            maxNode = leftMax;
    }
    if (root->right) {
        Node* rightMax = findMaxFrequencyNode(root->right);
        if (rightMax && rightMax->frequency > maxNode->frequency)
            maxNode = rightMax;
    }
    return maxNode;
}

/*We added a key to the mod-splay tree with the modSplayInsert method.
 In this method, the splay method is called only if the frequency of the newly added node is greater than the frequency of the root.*/
void modSplayInsert(Node** root, int key, int* comparison, int* rotation) {
    Node* parent = NULL;
    Node* current = *root;

    while (current) {
        parent = current;
        (*comparison)++;
        if (key < current->key) {
            current = current->left;
        } else if (key > current->key) {
            current = current->right;
        } else {
            current->frequency++;
            if (current->frequency > (*root)->frequency) {
                splay(root, current, rotation);
            }
            return;
        }
    }

    Node* newNode = createNode(key);
    newNode->parent = parent;
    if (!parent) {
        *root = newNode;
    } else if (key < parent->key) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }

}

//With this method, we made a pre-order treversal of the splay tree and printed the nodes in order.
void preOrderSplay(Node* root) {
    if (root) {
        printf("%d ", root->key);
        preOrderSplay(root->left);
        preOrderSplay(root->right);
    }
}

//With this method, we made a pre-order treversal of the mod-splay tree and printed the nodes and their frequencies in order.
void preOrderModSplay(Node* root) {
    if (root) {
        printf("(%d,%d) ", root->key, root->frequency);
        preOrderModSplay(root->left);
        preOrderModSplay(root->right);
    }
}

//Here we read the keys from the file and call the insert method for splay or mod-splay depending on the mode.
void readFromFileAndInsert(Node** root, const char* filename, int* comparison, int* rotation, int mode) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("File cannot open: %s\n", filename);
        return;
    }

    int key;
    while (fscanf(file, "%d,", &key) != EOF) {
        if (mode == 1){
            splayInsert(root, key, comparison, rotation);
        }
        else{
            modSplayInsert(root, key, comparison, rotation);
        }
    }

    fclose(file);
}

//Here we freed all nodes in the tree.
void freeTree(Node* root) {
    if (root) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

/*In this method, the input.txt file is read twice.
 In the first one, the splay tree is created, the cost is calculated and printed to the console.
 Then the tree is completely cleaned.
 In the second one, the mod-splay tree is created, its cost is calculated and printed to the console.*/
int main() {
    int comparison_count = 0;
    int rotation_count = 0;
    Node* root = NULL;

    
    readFromFileAndInsert(&root, "input.txt", &comparison_count, &rotation_count, 1);
    printf("Splay tree");
    printf("\nTree (pre-order traversal): ");
    preOrderSplay(root);
    printf("\n");
    printf("Cost: %d\n", comparison_count + rotation_count);
    
    
    freeTree(root);
    int mod_comparison = 0;
    int mod_rotation = 0;
    root = NULL;
    
    readFromFileAndInsert(&root, "input.txt", &mod_comparison, &mod_rotation, 2);
    printf("Mod-Splay tree");
    printf("\nTree (pre-order traversal): ");
    preOrderModSplay(root);
    printf("\n");
    printf("Cost: %d\n", mod_comparison + mod_rotation);

    return 0;
}

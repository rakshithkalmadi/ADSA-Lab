#include <stdio.h>
#include <stdlib.h>

#define T 2 // B-tree parameter, change as needed

// Structure for B-tree node
struct BTreeNode {
    int n; // Number of keys
    int keys[2 * T - 1]; // Array to hold keys
    struct BTreeNode* children[2 * T]; // Array to hold child pointers
};

// Function to create a new B-tree node
struct BTreeNode* createNode() {
    struct BTreeNode* newNode = (struct BTreeNode*)malloc(sizeof(struct BTreeNode));
    newNode->n = 0;
    for (int i = 0; i < 2 * T; i++) {
        newNode->children[i] = NULL;
    }
    return newNode;
}

// Function to insert a key into the B-tree
void insert(struct BTreeNode** root, int key) {
    struct BTreeNode* r = *root;

    if (r->n == 2 * T - 1) {
        struct BTreeNode* newNode = createNode();
        newNode->children[0] = r;
        *root = newNode;
        splitChild(newNode, 0, r);
        insertNonFull(newNode, key);
    } else {
        insertNonFull(r, key);
    }
}

// Function to insert a key into a non-full B-tree node
void insertNonFull(struct BTreeNode* x, int key) {
    int i = x->n - 1;

    if (x->n == 0) {
        x->keys[0] = key;
        x->n++;
        return;
    }

    while (i >= 0 && key < x->keys[i]) {
        x->keys[i + 1] = x->keys[i];
        i--;
    }

    x->keys[i + 1] = key;
    x->n++;

    return;
}

// Function to split a child of a B-tree node
void splitChild(struct BTreeNode* x, int i, struct BTreeNode* y) {
    struct BTreeNode* z = createNode();
    // x->children[i + 1] = z;

    for (int j = T; j < 2 * T - 1; j++) {
        z->keys[j - T] = y->keys[j];
        y->keys[j] = 0;
    }

    y->n = T - 1;
    z->n = T - 1;

    for (int j = x->n; j >= i + 1; j--) {
        x->children[j + 1] = x->children[j];
    }

    x->children[i + 1] = z;

    for (int j = x->n - 1; j >= i; j--) {
        x->keys[j + 1] = x->keys[j];
    }

    x->keys[i] = y->keys[T - 1];
    x->n++;

    return;
}

// Function to print the B-tree (inorder traversal)
void inorderTraversal(struct BTreeNode* x) {
    if (x) {
        int i;
        for (i = 0; i < x->n; i++) {
            inorderTraversal(x->children[i]);
            printf("%d ", x->keys[i]);
        }
        inorderTraversal(x->children[i]);
    }
}

int main() {
    struct BTreeNode* root = createNode();

    // Insert keys into the B-tree
    int keys[] = {10, 20, 5, 15, 25, 30, 35};
    int numKeys = sizeof(keys) / sizeof(keys[0]);

    for (int i = 0; i < numKeys; i++) {
        insert(&root, keys[i]);
        printf("Inserted: %d\n", keys[i]);
    }

    printf("Inorder Traversal of B-tree: ");
    inorderTraversal(root);
    printf("\n");

    return 0;
}

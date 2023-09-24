#include <stdio.h>
#include <stdlib.h>

#define T 3

// Structure for B-tree node
struct BTreeNode {
    int n; // Number of keys
    int keys[2 * T - 1]; // Array to hold keys
    struct BTreeNode* children[2 * T]; // Array to hold child pointers
    int leaf; // Is the node a leaf
};

// Function to create a new B-tree node
struct BTreeNode* createNode(int leaf) {
    struct BTreeNode* newNode = (struct BTreeNode*)malloc(sizeof(struct BTreeNode));
    newNode->n = 0;
    newNode->leaf = leaf;
    for (int i = 0; i < 2 * T; i++) {
        newNode->children[i] = NULL;
    }
    return newNode;
}

// Function to split a child of a B-tree node
void splitChild(struct BTreeNode* x, int i, struct BTreeNode* y) {
    struct BTreeNode* z = createNode(y->leaf);

    x->children[i + 1] = z;

    for (int j = 0; j < T - 1; j++) {
        z->keys[j] = y->keys[j + T];
        y->keys[j + T] = 0;
    }
    // or
    // for (int j = T; j < 2 * T - 1; j++) {
    //     z->keys[j - T] = y->keys[j];
    //     y->keys[j] = 0;
    // }

    z->n = T - 1;
    y->n = T - 1;

    for (int j = x->n; j >= i + 1; j--) {
        x->children[j + 1] = x->children[j];
    }

    x->children[i + 1] = z;

    for (int j = x->n - 1; j >= i; j--) {
        x->keys[j + 1] = x->keys[j];
    }

    x->keys[i] = y->keys[T - 1];
    y->keys[T - 1] = 0;

    x->n++;
}

// Function to insert a key into the B-tree
void insert(struct BTreeNode** root, int key) {
    struct BTreeNode* r = *root;

    if (r->n == 2 * T - 1) {
        struct BTreeNode* newNode = createNode(0);
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

    if (x->leaf) {
        while (i >= 0 && key < x->keys[i]) {
            x->keys[i + 1] = x->keys[i];
            i--;
        }

        x->keys[i + 1] = key;
        x->n++;
    } else {
        while (i >= 0 && key < x->keys[i]) {
            i--;
        }

        i++;
        if (x->children[i]->n == 2 * T - 1) {
            splitChild(x, i, x->children[i]);
            if (key > x->keys[i]) {
                i++;
            }
        }
        insertNonFull(x->children[i], key);
    }
}

// Function to find the successor of a key in the B-tree
int findSuccessor(struct BTreeNode* x) {
    while (!x->leaf) {
        x = x->children[0];
    }
    return x->keys[0];
}

// Function to delete a key from the B-tree
void deleteKey(struct BTreeNode** root, int key) {
    struct BTreeNode* r = *root;

    // if (r == NULL) {
    //     return;
    // }

    deleteKeyFromNode(r, key);

    if (r->n == 0) {
        // If the root becomes empty after deletion, update the root
        *root = r->children[0];
        free(r);
    }
}

// Function to delete a key from a B-tree node
void deleteKeyFromNode(struct BTreeNode* x, int key) {
    int i = 0;
    while (i < x->n && key > x->keys[i]) {
        i++;
    }

    if (i < x->n && key == x->keys[i]) {
        // Key found in this node
        if (x->leaf) {
            // Case 1: Key is in a leaf node
            for (int j = i; j < x->n - 1; j++) {
                x->keys[j] = x->keys[j + 1];
            }
            x->n--;
        } else {
            // Case 2: Key is in an internal node
            int successor = findSuccessor(x->children[i + 1]);
            x->keys[i] = successor;
            deleteKeyFromNode(x->children[i + 1], successor);
        }
    } else {
        // Key may be in a child node
        if (x->leaf) {
            // Key is not in the tree
            return;
        }

        if (x->children[i]->n < T) {
            // Case 3a: Child has less than T keys
            if (i > 0 && x->children[i - 1]->n >= T) {
                // Borrow a key from the left sibling
                borrowFromLeftSibling(x, i);
            } else if (i < x->n && x->children[i + 1]->n >= T) {
                // Borrow a key from the right sibling
                borrowFromRightSibling(x, i);
            } else {
                // Merge the child with its sibling
                if (i < x->n) {
                    mergeNodes(x, i);
                } else {
                    mergeNodes(x, i - 1);
                }
            }
        }
        deleteKeyFromNode(x->children[i], key);
    }
}

// Function to borrow a key from the left sibling
void borrowFromLeftSibling(struct BTreeNode* x, int idx) {
    struct BTreeNode* child = x->children[idx];
    struct BTreeNode* leftSibling = x->children[idx - 1];

    // Shift keys in child to make room for a borrowed key
    for (int i = child->n - 1; i >= 0; i--) {
        child->keys[i + 1] = child->keys[i];
    }

    // Shift children pointers if the child is not a leaf
    if (!child->leaf) {
        for (int i = child->n; i >= 0; i--) {
            child->children[i + 1] = child->children[i];
        }
    }

    // Move a key from the left sibling to the parent node
    child->keys[0] = x->keys[idx - 1];

    // Move the rightmost key from the left sibling to the child
    x->keys[idx - 1] = leftSibling->keys[leftSibling->n - 1];

    // Update counts in child and left sibling
    child->n++;
    leftSibling->n--;
}

// Function to borrow a key from the right sibling
void borrowFromRightSibling(struct BTreeNode* x, int idx) {
    struct BTreeNode* child = x->children[idx];
    struct BTreeNode* rightSibling = x->children[idx + 1];

    // Move a key from the right sibling to the parent node
    child->keys[child->n] = x->keys[idx];

    // Move the leftmost key from the right sibling to the child
    x->keys[idx] = rightSibling->keys[0];

    // Shift keys in the right sibling
    for (int i = 0; i < rightSibling->n - 1; i++) {
        rightSibling->keys[i] = rightSibling->keys[i + 1];
    }

    // Shift children pointers if the child is not a leaf
    if (!child->leaf) {
        child->children[child->n + 1] = rightSibling->children[0];
        for (int i = 0; i < rightSibling->n; i++) {
            rightSibling->children[i] = rightSibling->children[i + 1];
        }
    }

    // Update counts in child and right sibling
    child->n++;
    rightSibling->n--;
}

// Function to merge a child with its sibling
void mergeNodes(struct BTreeNode* x, int idx) {
    struct BTreeNode* leftChild = x->children[idx];
    struct BTreeNode* rightChild = x->children[idx + 1];

    // Move the key from the parent node to the left child
    leftChild->keys[T - 1] = x->keys[idx];

    // Copy keys from the right child to the left child
    for (int i = 0; i < T - 1; i++) {
        leftChild->keys[T + i] = rightChild->keys[i];
    }

    // Copy children pointers from the right child to the left child
    if (!leftChild->leaf) {
        for (int i = 0; i < T; i++) {
            leftChild->children[T + i] = rightChild->children[i];
        }
    }

    // Update counts in the left child
    leftChild->n = 2 * T - 1;

    // Shift keys and children pointers in the parent node
    for (int i = idx + 1; i < x->n; i++) {
        x->keys[i - 1] = x->keys[i];
    }

    for (int i = idx + 2; i <= x->n; i++) {
        x->children[i - 1] = x->children[i];
    }

    // Update count in the parent node
    x->n--;

    // Free memory allocated for the right child
    free(rightChild);
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
    struct BTreeNode* root = createNode(1); // Create the root node as a leaf node

    // Insert keys into the B-tree
    int keys[] = {10, 20, 5, 15, 25, 30, 35};
    int numKeys = sizeof(keys) / sizeof(keys[0]);

    for (int i = 0; i < numKeys; i++) {
        insert(&root, keys[i]);
        printf("Inserted: %d\n", keys[i]);
    }

    printf("B-tree before deletion: ");
    inorderTraversal(root);
    printf("\n");

    // Delete a key from the B-tree
    int keyToDelete = 20;
    deleteKey(&root, keyToDelete);

    printf("B-tree after deletion: ");
    inorderTraversal(root);
    printf("\n");

    return 0;
}

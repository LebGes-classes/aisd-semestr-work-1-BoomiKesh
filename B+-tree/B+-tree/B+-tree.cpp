
#include <iostream>
#include "Tree.h"
#include <vector>

class Node {
public:
    bool isLeaf;
    std::vector<int> keys;
    std::vector<Node*> children;
    Node* next;
    Node* parent;

    Node(bool isLeaf = false, Node* parent = nullptr)
        : isLeaf(isLeaf), parent(parent), next(nullptr) {
    }

    ~Node() {
        for (Node* child : children) {
            delete child;
        }
    }
};

class BPlusTree : public Tree {
private:
    Node* root;
    int degree;

    // Поиск позиции в векторе
    int findPos(const std::vector<int>& keys, int key) {
        int i = 0;
        while (i < keys.size() && keys[i] < key) {
            i++;
        }
        return i;
    }

    void insertInternal(int key, Node* parent, Node* child) {
        int pos = findPos(parent->keys, key);
        parent->keys.insert(parent->keys.begin() + pos, key);
        parent->children.insert(parent->children.begin() + pos + 1, child);

        if (parent->keys.size() > 2 * degree - 1) {
            Node* newNode = new Node(false, parent->parent);
            int splitPos = parent->keys.size() / 2;
            int middleKey = parent->keys[splitPos];

            newNode->keys.assign(parent->keys.begin() + splitPos + 1, parent->keys.end());
            newNode->children.assign(parent->children.begin() + splitPos + 1, parent->children.end());
            parent->keys.erase(parent->keys.begin() + splitPos, parent->keys.end());
            parent->children.erase(parent->children.begin() + splitPos + 1, parent->children.end());

            for (Node* node : newNode->children) {
                node->parent = newNode;
            }

            if (parent == root) {
                Node* newRoot = new Node(false);
                newRoot->keys.push_back(middleKey);
                newRoot->children.push_back(parent);
                newRoot->children.push_back(newNode);
                parent->parent = newRoot;
                newNode->parent = newRoot;
                root = newRoot;
            }
            else {
                insertInternal(middleKey, parent->parent, newNode);
            }
        }
    }

public:
    explicit BPlusTree(int degree) : root(nullptr), degree(degree) {}

    ~BPlusTree() {
        delete root;
    }

    //Вставка
    void insert(int key) override {
        if (root == nullptr) {
            root = new Node(true);
            root->keys.push_back(key);
            return;
        }

        Node* current = root;
        Node* parent = nullptr;

        while (!current->isLeaf) {
            parent = current;
            int i = findPos(current->keys, key);
            current = current->children[i];
        }

        int pos = findPos(current->keys, key);
        current->keys.insert(current->keys.begin() + pos, key);

        if (current->keys.size() > 2 * degree - 1) {
            Node* newLeaf = new Node(true, current->parent);
            int splitPos = current->keys.size() / 2;

            newLeaf->keys.assign(current->keys.begin() + splitPos, current->keys.end());
            current->keys.erase(current->keys.begin() + splitPos, current->keys.end());

            newLeaf->next = current->next;
            current->next = newLeaf;

            if (current == root) {
                Node* newRoot = new Node(false);
                newRoot->keys.push_back(newLeaf->keys[0]);
                newRoot->children.push_back(current);
                newRoot->children.push_back(newLeaf);
                current->parent = newRoot;
                newLeaf->parent = newRoot;
                root = newRoot;
            }
            else {
                insertInternal(newLeaf->keys[0], current->parent, newLeaf);
            }
        }
    }
    //Поиск элемента
    bool search(int key) override {
        Node* current = root;
        if (!current) return false;

        while (!current->isLeaf) {
            int i = findPos(current->keys, key);
            current = current->children[i];
        }

        for (int k : current->keys) {
            if (k == key) return true;
            if (k > key) break;
        }
        return false;
    }
    //Удаление элемента
    void remove(int key) override {
        if (!root) return;

        Node* current = root;
        Node* parent = nullptr;

        while (!current->isLeaf) {
            parent = current;
            int i = findPos(current->keys, key);
            current = current->children[i];
        }

        for (int i = 0; i < current->keys.size(); ++i) {
            if (current->keys[i] == key) {
                current->keys.erase(current->keys.begin() + i);
                break;
            }
        }
    }

    //Вывод дерева
    std::string toString() const override {
        if (!root) return "Empty tree";

        std::string result;
        std::vector<Node*> nodes = { root };

        while (!nodes.empty()) {
            std::vector<Node*> nextLevel;
            for (Node* node : nodes) {
                result += "[";
                for (int i = 0; i < node->keys.size(); ++i) {
                    result += std::to_string(node->keys[i]);
                    if (i < node->keys.size() - 1) result += ", ";
                }
                result += "] ";

                if (!node->isLeaf) {
                    for (Node* child : node->children) {
                        nextLevel.push_back(child);
                    }
                }
            }
            result += "\n";
            nodes = nextLevel;
        }

        return result;
    }
};

int main() {
    BPlusTree tree(2);
    tree.insert(10);
    std::cout << tree.toString() << std::endl;
    tree.insert(20);
    std::cout << tree.toString() << std::endl;
    tree.insert(5);
    std::cout << tree.toString() << std::endl;
    tree.insert(7);
    std::cout << tree.toString() << std::endl;
    tree.insert(13);
    std::cout << tree.toString() << std::endl;
    tree.insert(25);
    std::cout << tree.toString() << std::endl;
    tree.insert(41);
    std::cout << tree.toString() << std::endl;
    tree.insert(12);
    std::cout << tree.toString() << std::endl;
    tree.insert(13);
    std::cout << tree.toString() << std::endl;
    tree.insert(13);
    std::cout << tree.toString() << std::endl;
    tree.insert(11);
    std::cout << tree.toString() << std::endl;
    tree.insert(65);
    std::cout << tree.toString() << std::endl;

    std::cout << tree.toString() << std::endl;
    return 0;
}


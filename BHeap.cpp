#include <iostream>
#include "CircularDynamicArray.cpp"

using namespace std;

template <class keytype>
class BHeap {
    private:
        struct TreeNode{
            TreeNode* leftChild;
            TreeNode* rightSibling;
            TreeNode* parent;
            keytype k;
            int degree;

            TreeNode(){
                leftChild = nullptr;
                rightSibling = nullptr;
                parent = nullptr;
                degree = 0;
            }

            TreeNode(keytype key){
                leftChild = nullptr;
                rightSibling = nullptr;
                parent = nullptr;
                degree = 0;
                k = key;
            }

            TreeNode(const TreeNode& old){
                leftChild = old.leftChild;
                rightSibling = old.rightSibling;
                parent = old.parent;
                k = old.k;
                degree = old.degree;
            }

            ~TreeNode(){
                leftChild = nullptr;
                rightSibling = nullptr;
                parent = nullptr;
            }

            TreeNode& operator=(const TreeNode& old){
                if(old == *this){
                    return *this;
                }
                
                delete leftChild;
                delete rightSibling;
                delete parent;

                leftChild = old.leftChild;
                rightSibling = old.rightSibling;
                parent = old.parent;
                degree = old.degree;
                k = old.k;
                return *this;
            }
        };

        CircularDynamicArray <TreeNode*> h;
        TreeNode* min;
        keytype error;

    public:
        BHeap(){
            h = CircularDynamicArray<TreeNode*>();
            min = nullptr;
        }

        BHeap(keytype* k,  int s){
            h = CircularDynamicArray<TreeNode*>();
            min = nullptr;
            for(int i = 0; i < s; i++){
                insert(k[i]);
            }
        }

        ~BHeap(){
            
        }

        BHeap(const BHeap& old){
            for(int i = 0; i < old.h.length();i++){
                h.addEnd( old.h[i]);
            }
            min = old.min;
        }

        const BHeap& operator=(const BHeap& old){
            for(int i = 0; i<h.length();i++){
                h[i] = old.h[i];
            }
            min = old.min;
            return *this;
        }

        void insert(keytype k){
            TreeNode* ins = new TreeNode(k);
            if(h.length() == 0){
                min = ins;
            }
            if(min->k > ins->k){
                min = ins;
            }
            addTreeToHeap(ins);
            // int i = 0;
            // while(1){
            //     //cout << "In while" << endl;
            //     if(i == h.length()){ 
            //         h.addEnd(ins);
            //         break;
            //     }
            //     if (h[i] == nullptr){
            //         h[i] = ins;
            //         break;
            //     }
            //     else{
            //         if(ins->k < h[i]->k){
            //             TreeNode* temp = ins->leftChild;
            //             ins->leftChild = h[i];
            //             h[i]->rightSibling = temp;
            //             h[i]->parent = ins;
            //         }
            //         else{
            //             TreeNode* temp = h[i]->leftChild;
            //             h[i]->leftChild = ins;
            //             ins->rightSibling = temp;
            //             ins->parent = h[i];
            //             ins = h[i];
            //         }
            //         ins->degree = (i+1);
            //         h[i] = nullptr;
            //         i++;
            //     }
            // }
        }

        void recursivePre(TreeNode* node){
            if(node == nullptr) return;
            cout << node->k <<" ";
            recursivePre(node->leftChild);
            recursivePre(node->rightSibling);
        }

        void printKey(){
            for(int i = 0 ;i<h.length();i++){
                if(h[i] != nullptr){
                    cout << "B" << i << endl;
                    recursivePre(h[i]);
                    cout << endl;
                    cout << endl;
                }
            }
        }

        keytype peekKey(){
            if(min == nullptr){
                cout << "Error: Nothing in the heap" << endl;
                return error;
            }
            return min->k;
        }

        void expandHeap(int correctDegree){
            int i = 0;
            while(1){
                if(h.length()-1 == correctDegree){
                    return;
                }
                else{
                    h.addEnd(nullptr);
                }
            }
        }

        void addTreeToHeap(TreeNode* root){
            int i = root->degree;
            if(i > h.capacity()){
                expandHeap(i);
            }
            while(1){
                if(i == h.length()){ 
                    h.addEnd(root);
                    break;
                }
                if (h[i] == nullptr){
                    h[i] = root;
                    break;
                }
                else{
                    if(root->k < h[i]->k){
                        TreeNode* temp = root->leftChild;
                        root->leftChild = h[i];
                        h[i]->rightSibling = temp;
                        h[i]->parent = root;
                    }
                    else{
                        TreeNode* temp = h[i]->leftChild;
                        h[i]->leftChild = root;
                        root->rightSibling = temp;
                        root->parent = h[i];
                        root = h[i];
                    }
                    root->degree = (i+1);
                    h[i] = nullptr;
                    i++;
                }
            }
        }

        void merge(BHeap <keytype> &H2){
            for(int i = 0; i < H2.h.length();i++){
                if(H2.h[i] != nullptr) addTreeToHeap(H2.h[i]);
            }
            setMin();
            H2.h.clear();
        }

        void setMin(){
            int currMinInd = 0;
            while(1){
                if(h[currMinInd] != nullptr || currMinInd == h.length() - 1) break;
                currMinInd++;
            }
            for(int i = currMinInd + 1; i < h.length();i++){
                if(h[i] != nullptr){     
                    if((h[i]->k < h[currMinInd]->k)){
                        currMinInd = i;
                }
                }
            }
            min = h[currMinInd];
        }

        keytype extractMin(){
            keytype ret = min->k;
            if(h.length() == 1){
                h.clear();
                return ret;
            }
            CircularDynamicArray<TreeNode*> tempCDA(min->degree);
            TreeNode* curr = min->leftChild;
            h[min->degree] = nullptr;
            while(curr != nullptr){
                tempCDA[curr->degree] = curr;
                TreeNode* prev = curr;
                curr = curr->rightSibling;
                prev->rightSibling = nullptr;
                prev->parent = nullptr;
            }
            for(int i = 0; i < tempCDA.length();i++){
                if(tempCDA[i] != nullptr) addTreeToHeap(tempCDA[i]);
            }
            tempCDA.clear();
            setMin();
            return ret;
        }
};
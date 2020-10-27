#include <iostream>
#include <functional>
#include <algorithm>

template<typename T = double>
class AVLTreeNode{
    
private:
    T val;
    int height;
    AVLTreeNode<T> *left, *right;

    public : AVLTreeNode(T num = 0, std::function<int(T, T)> compareFunction = nullptr){
        this->val = num;
        this->left = nullptr;
        this->right = nullptr;
        this->height = 0;
    }

    ~AVLTreeNode(){
        if(getLeftNode() != nullptr)
            delete this->left;
        if(getRightNode() != nullptr)
            delete this->right;
    };

    static AVLTreeNode<T> *insertTreeNode(AVLTreeNode<T> *node, AVLTreeNode<T> *TargetNode){
        if (node->compareTo(TargetNode) < 0){
            if (node->getRightNode() == nullptr){
                node->setRightNode(TargetNode);
            }else{
                node->setRightNode(insertTreeNode(node->getRightNode(), TargetNode));
            }
        }else{
            if (node->getLeftNode() == nullptr){
                node->setLeftNode(TargetNode);
            }else{
                node->setLeftNode(insertTreeNode(node->getLeftNode(), TargetNode));
            }
        }

        node->updateHeight();
        return node->adjustTreeBalance();
    }

    static AVLTreeNode<T> *deleteTreeNode(AVLTreeNode<T> *node, AVLTreeNode<T> *TargetNode){
        if(node == nullptr)
            return nullptr;
        int position = node->compareTo(TargetNode);
        if(position < 0){
            node->setRightNode(deleteTreeNode(node->getRightNode(), TargetNode));
        }else if(position > 0){
            node->setLeftNode(deleteTreeNode(node->getLeftNode(), TargetNode));
        }else{
            if(node == TargetNode){
                if(node->getLeftNode() == nullptr && node->getRightNode() == nullptr){
                    delete node;
                    return nullptr; // no leaf
                }else if(node->getLeftNode() == nullptr){
                    AVLTreeNode *ret = node->getRightNode();
                    node->setRightNode(nullptr);
                    delete node;
                    return ret;
                }else if(node->getRightNode() == nullptr){
                    AVLTreeNode *ret = node->getLeftNode();
                    node->setLeftNode(nullptr);
                    delete node;
                    return ret;
                }else{

                    // find the parent of the max value node on the left tree.
                    if(node->getLeftNode()->getRightNode() == nullptr){
                        node = TargetNode->getLeftNode();
                        node->setRightNode(TargetNode->getRightNode());
                        TargetNode->setLeftNode(nullptr);
                        TargetNode->setRightNode(nullptr);
                        delete TargetNode;
                    }else{
                        AVLTreeNode *targetLeft = TargetNode->getLeftNode();
                        AVLTreeNode *targetRight = TargetNode->getRightNode();
                        AVLTreeNode *ptr = targetLeft;
                        while(ptr->getRightNode() != nullptr && ptr->getRightNode()->getRightNode() != nullptr){
                            ptr = ptr->getRightNode();
                        }
                        node = ptr->getRightNode();
                        ptr->setRightNode(TargetNode);
                        TargetNode->setLeftNode(node->getLeftNode());
                        TargetNode->setRightNode(node->getRightNode());
                        node->setLeftNode(targetLeft);
                        node->setRightNode(targetRight);

                        ptr->setRightNode(deleteTreeNode(ptr, TargetNode));
                        node = ptr;
                    }
                
                }
            }else{
                node->setLeftNode(deleteTreeNode(node->getLeftNode(), TargetNode));
            }
        }

        node->updateHeight();
        return node->adjustTreeBalance();
    }

    T getValue() const{
        return this->val;
    }

    void printOut() const{
        if(getLeftNode() != nullptr)
            getLeftNode()->printOut();

        std::cout << " " << getValue();

        if (getRightNode() != nullptr)
            getRightNode()->printOut();
    }

private:
    void setLeftNode(AVLTreeNode *node){
        this->left = node;
    }

    AVLTreeNode *getLeftNode() const{
        return this->left;
    }

    void setRightNode(AVLTreeNode *node){
        this->right = node;
    }

    AVLTreeNode *getRightNode() const{
        return this->right;
    }

    AVLTreeNode *leftRotation(){
        AVLTreeNode *ret = this;

        if (this->right != nullptr){
            ret = getRightNode();
            setRightNode(ret->getLeftNode());
            ret->setLeftNode(this);
        }

        return ret;
    }

    AVLTreeNode *rightRotation(){
        AVLTreeNode *ret = this;

        if (this->left != nullptr)
        {
            ret = getLeftNode();
            setLeftNode(ret->getRightNode());
            ret->setRightNode(this);
        }

        return ret;
    }

    int getHeight() const{
        return std::max((getLeftNode() == nullptr ? -1 : getLeftNode()->getHeight()),
                        (getRightNode() == nullptr ? -1 : getRightNode()->getHeight())) +
               1;
    }

    int getBalance() const{
        return (getLeftNode() == nullptr ? -1 : getLeftNode()->getHeight()) -
               (getRightNode() == nullptr ? -1 : getRightNode()->getHeight());
    }

    void updateHeight(){
        this->height = getHeight();
    }

    AVLTreeNode *adjustTreeBalance(){
        AVLTreeNode *ret = this;

        int bfValue = getBalance();
        if(bfValue > 1){
            if (getLeftNode() != nullptr && getLeftNode()->getLeftNode() != nullptr){
                // LL type
                ret = rightRotation();
            }else{
                // LR type
                setLeftNode(getLeftNode()->leftRotation());
                ret = rightRotation();
            }
        }else if(bfValue < -1){
            if (getRightNode() != nullptr && getRightNode()->getRightNode() != nullptr){
                // RR type
                ret = leftRotation();
            }else{
                // RL type
                setRightNode(getRightNode()->rightRotation());
                ret = leftRotation();
            }
        }

        return ret;
    }

    int compareTo(AVLTreeNode<T> *TargetNode){
        return getValue() - TargetNode->getValue();
    }

};

int main(int, char**) {
    AVLTreeNode<int> *n1 = new AVLTreeNode<int>(7);
    AVLTreeNode<int> *n2 = new AVLTreeNode<int>(9);
    AVLTreeNode<int> *n3 = new AVLTreeNode<int>(15);
    AVLTreeNode<int> *n4 = new AVLTreeNode<int>(16);
    AVLTreeNode<int> *n5 = new AVLTreeNode<int>(21);
    AVLTreeNode<int> *head = n1;

    head = AVLTreeNode<int>::insertTreeNode(head, n2);
    head = AVLTreeNode<int>::insertTreeNode(head, n3);
    head = AVLTreeNode<int>::insertTreeNode(head, n4);
    head = AVLTreeNode<int>::insertTreeNode(head, n5);

    head = AVLTreeNode<int>::deleteTreeNode(head, n4);

    head->printOut();

    delete head;
}

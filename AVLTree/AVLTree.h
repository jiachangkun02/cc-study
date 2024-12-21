//
// Created by jiachangkun on 24-12-17.
//

#ifndef AVLTREE_H
#define AVLTREE_H
#include <utility>
#include <pstl/execution_defs.h>

#endif //AVLTREE_H

#pragma once
#include <iostream>

template<typename K,typename V>
struct AVLTreeNode
{
    AVLTreeNode<K,V>* _left;
    AVLTreeNode<K,V>* _right;
    AVLTreeNode<K,V>* _parent;

    int _bf;//右边高度减左边高度

    std::pair<K,V> _kv;
    AVLTreeNode(const std::pair<K,V>& kv)
        :_left(nullptr)
         ,_right(nullptr)
         ,_parent(nullptr)
         ,_bf(0)
         ,_kv(kv)
    {

    }
};

template<typename K,typename V>
class AVLTree
{
    typedef AVLTreeNode<K,V> node;
public:
    bool insert(const std::pair<K,V>& kv)
    {
        if ((_root==nullptr))
        {
            _root=new node(kv);
            return true;
        }
        node* parent=nullptr;
        node* cur=_root;
        while (cur)
        {
            if (cur->_kv.first>kv.first)
            {
                parent=cur;
                cur=cur->_left;
            }
            else if (cur->_kv.first<kv.first)
            {
                parent=cur;
                cur=cur->_right;
            }
            else
            {
                return false;
            }
        }
        cur=new node(kv);
        if (parent->_kv.first>kv.first)
        {
            parent->_left=cur;
            cur->_parent=parent;
            return true;
        }
        else if (parent->_kv.first<kv.first)
        {
            parent->_right=cur;
            cur->_parent=parent;
            return true;
        }
        //更新平衡因子
        while (parent)
        {
            if (cur==parent->_right)
            {
                ++parent->_bf;
            }
            else if(cur==parent->_left)
            {
                --parent->_bf;
            }

            if (parent->_bf==0)
            {
                break;
            }
            else if (parent->_bf==1 || parent->_bf==-1)
            {
                //左右两边还不平衡 还要继续向上更新
                cur=parent;
                parent=parent->_parent;
            }
            else if (parent->_bf==-2 || parent->_bf==2)
            {
                //旋转处理
                if (parent->_bf==2)
                {
                    if (cur->_bf==1)
                    {//单旋
                        rotateL(parent);
                    }
                    else if (cur->_bf==-1)
                    {//双旋
                        rotateRL(parent);
                    }
                }
                else if (parent->_bf==-2)
                {
                    if (cur->_bf==-1)
                    { //单旋
                        rotateR(parent);
                    }
                    else if (cur->_bf==1)
                    {//双旋
                        rotateLR(parent);
                    }
                }
                //旋转完成后，parent所在的树的高度恢复到了插入节点前的高度
                break;
            }
        }
        return true;
    }


    void rotateL(node* parent)
    {
        node* subR=parent->_right;
        node* subRL=subR->_left;

        parent->_right=subRL;
        if (subRL)
        {
            subRL->_parent=parent;
        }
        subR->_left=parent;
        node* ppnode=parent->_parent;
        parent->_parent=subR;

        if (_root==parent)
        {
            _root=subR;
            subR->_parent==nullptr;
        }
        else
        {
            if (ppnode->_left==parent)
            {
                ppnode->_left=subR;
            }
            else
            {
                ppnode->_right=subR;
            }
            subR->_parent=ppnode;
        }
        parent->_bf=subR->_bf=0;
    }

    void rotateR(node* parent)
    {
        node* subL=parent->_left;
        node* subLR=subL->_right;

        parent->_left=subLR;
        if (subLR)
        {
            subLR->_parent=parent;
        }
        subL->_right=parent;
        node* ppnode=parent->_parent;
        parent->_parent=subL;

        if (_root==parent)
        {
            _root=subL;
            subL->_parent=nullptr;
        }
        else
        {
            if (ppnode->_left==parent)
            {
                ppnode->_left=subL;
            }
            if (ppnode->_right==parent)
            {
                ppnode->_right==subL;
            }
            subL->_parent=ppnode;
        }
        parent->_bf=subL->_bf=0;

    }

    //右左双旋 先右再左
    void rotateRL(node* parent)
    {
        node* subR=parent->_right;
        node* subRL=subR->_left;
        int bf=subRL->_bf;

        rotateR(parent->_right);
        rotateL(parent);

        if (bf==-1)
        {
            parent->_bf=0;
            subR->_bf=1;
            subRL->_bf=0;
        }
        else if (bf==1)
        {
            subR->_bf=0;
            parent->_bf=-1;
            subRL->_bf=0;
        }
        else if (bf==0)
        {
            subR->_bf=0;
            subRL->_bf=0;
            parent->_bf=0;
        }
    }

    //左右双旋 先左再右
    void rotateLR(node* parent)
    {
        node* subL=parent->_left;
        node* subLR=subL->_right;
        int bf=subLR->_bf;

        rotateL(subL);
        rotateR(parent);

        if (bf==-1)
        {
            parent->_bf=1;
            subL->_bf=0;
            subLR->_bf=0;
        }
        else if (bf==1)
        {
            subL->_bf=-1;
            parent->_bf=0;
            subLR->_bf=0;
        }
        else if (bf==0)
        {
            subL->_bf=0;
            subLR->_bf=0;
            parent->_bf=0;
        }
    }

    void _inorder(node* root)
    {
        if (root==nullptr)
        {
            return;
        }
        _inorder(root->_left);
        std::cout << root->_kv.first<<":"<<root->_kv.second<<" ";
        _inorder(root->_right);
    }
    void inorder()
    {
        _inorder(_root);
        std::cout << std::endl;
    }
private:
    node* _root;
};//cur 新增在parent的左边 parent的bf-- 新增在parent的右边bf++
 // 更新完parent的bf后 如果parent的bf==0 说明parent的高度不变 则更新结束 插入完成
//                     （左右两边平衡了 把不平衡的那一边给填平了）

//更新完parent的bf后 如果parent的bf==1或-1 则parent的高度变了，继续往上更新
//                  (同理 平衡变成不平衡了 会对上层有影响 继续往上更新)
//如果更新完后 bf==2或-2 则需要旋转处理


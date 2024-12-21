#include "AVLTree.h"
#include <cstring>

void test()
{
    int a[]={10 ,51,78,32,9,1,6,7};
    AVLTree<int,int> t;
    for (auto e : a)
    {
        t.insert(std::make_pair(e,e));
    }
    t.inorder();
}

int main()
{
    test();
    return 0;
}

#include "hsctrees.h"
#include "hsclib/inc_base.h"

hsctree *init_hsctree(ubi_btKillNodeRtn delfunc, ubi_btCompFunc cmpfunc, unsigned flags)
{
   hsctree *tree = umalloc(sizeof(*tree));

   if(tree) {
      ubi_trInitTree(&tree->r,cmpfunc,flags);
      tree->delfunc = delfunc;
   }
   return tree;
}

void del_hsctree(hsctree *tree)
{
   if(tree)
      ubi_trKillTree(&tree->r, tree->delfunc);
   ufree(tree);
}

ubi_trNode *new_hsctreenode(APTR data)
{
   ubi_trNode *nd = (ubi_trNode*)umalloc(sizeof(ubi_trNode) + sizeof(APTR));
   *HSCTREENODEDP(nd,APTR*) = data;
   return nd;
}


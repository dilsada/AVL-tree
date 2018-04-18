#ifndef _BIN_SEARCH_TREE_H_
#define _BIN_SEARCH_TREE_H_

#include <functional>
#include <cstddef>
#include <stack>
#include <list>
#include <ostream>


template <typename Key, typename Object, 
          typename Comparator=std::less<Key> >
class BinarySearchTree 
{
  private: 
    struct BinaryNode 
    {
      Key key; 
      Object data; 
      BinaryNode * left;
      BinaryNode * right;
      size_t height; //height info should be updated per insert & delete

      BinaryNode(const Key &, const Object &,
                 BinaryNode *, BinaryNode *, size_t =0);
    };

  public: 
    class Iterator 
    {
      public:
        Iterator( ); 
        Iterator & operator++( ); 
 
        Object & operator*( );  
        const Object & operator*( ) const; 

        bool operator==(const Iterator & rhs) const; 
        bool operator!=(const Iterator & rhs) const; 
      
      private:
        BinaryNode * current; 
        const BinaryNode * root; 
        std::stack<BinaryNode *> s; 
        bool useStack; 
        
      private:
        Iterator(BinaryNode *, const BinarySearchTree &, bool =true);
        

        void reverseinorder(BinaryNode *);

      friend class BinarySearchTree<Key, Object, Comparator>; 
    };

  public:
    BinarySearchTree( ); 
    ~BinarySearchTree( ); 

    void insert(const Key &, const Object &);
    void remove(const Key &); 
  public: 
    Iterator find(const Key &) const; 
    std::list<Iterator> find(const Key &, const Key &) const;

    Iterator begin( ) const; 
    Iterator end( ) const; 

  public: 
    int height( ) const; 
    size_t size( ) const; 
    bool empty( ) const; 
    void print(std::ostream &) const;
  
  private: 
    BinaryNode * root; 
    size_t nodes; 
    Comparator isLessThan; 

  private:
    
    void makeEmpty(BinaryNode * &);

    BinaryNode * find(const Key &, BinaryNode *) const;
    int height(BinaryNode *) const;
    void print(BinaryNode *, std::ostream &) const;

    template <typename T>
    static const T & max(const T &, const T &);

    
    void rotateWithLeftChild(BinaryNode * &);
    void rotateWithRightChild(BinaryNode * &);
    void doubleWithLeftChild(BinaryNode * &);
    void doubleWithRightChild(BinaryNode * &);
    
    

    void insertion(const Key &, const Object &, BinaryNode* &);
    BinaryNode* findmax(BinaryNode *) const;
    BinaryNode* findmin(BinaryNode *) const;
    void deletion(const Key &, BinaryNode* &);
    void balance(BinaryNode *&);
    void roadbalance(BinaryNode *&,BinaryNode *&);



  private:
    BinarySearchTree(const BinarySearchTree &);
    const BinarySearchTree & operator=(const BinarySearchTree &);
};


template <typename K, typename O, typename C>
BinarySearchTree<K,O,C>::BinaryNode::
BinaryNode(const K & _k, const O & _d,
           BinaryNode * _l, BinaryNode * _r, size_t _h)
  : key(_k), data(_d), left(_l), right(_r), height(_h)
{
}


template <typename K, typename O, typename C>
BinarySearchTree<K,O,C>::BinarySearchTree( )
  : root(NULL), nodes(0) 
{
}


template <typename K, typename O, typename C>
BinarySearchTree<K,O,C>::~BinarySearchTree( )
{
  makeEmpty(root);
}

template <typename K, typename O, typename C>
void
BinarySearchTree<K,O,C>::makeEmpty(BinaryNode * & t)
{
  if (t != NULL)
  {
    makeEmpty(t->left);
    makeEmpty(t->right);
    delete t;
    --nodes;
  }

  t = NULL;
}


template <typename K, typename O, typename C>
void
BinarySearchTree<K,O,C>::insert(const K & k, const O & x){

  if( find(k,root) == NULL)
    nodes++;

  insertion(k,x,root);

}

template <typename K, typename O, typename C>
void
BinarySearchTree<K,O,C>::insertion(const K & key, const O & obj, BinaryNode* & ptr){

  if(ptr == NULL){
    ptr = new BinaryNode(key,obj,NULL,NULL);
  }

  else{

    if(isLessThan(key, ptr->key)){
      insertion(key,obj,ptr->left);
      
      if(height(ptr->left) - height(ptr->right) > 1){
        
        if(isLessThan(key,ptr->left->key))
          rotateWithLeftChild(ptr);
        else
          doubleWithLeftChild(ptr); 
      
      }
    }

    else if(isLessThan(ptr->key,key)){
      insertion(key,obj,ptr->right);

      if(height(ptr->right) - height(ptr->left) > 1){

        if(isLessThan(ptr->right->key,key))
          rotateWithRightChild(ptr);
        else
          doubleWithRightChild(ptr);

      }
    }

    else{
      ptr->data = obj;
    }

  }

  ptr->height = max(height(ptr->left),height(ptr->right)) +1; 

}


template <typename K, typename O, typename C>
void
BinarySearchTree<K,O,C>::remove(const K & k)
{
    if(find(k,root) != NULL)
      nodes--;
    deletion(k,root);

}

template <typename K, typename O, typename C>
void
BinarySearchTree<K,O,C>::deletion(const K & key, BinaryNode* & ptr){
    
    if(ptr == NULL)
        return;

    else if(isLessThan(key,ptr->key))
        deletion(key,ptr->left);

    else if(isLessThan(ptr->key,key))
        deletion(key,ptr->right);

    else if(ptr->left != NULL && ptr->right != NULL){ //two child
        Iterator it(root,*this,true);
        while(it.current != ptr){
            ++it;
        }

        BinaryNode* successor = (++it).current; 
        

        if( ptr->right == successor){
            successor->left = ptr->left;
            BinaryNode* oldnode = ptr;
            ptr = successor;

            delete oldnode;
        }
        
        else{ 
            BinaryNode* sth = successor->right;

            while(true){
              if(it.current->left == successor)
                break;
              ++it;
                
            }

            BinaryNode* successor2 = it.current;

            successor2->left = sth;
            successor->right = ptr->right;
            successor->left = ptr->left;
            BinaryNode* oldnode = ptr;
            ptr = successor;
            delete oldnode;

            roadbalance(ptr->right,sth);
        } 
    }

    else{ //with zero or one child
        BinaryNode* oldnode = ptr;
        ptr = (ptr->left != NULL) ? ptr->left : ptr->right;
        delete oldnode;

        if(ptr == NULL)
          return;

    }
    ptr->height = max(height(ptr->left), height(ptr->right))+1;

    if(height(ptr->left) - height(ptr->right) == 2){

        if(height(ptr->left->left) - height(ptr->left->right) == 1)
            return rotateWithLeftChild(ptr);

        else
            return doubleWithLeftChild(ptr);
    }
    
    else if(height(ptr->right) - height(ptr->left) == 2){

        if(height(ptr->right->right) - height(ptr->right->left) == 1)
            return rotateWithRightChild(ptr);

        else
            return doubleWithRightChild(ptr);
    }
    
}




template <typename K, typename O, typename C>
void
BinarySearchTree<K,O,C>::balance(BinaryNode* &ptr){

    ptr->height = max(height(ptr->left), height(ptr->right))+1;

    if(height(ptr->left) - height(ptr->right) == 2){

        if(height(ptr->left->left) - height(ptr->left->right) == 1)
            return rotateWithLeftChild(ptr);

        else
            return doubleWithLeftChild(ptr);
    }
    
    else if(height(ptr->right) - height(ptr->left) == 2){

        if(height(ptr->right->right) - height(ptr->right->left) == 1)
            return rotateWithRightChild(ptr);

        else
            return doubleWithRightChild(ptr);

    }
}

//balances from ptr2 to ptr2 
template <typename K, typename O, typename C>
void
BinarySearchTree<K,O,C>::roadbalance(BinaryNode* &ptr1,BinaryNode* &ptr2){

    if(ptr1->left != ptr2)
        roadbalance(ptr1->left,ptr2);

    balance(ptr1);
}




template <typename K, typename O, typename C>
typename BinarySearchTree<K,O,C>::Iterator
BinarySearchTree<K,O,C>::find(const K & key) const
{
  BinaryNode * node = find(key, root);

  if (node == NULL)
  {
    return end();
  }
  else
  {               //not inorder iterator
    return Iterator(node, *this, false);
  }
}


template <typename K, typename O, typename C>
typename BinarySearchTree<K,O,C>::BinaryNode *
BinarySearchTree<K,O,C>::find(const K & key, BinaryNode * t) const
{
  if (t == NULL)
  {
    return NULL;
  }
  else if (isLessThan(key, t->key))
  {
    return find(key, t->left);
  }
  else if (isLessThan(t->key, key))
  {
    return find(key, t->right);
  }
  else //found
  {
    return t;
  }
}


template <typename K, typename O, typename C>
std::list<typename BinarySearchTree<K,O,C>::Iterator>
BinarySearchTree<K,O,C>::find(const K & lower, const K & upper) const{

  std::list<Iterator> mylist;
  
  if(!(isLessThan(upper,findmin(root)->key)) && !(isLessThan(findmax(root)->key,lower))){

    Iterator it(root,*this,true);

    while(isLessThan( it.current->key, lower)){
      ++it;
    }

    while(it!=end() && isLessThan( it.current->key, upper)){
      
      Iterator minion(it.current,*this,false);
      mylist.push_back(minion);

      ++it;
    }

    if(it != end()){

      if( !isLessThan(upper,it.current->key)){
        Iterator minion(it.current,*this,false);
        mylist.push_back(minion);
      }
    }
  }

  return mylist; 

}

template <typename K, typename O, typename C>
typename BinarySearchTree<K,O,C>::BinaryNode *
BinarySearchTree<K,O,C>::findmin(BinaryNode * t) const{
  if(t == NULL)
    return NULL;

  if(t->left == NULL)
    return t;

  else
    return findmin(t->left);
}

template <typename K, typename O, typename C>
typename BinarySearchTree<K,O,C>::BinaryNode *
BinarySearchTree<K,O,C>::findmax(BinaryNode * t) const{
  if(t == NULL)
    return NULL;

  if(t->right == NULL)
    return t;

  else
    return findmax(t->right);
}



template <typename K, typename O, typename C>
typename BinarySearchTree<K,O,C>::Iterator
BinarySearchTree<K,O,C>::begin() const
{
  return Iterator(root, *this);
}


template <typename K, typename O, typename C>
typename BinarySearchTree<K,O,C>::Iterator
BinarySearchTree<K,O,C>::end() const
{
  return Iterator(NULL, *this);
}


template <typename K, typename O, typename C>
int
BinarySearchTree<K,O,C>::height( ) const
{
  return height(root);
}


template <typename K, typename O, typename C>
int
BinarySearchTree<K,O,C>::height(BinaryNode * t) const
{
  return (t == NULL) ? -1 : t->height;
}


template <typename K, typename O, typename C>
size_t
BinarySearchTree<K,O,C>::size( ) const
{
  return nodes;
}


template <typename K, typename O, typename C>
bool
BinarySearchTree<K,O,C>::empty( ) const
{
  return nodes == 0;
}


template <typename K, typename O, typename C>
void
BinarySearchTree<K,O,C>::print(std::ostream & out) const
{
  print(root, out);
  out << '\n';
}


template <typename K, typename O, typename C>
void
BinarySearchTree<K,O,C>::print(BinaryNode * t, std::ostream & out) const
{
  if (t != NULL && t->left != NULL) 
  {
    out << '[';
    print( t->left, out );
  }
  else if (t != NULL && t->left == NULL && t->right != NULL)
  {
    out << "[";
  }

  if (t != NULL)
  {
    if (t->left == NULL && t->right == NULL)
    {
      out << '(' << (t->key) << ')';
    }
    else if (t->left != NULL || t->right != NULL)
    {
      out << '{' << (t->key) << ",H" << t->height << '}';
    }
  }
  
  if (t != NULL && t->right != NULL)
  {
    print( t->right, out );
    out << ']';
  }
  else if (t != NULL && t->left != NULL && t->right == NULL)
  {
    out << "]";
  }
}


template <typename K, typename O, typename C>
template <typename T>
const T &
BinarySearchTree<K,O,C>::max(const T & el1, const T & el2)
{
  return el1 > el2 ? el1 : el2;
}

template <typename K, typename O, typename C>
void 
BinarySearchTree<K,O,C>::rotateWithLeftChild(BinaryNode * & k2)
{
  BinaryNode *k1 = k2->left;
  k2->left = k1->right;
  k1->right = k2;
  k2->height = BinarySearchTree<K,O,C>::max(height(k2->left), 
                                            height(k2->right)) + 1;
  k1->height = BinarySearchTree<K,O,C>::max(height(k1->left), 
                                            height(k2)) + 1;
  k2 = k1;
}

template <typename K, typename O, typename C>
void 
BinarySearchTree<K,O,C>::rotateWithRightChild(BinaryNode * & k1)
{
  BinaryNode *k2 = k1->right;
  k1->right = k2->left;
  k2->left = k1;
  k1->height = BinarySearchTree<K,O,C>::max(height(k1->left), 
                                            height(k1->right)) + 1;
  k2->height = BinarySearchTree<K,O,C>::max(height(k2->right), 
                                            height(k1)) + 1;
  k1 = k2;
}

template <typename K, typename O, typename C>
void 
BinarySearchTree<K,O,C>::doubleWithLeftChild(BinaryNode * & k3)
{
  rotateWithRightChild(k3->left);
  rotateWithLeftChild(k3);
}

template <typename K, typename O, typename C>
void 
BinarySearchTree<K,O,C>::doubleWithRightChild(BinaryNode * & k1)
{
  rotateWithLeftChild(k1->right);
  rotateWithRightChild(k1);
}


template <typename K, typename O, typename C>
BinarySearchTree<K,O,C>::Iterator::Iterator( ) 
  : current(NULL), root(NULL), useStack(false)
{
}


template <typename K, typename O, typename C>
O &
BinarySearchTree<K,O,C>::Iterator::operator*( ){
  return current->data;
}


template <typename K, typename O, typename C>
const O & 
BinarySearchTree<K,O,C>::Iterator::operator*( ) const{
  return current->data; 
}


template <typename K, typename O, typename C>
bool 
BinarySearchTree<K,O,C>::Iterator::
operator==(const Iterator & rhs) const
{ 
  return current == rhs.current && 
         root == rhs.root; 
}


template <typename K, typename O, typename C>
bool 
BinarySearchTree<K,O,C>::Iterator::
operator!=(const Iterator & rhs) const
{ 
  return !(*this == rhs); 
}


template <typename K, typename O, typename C>
typename BinarySearchTree<K,O,C>::Iterator &
BinarySearchTree<K,O,C>::Iterator::operator++( ){

  if(useStack == false)
    current = NULL;
  else{
    if(s.empty())
      current = NULL;
    else{
      current = s.top();
      s.pop();
    }
  }
  return *this;
}


template <typename K, typename O, typename C>
BinarySearchTree<K,O,C>::Iterator::
Iterator(BinaryNode * p, const BinarySearchTree & rhs, bool stk){

  current = p;
  root = rhs.root;
  useStack = stk;


  if(p != NULL && useStack == true){
    reverseinorder(p);
    current = s.top();
    s.pop();
  }
}


template <typename K, typename O, typename C>
void 
BinarySearchTree<K,O,C>::Iterator::
reverseinorder(BinaryNode *ptr){
  if(ptr != NULL){
    reverseinorder(ptr -> right);
    s.push(ptr);
    reverseinorder(ptr -> left);
  }

}



#endif


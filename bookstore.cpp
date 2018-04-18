#include "bookstore.hpp"

BookStore::BookStore( ) 
{
}

void
BookStore::insert(const Book & book)
{
  std::string isbnkey = book.getISBN();
  primaryIndex.insert(isbnkey,book);

  BSTP::Iterator it = primaryIndex.find(isbnkey);
  Book* bookptr = &(*it);

  Book::SecondaryKey skey(book);
  secondaryIndex.insert(skey,bookptr);

  ternaryIndex.insert(skey,bookptr);

}

void
BookStore::remove(const std::string & isbn)
{

  BSTP::Iterator it = primaryIndex.find(isbn);

  if(it != primaryIndex.end()){
    
    Book::SecondaryKey skey(*it);

    primaryIndex.remove(isbn);
    secondaryIndex.remove(skey);
    ternaryIndex.remove(skey);
  }

}

void
BookStore::remove(const std::string & title,
                  const std::string & author)
{
  Book::SecondaryKey skey(title,author);

  BSTS::Iterator it2 = secondaryIndex.find(skey);

  if(it2 != secondaryIndex.end()){

    std::string isbnkey = (*it2)->getISBN();
  
    primaryIndex.remove(isbnkey);
    secondaryIndex.remove(skey);
    ternaryIndex.remove(skey);
  }

}

void
BookStore::removeAllBooksWithTitle(const std::string & title)
{

  Book::SecondaryKey key1(title,"a");
  Book::SecondaryKey key2(title,"{");

  std::list<BSTT::Iterator> mylist = ternaryIndex.find(key1,key2);

  while(!mylist.empty()){

    std::string isbnkey = (*(mylist.front()))->getISBN();
    remove(isbnkey);

    mylist.pop_front();
  }

}

void
BookStore::makeAvailable(const std::string & isbn)
{
  BSTP::Iterator it = primaryIndex.find(isbn);
  if(it != primaryIndex.end())
    (*it).setAvailable( );
}

void
BookStore::makeUnavailable(const std::string & title,
                           const std::string & author)
{
  Book::SecondaryKey skey(title,author);

  BSTS::Iterator it2 = secondaryIndex.find(skey);
  if(it2 != secondaryIndex.end()){
    std::string isbnkey = (*it2)->getISBN();
    BSTP::Iterator it = primaryIndex.find(isbnkey);
    (*it).setUnavailable( );    
  }

}

void
BookStore::updatePublisher(const std::string & author, 
                           const std::string & publisher)
{
  Book::SecondaryKey key1("a",author);
  Book::SecondaryKey key2("z",author);

  std::list<BSTS::Iterator> mylist = secondaryIndex.find(key1,key2);

  while(!mylist.empty()){

    BSTP::Iterator it = primaryIndex.find((*(mylist.front()))->getISBN());
    (*it).setPublisher(publisher);

    mylist.pop_front();

  }

}

void
BookStore::printBooksWithISBN(const std::string & isbn1,
                              const std::string & isbn2,
                              unsigned short since) const
{
  std::list<BSTP::Iterator> mylist = primaryIndex.find(isbn1,isbn2);
  while(!mylist.empty()){

    if((*(mylist.front())).getYear() >= since)
      std::cout<< *(mylist.front()) << std::endl;

    mylist.pop_front();
  }  

}

void
BookStore::printBooksOfAuthor(const std::string & author,
                              const std::string & first,
                              const std::string & last) const
{

  Book::SecondaryKey key1(first,author);
  Book::SecondaryKey key2(last,author);
  std::list<BSTS::Iterator> mylist = secondaryIndex.find(key1,key2);

  while(!mylist.empty()){

    BSTP::Iterator it = primaryIndex.find((*(mylist.front()))->getISBN());
    std::cout<< *it << std::endl;

    mylist.pop_front();
  }

}

void 
BookStore::printPrimarySorted( ) const
{
  BSTP::Iterator it;

  for (it=primaryIndex.begin(); it != primaryIndex.end(); ++it)
  {
    std::cout << *it << std::endl;
  }
}

void 
BookStore::printSecondarySorted( ) const
{
  BSTS::Iterator it;

  for (it = secondaryIndex.begin(); it != secondaryIndex.end(); ++it)
  {
    std::cout << *(*it) << std::endl;
  }
}

void 
BookStore::printTernarySorted( ) const
{
  BSTT::Iterator it;

  for (it = ternaryIndex.begin(); it != ternaryIndex.end(); ++it)
  {
    std::cout << *(*it) << std::endl;
  }
}


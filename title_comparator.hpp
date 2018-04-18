#ifndef _title_h__
#define _title_h__

#include "book.hpp"
#include <cstring>

class TitleComparator
{
  public:
    bool operator( ) (const Book::SecondaryKey & key1, 
                      const Book::SecondaryKey & key2) const
    {

    	const char *title1 = (key1.getTitle()).c_str();
    	const char *title2 = (key2.getTitle()).c_str();
    	
    	int titlecomp = strcasecmp(title1,title2);

    	if(titlecomp < 0)
    		return true;

    	else if(titlecomp == 0){
    		
    		const char *author1 = (key1.getAuthor()).c_str();
	    	const char *author2 = (key2.getAuthor()).c_str();

    		int authorcomp = strcasecmp(author1,author2);
    		
    		if(authorcomp < 0)
    			return true;
    		else
    			return false;
    	}
    	else
    		return false;

    }

};

#endif

//
//  App42FacebookProfile.h
//  App42Cocos2dX3.0Sample
//
//  Created by Shephertz Technologies Pvt Ltd on 30/06/14.
//
//

#ifndef __App42Cocos2dX3_0Sample__App42FacebookProfile__
#define __App42Cocos2dX3_0Sample__App42FacebookProfile__

#include <iostream>
using namespace std;
namespace App42
{
	class App42FacebookProfile
	{
	public:

		/**
		 * Getters
		 */
		string getName(){ return name; };
		string getPicture(){ return picture; };
		string getFbId(){ return fbId; };

		/**
		 * Setters
		 */
		void setName(string _name){ name = _name; };
		void setPicture(string _picture){ picture = _picture; };
		void setFbId(string _fbId){ fbId = _fbId; };

	protected:
		std::string name;
		std::string picture;
		std::string fbId;

	private:
	};
}//namespace App42
#endif /* defined(__App42Cocos2dX3_0Sample__App42FacebookProfile__) */

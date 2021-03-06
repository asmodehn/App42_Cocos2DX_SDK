//
//  App42Response.h
//  App42CPPSDK
//
//  Created by Rajeev Ranjan on 14/10/13.
//
//

#ifndef __App42CPPSDK__App42Response__
#define __App42CPPSDK__App42Response__

#include <iostream>
#include "JSONDocument.h"
#include "cJSON.h"
#include "Exceptions.h"
#include "App42HttpRequest.h"
#include "App42HttpResponse.h"
#include "App42Constants.h"
#include "App42CallBack.h"
#include "ACL.h"

using namespace App42::Network;

namespace App42
{

	class App42Response : public App42CallBack
	{
	protected:
		int _result;
		int totalRecords;
		std::string _body;
		const char *errorBuffer;
		App42HttpRequest *_httpRequest;
		SEL_App42CallFuncND _app42Selector;

	public:

		App42Response(SEL_App42CallFuncND app42Selector);

		virtual ~App42Response();

		bool isSuccess;
		// The HTTP response code.
		int getCode();
		int httpErrorCode;
		// The App Error Code.
		int appErrorCode;
		std::string errorDetails;
		std::string errorMessage;
		int getTotalRecords();
		void setTotalRecords();
		// The JSON body of the HTTP response containing details
		std::string getBody();
		virtual void onComplete(void *sender, void *data);

		void buildJsonDocument(cJSON *json, JSONDocument *jsonDocument);
		void buildErrorMessage();
	};

}//namespace App42

#endif /* defined(__App42CPPSDK__App42Response__) */

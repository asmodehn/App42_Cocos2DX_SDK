//
//  RewardService.cpp
//  App42CPPSDK
//
//  Created by Rajeev Ranjan on 18/10/13.
//
//

#include "RewardService.h"
#include "App42Constants.h"
#include "cJSON.h"
#include <map>
#include "Signing.h"
#include "BodyBuilder.h"
#include "App42RewardResponse.h"
#include "Connector.h"

using namespace App42::Network;
namespace App42
{
	// define the static..
	RewardService* RewardService::_instance = NULL;

	RewardService::RewardService()
	{

	}

	RewardService* RewardService::Initialize(string apikey, string secretkey)
	{
		if (_instance == NULL)
		{
			_instance = new RewardService();
		}
		_instance->Init(apikey, secretkey);
		return _instance;
	}

	RewardService* RewardService::getInstance()
	{
		return _instance;
	}

	void RewardService::Terminate()
	{
		if (_instance != NULL)
		{
			delete _instance;
			_instance = NULL;
		}
	}

	string BuildCreateRewardBody(string rewardName, string descrption)
	{

		cJSON *bodyJSON = cJSON_CreateObject();
		cJSON *app42JSON = cJSON_CreateObject();
		cJSON *rewardsJSON = cJSON_CreateObject();
		cJSON *rewardJSON = cJSON_CreateObject();

		cJSON_AddStringToObject(rewardJSON, "name", rewardName.c_str());
		cJSON_AddStringToObject(rewardJSON, "description", descrption.c_str());
		cJSON_AddItemReferenceToObject(rewardsJSON, "reward", rewardJSON);

		cJSON_AddItemReferenceToObject(app42JSON, "rewards", rewardsJSON);

		cJSON_AddItemReferenceToObject(bodyJSON, "app42", app42JSON);

		char *cptrFormatted = cJSON_PrintUnformatted(bodyJSON);
		string bodyString = cptrFormatted;

		cJSON_Delete(rewardsJSON);
		cJSON_Delete(rewardJSON);
		cJSON_Delete(app42JSON);
		cJSON_Delete(bodyJSON);

		free(cptrFormatted);

		return bodyString;

	}

	string BuildRewardBody(string gameName, string userName, string rewardName, double rewardPoints)
	{

		cJSON *bodyJSON = cJSON_CreateObject();
		cJSON *app42JSON = cJSON_CreateObject();
		cJSON *rewardsJSON = cJSON_CreateObject();
		cJSON *rewardJSON = cJSON_CreateObject();

		cJSON_AddStringToObject(rewardJSON, "gameName", gameName.c_str());
		cJSON_AddNumberToObject(rewardJSON, "points", rewardPoints);
		cJSON_AddStringToObject(rewardJSON, "name", rewardName.c_str());
		cJSON_AddStringToObject(rewardJSON, "userName", userName.c_str());
		cJSON_AddItemReferenceToObject(rewardsJSON, "reward", rewardJSON);

		cJSON_AddItemReferenceToObject(app42JSON, "rewards", rewardsJSON);

		cJSON_AddItemReferenceToObject(bodyJSON, "app42", app42JSON);

		char *cptrFormatted = cJSON_PrintUnformatted(bodyJSON);
		string bodyString = cptrFormatted;

		cJSON_Delete(rewardsJSON);
		cJSON_Delete(rewardJSON);
		cJSON_Delete(app42JSON);
		cJSON_Delete(bodyJSON);

		free(cptrFormatted);

		return bodyString;
	}

	void RewardService::CreateReward(const char* rewardName, const char* description, SEL_App42CallFuncND pSelector)
	{

		App42RewardResponse *response = new App42RewardResponse(pSelector);

		try
		{
			Util::throwExceptionIfStringNullOrBlank(description, "Description");
			Util::throwExceptionIfStringNullOrBlank(rewardName, "Reward Name");
			Util::throwExceptionIfCallBackIsNull(pSelector, "Callback");
		}
		catch (App42Exception *e)
		{
			std::string ex = e->what();
			response->httpErrorCode = e->getHttpErrorCode();
			response->appErrorCode = e->getAppErrorCode();
			response->errorDetails = ex;
			response->isSuccess = false;
			if (pSelector)
			{
				pSelector(response);
			}
			delete e;
			e = NULL;
			return;
		}

		/**
		 * Creating SignParams and signature
		 */
		map<string, string> signParams;
		populateSignParams(signParams);
		string rewardbody = BuildCreateRewardBody(rewardName, description);
		signParams["body"] = rewardbody;
		string signature = Util::signMap(secretKey, signParams);

		/**
		 * Creating URL
		 */
		string resource = "game/reward";
		string url = getBaseUrl(resource);
		url.append("?");
		string encodedUrl = url_encode(url);

		/**
		 * Creating Headers
		 */
		std::vector<std::string> headers;
		map<string, string> metaHeaders;
		populateMetaHeaderParams(metaHeaders);
		Util::BuildHeaders(metaHeaders, headers);
		string timestamp = Util::getTimeStamp();
		Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);

		/**
		 * Initiating Http call
		 */
		Util::executePost(encodedUrl, headers, rewardbody.c_str(), std::bind(&App42RewardResponse::onComplete, response, std::placeholders::_1, std::placeholders::_2));

	}

	void RewardService::GetAllRewards(SEL_App42CallFuncND pSelector)
	{
		App42RewardResponse *response = new App42RewardResponse(pSelector);

		try
		{
			Util::throwExceptionIfCallBackIsNull(pSelector, "Callback");
		}
		catch (App42Exception *e)
		{
			std::string ex = e->what();
			response->httpErrorCode = e->getHttpErrorCode();
			response->appErrorCode = e->getAppErrorCode();
			response->errorDetails = ex;
			response->isSuccess = false;
			if (pSelector)
			{
				pSelector(response);
			}
			delete e;
			e = NULL;
			return;
		}

		string timestamp = Util::getTimeStamp();
		/**
		 * Creating SignParams and signature
		 */
		map<string, string> signParams;
		Util::BuildGetSigningMap(apiKey, timestamp, VERSION, signParams);
		string signature = Util::signMap(secretKey, signParams);

		/**
		 * Creating URL
		 */
		string resource = "game/reward";
		string url = getBaseUrl(resource);
		url.append("?");
		string encodedUrl = url_encode(url);

		/**
		 * Creating Headers
		 */
		std::vector<std::string> headers;
		map<string, string> metaHeaders;
		populateMetaHeaderParams(metaHeaders);
		Util::BuildHeaders(metaHeaders, headers);
		Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);

		/**
		 * Initiating Http call
		 */
		Util::executeGet(encodedUrl, headers, std::bind(&App42RewardResponse::onComplete, response, std::placeholders::_1, std::placeholders::_2));

	}

	void RewardService::GetAllRewards(int max, int offset, SEL_App42CallFuncND pSelector)
	{
		App42RewardResponse *response = new App42RewardResponse(pSelector);

		try
		{
			Util::throwExceptionIfMaxIsNotValid(max, "Max");
			Util::throwExceptionIfCallBackIsNull(pSelector, "Callback");
		}
		catch (App42Exception *e)
		{
			std::string ex = e->what();
			response->httpErrorCode = e->getHttpErrorCode();
			response->appErrorCode = e->getAppErrorCode();
			response->errorDetails = ex;
			response->isSuccess = false;
			if (pSelector)
			{
				pSelector(response);
			}
			delete e;
			e = NULL;
			return;
		}

		string timestamp = Util::getTimeStamp();
		/**
		 * Creating SignParams and signature
		 */
		map<string, string> signParams;
		Util::BuildGetSigningMap(apiKey, timestamp, VERSION, signParams);
		signParams["max"] = Util::ItoA(max);
		signParams["offset"] = Util::ItoA(offset);
		string signature = Util::signMap(secretKey, signParams);

		/**
		 * Creating URL
		 */
		string resource = "game/reward";
		resource.append("/paging/");
		resource.append(Util::ItoA(max) + "/");
		resource.append(Util::ItoA(offset));

		string url = getBaseUrl(resource);
		url.append("?");
		string encodedUrl = url_encode(url);

		/**
		 * Creating Headers
		 */
		std::vector<std::string> headers;
		map<string, string> metaHeaders;
		populateMetaHeaderParams(metaHeaders);
		Util::BuildHeaders(metaHeaders, headers);
		Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);

		/**
		 * Initiating Http call
		 */
		Util::executeGet(encodedUrl, headers, std::bind(&App42RewardResponse::onComplete, response, std::placeholders::_1, std::placeholders::_2));

	}

	void RewardService::GetAllRewardsCount(SEL_App42CallFuncND pSelector)
	{
		App42RewardResponse *response = new App42RewardResponse(pSelector);

		try
		{
			Util::throwExceptionIfCallBackIsNull(pSelector, "Callback");
		}
		catch (App42Exception *e)
		{
			std::string ex = e->what();
			response->httpErrorCode = e->getHttpErrorCode();
			response->appErrorCode = e->getAppErrorCode();
			response->errorDetails = ex;
			response->isSuccess = false;
			if (pSelector)
			{
				pSelector(response);
			}
			delete e;
			e = NULL;
			return;
		}

		string timestamp = Util::getTimeStamp();
		/**
		 * Creating SignParams and signature
		 */
		map<string, string> signParams;
		Util::BuildGetSigningMap(apiKey, timestamp, VERSION, signParams);
		string signature = Util::signMap(secretKey, signParams);

		/**
		 * Creating URL
		 */
		string resource = "game/reward";
		resource.append("/count");
		string url = getBaseUrl(resource);
		url.append("?");
		string encodedUrl = url_encode(url);

		/**
		 * Creating Headers
		 */
		std::vector<std::string> headers;
		map<string, string> metaHeaders;
		populateMetaHeaderParams(metaHeaders);
		Util::BuildHeaders(metaHeaders, headers);
		Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);

		/**
		 * Initiating Http call
		 */
		Util::executeGet(encodedUrl, headers, std::bind(&App42RewardResponse::onComplete, response, std::placeholders::_1, std::placeholders::_2));

	}

	void RewardService::EarnRewards(const char* gameName, const char* userName, const char* rewardName, double rewardPoints, SEL_App42CallFuncND pSelector)
	{
		App42RewardResponse *response = new App42RewardResponse(pSelector);

		try
		{
			Util::throwExceptionIfStringNullOrBlank(gameName, "Game Name");
			Util::throwExceptionIfStringNullOrBlank(userName, "User Name");
			Util::throwExceptionIfStringNullOrBlank(rewardName, "Reward Name");
			Util::throwExceptionIfCallBackIsNull(pSelector, "Callback");
		}
		catch (App42Exception *e)
		{
			std::string ex = e->what();
			response->httpErrorCode = e->getHttpErrorCode();
			response->appErrorCode = e->getAppErrorCode();
			response->errorDetails = ex;
			response->isSuccess = false;
			if (pSelector)
			{
				pSelector(response);
			}
			delete e;
			e = NULL;
			return;
		}

		string timestamp = Util::getTimeStamp();

		/**
		 * Creating SignParams and signature
		 */
		map<string, string> signParams;
		populateSignParams(signParams);
		string rewardbody = BuildRewardBody(gameName, userName, rewardName, rewardPoints);
		signParams["body"] = rewardbody;
		string signature = Util::signMap(secretKey, signParams);

		/**
		 * Creating URL
		 */
		string resource = "game/reward";
		resource.append("/earn");
		string baseUrl = getBaseUrl(resource);
		baseUrl.append("?");
		string encodedUrl = url_encode(baseUrl);
		/**
		 * Creating Headers
		 */
		std::vector<std::string> headers;
		map<string, string> metaHeaders;
		populateMetaHeaderParams(metaHeaders);
		Util::BuildHeaders(metaHeaders, headers);
		Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);

		/**
		 * Initiating Http call
		 */
		Util::executePost(encodedUrl, headers, rewardbody.c_str(), std::bind(&App42RewardResponse::onComplete, response, std::placeholders::_1, std::placeholders::_2));
	}

	void RewardService::RedeemRewards(const char* gameName, const char* userName, const char* rewardName, double rewardPoints, SEL_App42CallFuncND pSelector)
	{
		App42RewardResponse *response = new App42RewardResponse(pSelector);

		try
		{
			Util::throwExceptionIfStringNullOrBlank(gameName, "Game Name");
			Util::throwExceptionIfStringNullOrBlank(userName, "User Name");
			Util::throwExceptionIfStringNullOrBlank(rewardName, "Reward Name");
			Util::throwExceptionIfCallBackIsNull(pSelector, "Callback");
		}
		catch (App42Exception *e)
		{
			std::string ex = e->what();
			response->httpErrorCode = e->getHttpErrorCode();
			response->appErrorCode = e->getAppErrorCode();
			response->errorDetails = ex;
			response->isSuccess = false;
			if (pSelector)
			{
				pSelector(response);
			}
			delete e;
			e = NULL;
			return;
		}

		/**
		 * Creating SignParams and signature
		 */
		map<string, string> signParams;
		populateSignParams(signParams);
		string rewardbody = BuildRewardBody(gameName, userName, rewardName, rewardPoints);
		signParams["body"] = rewardbody;
		string signature = Util::signMap(secretKey, signParams);

		/**
		 * Creating URL
		 */
		string resource = "game/reward";
		resource.append("/redeem");
		string baseUrl = getBaseUrl(resource);
		baseUrl.append("?");
		string encodedUrl = url_encode(baseUrl);

		/**
		 * Creating Headers
		 */
		std::vector<std::string> headers;
		map<string, string> metaHeaders;
		populateMetaHeaderParams(metaHeaders);
		Util::BuildHeaders(metaHeaders, headers);

		string timestamp = Util::getTimeStamp();
		Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);

		/**
		 * Initiating Http call
		 */
		Util::executePost(baseUrl, headers, rewardbody.c_str(), std::bind(&App42RewardResponse::onComplete, response, std::placeholders::_1, std::placeholders::_2));
	}

	void RewardService::GetGameRewardPointsForUser(const char* gameName, const char* userName, SEL_App42CallFuncND pSelector)
	{
		App42RewardResponse *response = new App42RewardResponse(pSelector);

		try
		{
			Util::throwExceptionIfStringNullOrBlank(gameName, "Game Name");
			Util::throwExceptionIfStringNullOrBlank(userName, "User Name");
			Util::throwExceptionIfCallBackIsNull(pSelector, "Callback");
		}
		catch (App42Exception *e)
		{
			std::string ex = e->what();
			response->httpErrorCode = e->getHttpErrorCode();
			response->appErrorCode = e->getAppErrorCode();
			response->errorDetails = ex;
			response->isSuccess = false;
			if (pSelector)
			{
				pSelector(response);
			}
			delete e;
			e = NULL;
			return;
		}

		string timestamp = Util::getTimeStamp();

		/**
		 * Creating SignParams and signature
		 */
		map<string, string> signParams;
		Util::BuildGetSigningMap(apiKey, timestamp, VERSION, signParams);
		signParams["gameName"] = gameName;
		signParams["userName"] = userName;
		string signature = Util::signMap(secretKey, signParams);

		/**
		 * Creating URL
		 */
		string resource = "game/reward/";
		resource.append(gameName);
		resource.append("/");
		resource.append(userName);
		string url = getBaseUrl(resource);
		url.append("?");
		string encodedUrl = url_encode(url);

		/**
		 * Creating Headers
		 */
		std::vector<std::string> headers;
		map<string, string> metaHeaders;
		populateMetaHeaderParams(metaHeaders);
		Util::BuildHeaders(metaHeaders, headers);
		Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);

		/**
		 * Initiating Http call
		 */
		Util::executeGet(encodedUrl, headers, std::bind(&App42RewardResponse::onComplete, response, std::placeholders::_1, std::placeholders::_2));
	}

	void RewardService::GetRewardByName(const char* rewardName, SEL_App42CallFuncND pSelector)
	{
		App42RewardResponse *response = new App42RewardResponse(pSelector);

		try
		{
			Util::throwExceptionIfStringNullOrBlank(rewardName, "Reward Name");
			Util::throwExceptionIfCallBackIsNull(pSelector, "Callback");
		}
		catch (App42Exception *e)
		{
			std::string ex = e->what();
			response->httpErrorCode = e->getHttpErrorCode();
			response->appErrorCode = e->getAppErrorCode();
			response->errorDetails = ex;
			response->isSuccess = false;
			if (pSelector)
			{
				pSelector(response);
			}
			delete e;
			e = NULL;
			return;
		}

		string timestamp = Util::getTimeStamp();

		/**
		 * Creating SignParams and signature
		 */
		map<string, string> signParams;
		Util::BuildGetSigningMap(apiKey, timestamp, VERSION, signParams);
		signParams["name"] = rewardName;
		string signature = Util::signMap(secretKey, signParams);

		/**
		 * Creating URL
		 */
		string resource = "game/reward/";
		resource.append(rewardName);
		string url = getBaseUrl(resource);
		url.append("?");
		string encodedUrl = url_encode(url);

		/**
		 * Creating Headers
		 */
		std::vector<std::string> headers;
		map<string, string> metaHeaders;
		populateMetaHeaderParams(metaHeaders);
		Util::BuildHeaders(metaHeaders, headers);
		Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);

		/**
		 * Initiating Http call
		 */
		Util::executeGet(encodedUrl, headers, std::bind(&App42RewardResponse::onComplete, response, std::placeholders::_1, std::placeholders::_2));
	}

	void RewardService::GetTopNRewardEarners(const char* gameName, const char* rewardName, int max, SEL_App42CallFuncND pSelector)
	{
		App42RewardResponse *response = new App42RewardResponse(pSelector);

		try
		{
			Util::throwExceptionIfStringNullOrBlank(gameName, "Game Name");
			Util::throwExceptionIfStringNullOrBlank(rewardName, "Reward Name");
			Util::throwExceptionIfMaxIsNotValid(max, "Max");
			Util::throwExceptionIfCallBackIsNull(pSelector, "Callback");
		}
		catch (App42Exception *e)
		{
			std::string ex = e->what();
			response->httpErrorCode = e->getHttpErrorCode();
			response->appErrorCode = e->getAppErrorCode();
			response->errorDetails = ex;
			response->isSuccess = false;
			if (pSelector)
			{
				pSelector(response);
			}
			delete e;
			e = NULL;
			return;
		}

		string timestamp = Util::getTimeStamp();

		/**
		 * Creating SignParams and signature
		 */
		map<string, string> signParams;
		Util::BuildGetSigningMap(apiKey, timestamp, VERSION, signParams);
		signParams["rewardName"] = rewardName;
		signParams["gameName"] = gameName;
		signParams["max"] = Util::ItoA(max);
		string signature = Util::signMap(secretKey, signParams);

		/**
		 * Creating URL
		 */
		string resource = "game/reward/";
		resource.append(gameName);
		resource.append("/");
		resource.append(rewardName);
		resource.append("/");
		resource.append(Util::ItoA(max));
		string url = getBaseUrl(resource);
		url.append("?");
		string encodedUrl = url_encode(url);

		/**
		 * Creating Headers
		 */
		std::vector<std::string> headers;
		map<string, string> metaHeaders;
		populateMetaHeaderParams(metaHeaders);
		Util::BuildHeaders(metaHeaders, headers);
		Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);

		/**
		 * Initiating Http call
		 */
		Util::executeGet(encodedUrl, headers, std::bind(&App42RewardResponse::onComplete, response, std::placeholders::_1, std::placeholders::_2));
	}

	void RewardService::GetAllRewardsByUser(const char* userName, const char* rewardName, SEL_App42CallFuncND pSelector)
	{
		App42RewardResponse *response = new App42RewardResponse(pSelector);

		try
		{
			Util::throwExceptionIfStringNullOrBlank(userName, "User Name");
			Util::throwExceptionIfStringNullOrBlank(rewardName, "Reward Name");
			Util::throwExceptionIfCallBackIsNull(pSelector, "Callback");
		}
		catch (App42Exception *e)
		{
			std::string ex = e->what();
			response->httpErrorCode = e->getHttpErrorCode();
			response->appErrorCode = e->getAppErrorCode();
			response->errorDetails = ex;
			response->isSuccess = false;
			if (pSelector)
			{
				pSelector(response);
			}
			delete e;
			e = NULL;
			return;
		}

		string timestamp = Util::getTimeStamp();

		/**
		 * Creating SignParams and signature
		 */
		map<string, string> signParams;
		Util::BuildGetSigningMap(apiKey, timestamp, VERSION, signParams);
		signParams["rewardName"] = rewardName;
		signParams["userName"] = userName;
		string signature = Util::signMap(secretKey, signParams);

		/**
		 * Creating URL
		 */
		string resource = "game/reward/";
		resource.append(userName);
		resource.append("/points/");
		resource.append(rewardName);
		string url = getBaseUrl(resource);
		url.append("?");
		string encodedUrl = url_encode(url);

		/**
		 * Creating Headers
		 */
		std::vector<std::string> headers;
		map<string, string> metaHeaders;
		populateMetaHeaderParams(metaHeaders);
		Util::BuildHeaders(metaHeaders, headers);
		Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);

		/**
		 * Initiating Http call
		 */
		Util::executeGet(encodedUrl, headers, std::bind(&App42RewardResponse::onComplete, response, std::placeholders::_1, std::placeholders::_2));
	}

	void RewardService::GetTopNRewardEarnersByGroup(const char* gameName, const char* rewardName, vector<std::string>userList, SEL_App42CallFuncND pSelector)
	{
		App42RewardResponse *response = new App42RewardResponse(pSelector);

		try
		{
			Util::throwExceptionIfStringNullOrBlank(gameName, "Game Name");
			Util::throwExceptionIfStringNullOrBlank(rewardName, "Reward Name");
			Util::throwExceptionIfCallBackIsNull(pSelector, "Callback");
		}
		catch (App42Exception *e)
		{
			std::string ex = e->what();
			response->httpErrorCode = e->getHttpErrorCode();
			response->appErrorCode = e->getAppErrorCode();
			response->errorDetails = ex;
			response->isSuccess = false;
			if (pSelector)
			{
				pSelector(response);
			}
			delete e;
			e = NULL;
			return;
		}

		string timestamp = Util::getTimeStamp();

		/**
		 * Creating SignParams and signature
		 */
		map<string, string> signParams;
		Util::BuildGetSigningMap(apiKey, timestamp, VERSION, signParams);

		string groupString = "[";
		groupString.append(Util::GetStringFromVector(userList));
		groupString.append("]");
		signParams["userList"] = groupString;


		signParams["rewardName"] = rewardName;
		signParams["gameName"] = gameName;
		string signature = Util::signMap(secretKey, signParams);

		/**
		 * Creating QueryParams
		 */
		map<string, string> queryParams;
		queryParams["userList"] = groupString;
		string queryString = buildQueryString(queryParams);

		/**
		 * Creating URL
		 */
		string resource = "game/reward/";
		resource.append(gameName);
		resource.append("/");
		resource.append(rewardName);
		resource.append("/group/points");
		string url = getBaseUrl(resource);
		url.append("?");
		url.append(queryString);
		string encodedUrl = url_encode(url);

		/**
		 * Creating Headers
		 */
		std::vector<std::string> headers;
		map<string, string> metaHeaders;
		populateMetaHeaderParams(metaHeaders);
		Util::BuildHeaders(metaHeaders, headers);
		Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);

		/**
		 * Initiating Http call
		 */
		Util::executeGet(encodedUrl, headers, std::bind(&App42RewardResponse::onComplete, response, std::placeholders::_1, std::placeholders::_2));
	}

	void RewardService::GetUserRankingOnReward(const char* gameName, const char* rewardName, const char* userName, SEL_App42CallFuncND pSelector)
	{
		App42RewardResponse *response = new App42RewardResponse(pSelector);

		try
		{
			Util::throwExceptionIfStringNullOrBlank(gameName, "Game Name");
			Util::throwExceptionIfStringNullOrBlank(rewardName, "Reward Name");
			Util::throwExceptionIfStringNullOrBlank(userName, "User Name");
			Util::throwExceptionIfCallBackIsNull(pSelector, "Callback");
		}
		catch (App42Exception *e)
		{
			std::string ex = e->what();
			response->httpErrorCode = e->getHttpErrorCode();
			response->appErrorCode = e->getAppErrorCode();
			response->errorDetails = ex;
			response->isSuccess = false;
			if (pSelector)
			{
				pSelector(response);
			}
			delete e;
			e = NULL;
			return;
		}

		string timestamp = Util::getTimeStamp();

		/**
		 * Creating SignParams and signature
		 */
		map<string, string> signParams;
		Util::BuildGetSigningMap(apiKey, timestamp, VERSION, signParams);
		signParams["rewardName"] = rewardName;
		signParams["gameName"] = gameName;
		signParams["userName"] = userName;
		string signature = Util::signMap(secretKey, signParams);

		/**
		 * Creating URL
		 */
		string resource = "game/reward/";
		resource.append(gameName);
		resource.append("/");
		resource.append(rewardName);
		resource.append("/rank/");
		resource.append(userName);
		string url = getBaseUrl(resource);
		url.append("?");
		string encodedUrl = url_encode(url);

		/**
		 * Creating Headers
		 */
		std::vector<std::string> headers;
		map<string, string> metaHeaders;
		populateMetaHeaderParams(metaHeaders);
		Util::BuildHeaders(metaHeaders, headers);
		Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);

		/**
		 * Initiating Http call
		 */
		Util::executeGet(encodedUrl, headers, std::bind(&App42RewardResponse::onComplete, response, std::placeholders::_1, std::placeholders::_2));
	}
}//namespace App42

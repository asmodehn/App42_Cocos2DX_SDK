//
//  ScoreService.cpp
//  App42CPPSDK
//
//  Created by Rajeev Ranjan on 18/10/13.
//
//

#include "ScoreService.h"
#include "App42Constants.h"
#include "cJSON.h"
#include <map>
#include "Signing.h"
#include "BodyBuilder.h"
#include "cocos-ext.h"
#include "App42GameResponse.h"
#include "Connector.h"

// define the static..
ScoreService* ScoreService::_instance = NULL;

ScoreService::ScoreService()
{
    
}

void ScoreService::Initialize(string apikey, string secretkey)
{
    if(_instance == NULL)
    {
        _instance = new ScoreService();
    }
    _instance->Init(apikey,secretkey);
}

ScoreService* ScoreService::getInstance()
{
    return _instance;
}

void ScoreService::Terminate()
{
    if(_instance != NULL)
    {
        delete _instance;
		_instance = NULL;
    }
}

string BuildScoreBody(string gameName, string userName, double score)
{
    
    cJSON *bodyJSON = cJSON_CreateObject();
    cJSON *app42JSON = cJSON_CreateObject();
    cJSON *gameJSON = cJSON_CreateObject();
    cJSON *sJSON = cJSON_CreateObject();
    cJSON *scoreJSON = cJSON_CreateObject();
    
    cJSON_AddStringToObject(gameJSON, "name", gameName.c_str());
    cJSON_AddNumberToObject(scoreJSON, "value", score);
    cJSON_AddStringToObject(scoreJSON, "userName", userName.c_str());
    cJSON_AddItemReferenceToObject(sJSON, "score", scoreJSON);
    
    cJSON_AddItemReferenceToObject(app42JSON, "game", gameJSON);
    cJSON_AddItemReferenceToObject(gameJSON, "scores", sJSON);
    
    cJSON_AddItemReferenceToObject(bodyJSON, "app42", app42JSON);
    
    char *cptrFormatted = cJSON_PrintUnformatted(bodyJSON);
    string bodyString = cptrFormatted;
    
    cJSON_Delete(gameJSON);
    cJSON_Delete(app42JSON);
    cJSON_Delete(bodyJSON);
    cJSON_Delete(sJSON);
    cJSON_Delete(scoreJSON);
    
    free(cptrFormatted);
    
    return bodyString;
    
}


void ScoreService::AddScore(string gameName, string userName, double score, CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector)
{
    
    map<string, string> postMap;
    populateSignParams(postMap);
    string addScoreBody = BuildScoreBody(gameName, userName,score);
    postMap["body"] = addScoreBody;
    
    string signature = Util::signMap(secretKey, postMap);
    
    string baseUrl = getBaseUrl("game/score/add");
    baseUrl.append("?");
    //Util::app42Trace("\n baseUrl = %s",baseUrl.c_str());
    //Util::app42Trace("\n addScoreBody = %s",addScoreBody.c_str());
    
    std::vector<std::string> headers;
    string timestamp = Util::getTimeStamp();
    Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);
    
    App42GameResponse *response = new App42GameResponse::App42GameResponse(pTarget,pSelector);
    Util::executePost(baseUrl, headers, addScoreBody.c_str(), response, callfuncND_selector(App42GameResponse::onComplete));
}



void ScoreService::DeductScore(string gameName, string userName, double score, CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector)
{
    map<string, string> postMap;
    populateSignParams(postMap);
    string deductScoreBody = BuildScoreBody(gameName, userName,score);
    postMap["body"] = deductScoreBody;
    
    string signature = Util::signMap(secretKey, postMap);
    
    string baseUrl = getBaseUrl("game/score/deduct");
    baseUrl.append("?");
   // Util::app42Trace("\n baseUrl = %s",baseUrl.c_str());
   // Util::app42Trace("\n addScoreBody = %s",deductScoreBody.c_str());
    
    std::vector<std::string> headers;
    string timestamp = Util::getTimeStamp();
    Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);
    
    App42GameResponse *response = new App42GameResponse::App42GameResponse(pTarget,pSelector);
    Util::executePost(baseUrl, headers, deductScoreBody.c_str(), response, callfuncND_selector(App42GameResponse::onComplete));
}



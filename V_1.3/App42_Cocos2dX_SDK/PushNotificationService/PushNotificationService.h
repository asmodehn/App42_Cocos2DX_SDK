//
//  PushNotificationService.h
//  App42CPPSDK
//
//  Created by Rajeev Ranjan on 21/10/13.
//
//

#ifndef __App42CPPSDK__PushNotificationService__
#define __App42CPPSDK__PushNotificationService__

#include <iostream>
#include "App42Service.h"

using namespace std;

typedef enum DeviceType
{
    DT_IOS,
    DT_ANDROID,
    DT_WP7,
    DT_NOKIAX,
}DeviceType;

class PushNotificationService : public App42Service
{
    
private:
    PushNotificationService();
	static PushNotificationService* _instance;
    
    inline const char *getDeviceType(DeviceType deviceType)
    {
        static const char *strings[] = {"iOS", "ANDROID", "WP7","NokiaX"};
        
        return strings[deviceType];
    }
    
public:
    
    
    
	/**
     * This is a constructor that takes
     *
     * @param apiKey
     * @param secretKey
     * @param server
     *
     */
    static PushNotificationService* Initialize(string apikey, string secretkey);
    
	static void Terminate();
    
	/**
     * Builds the instance of PushNotificationService.
     *
     * @return PushNotificationService - PushNotificationService Object
     */
	static PushNotificationService* getInstance();
    
    
    void CreateChannel(const char* channel,const char* description, SEL_App42CallFuncND pSelector);
    
    void RegisterDeviceToken(const char* deviceToken, const char* userName, DeviceType deviceType,  SEL_App42CallFuncND pSelector);
    
    void SendPushMessageToUser(const char* username,  const char* message,  SEL_App42CallFuncND pSelector);
    
    void SendPushMessageToUser(const char* userName, map<string, string>messageMap, SEL_App42CallFuncND pSelector);
    
    void SendPushMessageToAll(const char* message, SEL_App42CallFuncND pSelector);
    
    void SubscribeToChannel(const char* channel, const char* userName, SEL_App42CallFuncND pSelector);
    
    void SubscribeToChannel(const char* userName,const char* channelName, const char* deviceToken, DeviceType deviceType, SEL_App42CallFuncND pSelector);
    
    void UnsubscribeFromChannel(const char* channel, const char* userName, SEL_App42CallFuncND pSelector);
    
    void UnsubscribeDeviceToChannel(const char* userName,const char* channelName, const char* deviceToken,  SEL_App42CallFuncND pSelector);
    
    void SendPushMessageToChannel(const char* channel, const char* message, SEL_App42CallFuncND pSelector);
    
    void SendPushMessageToChannel(const char* channel, map<string, string>messageMap, SEL_App42CallFuncND pSelector);

    void DeleteDeviceToken(const char* deviceToken, const char* userName, SEL_App42CallFuncND pSelector);
    void DeleteAllDevices(const char* userName, SEL_App42CallFuncND pSelector);
    
    void UnsubscribeDevice(const char* deviceToken, const char* userName, SEL_App42CallFuncND pSelector);
    void ResubscribeDevice(const char* deviceToken, const char* userName, SEL_App42CallFuncND pSelector);
    
    void SendPushMessageToAllByType(const char* message,DeviceType deviceType, SEL_App42CallFuncND pSelector);
    
    /**
     * Send push message to list of specific users.
     *
     * @param message
     *            - Message which you have to send.
     * @param userList
     *            - List of the users for which message has to be send.
     * @return PushNotification Object
     * @throws App42Exception
     */
    void SendPushMessageToGroup(vector<string> userList,const char* message, SEL_App42CallFuncND pSelector);
    
    void SendPushToTargetUsers(const char* message,const char* dbName, const char* collectionName, Query *query, SEL_App42CallFuncND pSelector);
    
    void ScheduleMessageToUser(const char* username,const char* message, tm *expiryDate, SEL_App42CallFuncND pSelector);
    
};
#endif /* defined(__App42CPPSDK__PushNotificationService__) */

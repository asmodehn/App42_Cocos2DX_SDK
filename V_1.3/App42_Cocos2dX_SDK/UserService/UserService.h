//
//  UserService.h
//  App42CPPSDK
//
//  Created by Rajeev Ranjan on 08/10/13.
//
//

#ifndef __App42CPPSDK__UserService__
#define __App42CPPSDK__UserService__

#include <iostream>
#include "App42Service.h"
#include "App42Constants.h"
#include "App42UserResponse.h"

using namespace std;


class UserService : public App42Service
{
    
private:
    UserService();
    
    string getJsonStringFromVector(vector<string>roles);
    string buildCreateUserBody(const char *username, const char *password, const char *email,const char *roleList);
    string buildChangePasswordBody(string username, string oldPwd, string newPwd);
    string buildCreateOrUpdateProfileBody(App42User *user);
    string buildProfileData(App42UserProfile *profile);
    string buildLogoutBody(string sessionId);
	static UserService* _instance;
public:
    
    
	/**
     * This is a constructor that takes
     *
     * @param apiKey
     * @param secretKey
     * @param server
     *
     */
	
    static UserService* Initialize(string apikey, string secretkey);

	static void Terminate();
    
	/**
     * Builds the instance of UserService.
     *
     * @return UserService - UserService Object
     */
	static UserService* getInstance();
    
	/**
     * Create a User with userName, password & emailAddress
     *
     * @param userName - Name of the User
     * @param password - Password for the User
     * @param email - Email address of the user
     * @return app42Result - The result of the request
     *
     */
	void CreateUser(const char *username, const char *password, const char *email, SEL_App42CallFuncND pSelector);
    
    /**
     * Create a User with userName, password & emailAddress and assigns the
     * roles to the created User
     *
     * @param uName
     *            - Name of the User
     * @param pwd
     *            - Password for the User
     * @param emailAddress
     *            - Email address of the user
     * @param roleList
     *            - List of roles to be assigned to User
     *
     * @return
     * 
     */
    void CreateUser(const char *username, const char *password, const char *email, vector<string>roleList, SEL_App42CallFuncND pSelector);

	/**
     * Authenticate user based on userName and password
     *
     * @param userName  - UserName which should be unique for the App
     * @param password - Password for the User
     * @return app42Result - The result of the request
     */
	void Authenticate(const char *userName, const char *password, SEL_App42CallFuncND pSelector);
    
	/**
     * Gets the User details based on userName
     *
     * @param userName - Name of the User to retrieve.
     * @return app42Result - The result of the request.
     */
	void GetUser(const char *username, SEL_App42CallFuncND pSelector);
    /**
     * Gets the User details based on emailId
     *
     * @param emailId - EmailId of the User to retrieve.
     * @return app42Result - The result of the request.
     */
    void GetUserByEmailId(const char *emailId, SEL_App42CallFuncND pSelector);
	/**
     * Gets the details of all users
     * 
     * @return app42Result - The result of the request.
     */
	void GetAllUsers( SEL_App42CallFuncND pSelector);
    /**
     * Gets Total user count
     *
     * @return app42Result - The result of the request.
     */
	void GetAllUsersCount( SEL_App42CallFuncND pSelector);
    
    /**
     * Gets all users by Paging
     *
     * @param max
     *            - Maximum number of records to be fetched
     * @param offset
     *            - From where the records are to be fetched
     *
     * @return the List that contains all User Object
     * 
     */
    void GetAllUsers(int max, int offset, SEL_App42CallFuncND pSelector);
    
    /**
     * Locks the user based on the userName. Apps can use these feature to lock
     * a user because of reasons specific to their usercase e.g. If payment not
     * received and the App wants the user to be inactive
     *
     * @param username
     *            - UserName which should be unique for the App
     *
     * @returns the locked User Object
     * 
     */
    void LockUser(const char *username, SEL_App42CallFuncND pSelector);
    /**
     * Unlocks the user based on the userName. App developers can use this
     * feature to unlock a user because of reasons specific to their usercase
     * e.g. When payment received, the App developer wants the user to be
     * active.
     *
     * @param username
     *            - UserName which should be unique for the App
     *
     * @returns the unlocked User Object
     * 
     */
    void UnlockUser(const char *username, SEL_App42CallFuncND pSelector);
    /**
     * Gets the details of all the locked users
     *
     * @return the list containing locked User Objects
     * 
     */
    void GetLockedUsers( SEL_App42CallFuncND pSelector);
    /**
     * Gets the details of all the locked users By paging.
     *
     * @param max
     *            - Maximum number of records to be fetched
     * @param offset
     *            - From where the records are to be fetched
     *
     * @return the List containing locked User Objects
     * 
     */
    void GetLockedUsers(int max, int offset, SEL_App42CallFuncND pSelector);
    /**
     * Gets Total user count
     *
     * @return app42Result - The result of the request.
     */
    void GetLockedUsersCount( SEL_App42CallFuncND pSelector);
    /**
     * Updates the User's Email Address based on userName. Note: Only email can
     * be updated. Username cannot be updated.
     *
     * @param username
     *            - UserName which should be unique for the App
     * @param emailAddress
     *            - Email address of the user
     *
     * @returns updated User Object
     * 
     */
    void UpdateEmail(const char *username,const char *emailAddress, SEL_App42CallFuncND pSelector);
    /**
     * Deletes a particular user based on userName.
     *
     * @param userName
     *            - UserName which should be unique for the App
     *
     * @returns App42Response Object if user deleted successfully
     * 
     */
    void DeleteUser(const char *username, SEL_App42CallFuncND pSelector);
    /**
     * Changes the password for user based on the userName.
     *
     * @param username
     *            - UserName which should be unique for the App
     * @param oldPassword
     *            - Old Password for the user for authentication
     * @param newPassword
     *            - New Password for the user to change
     *
     * @returns App42Response Object if updated successfully
     * 
     */
    void ChangeUserPassword(const char *username,const char *oldPassword,const char *newPassword, SEL_App42CallFuncND pSelector);
    /**
     * Updates the User password based on userName. Username cannot be updated.
     *
     * @param uName
     *            - UserName which should be unique for the App
     *
     * @returns App42Response Object
     *
     */
    void ResetUserPassword(const char *username, SEL_App42CallFuncND pSelector);
    /**
     * Creates or Updates User Profile. First time the Profile for the user is
     * created and in future calls user profile will be updated. This will
     * always update the profile with new value passed in profile object. Call
     * to this method should have all the values you want to retain in user
     * profile object, otherwise old values of profile will get updated with
     * null. Method only updates the profile of user, passing email/password in
     * user object does not have any significance for this method call.
     *
     * @param user
     *            - User for which profile has to be updated, this should
     *            contain the userName and profile object in it.
     *
     * @returns User Object with updated Profile information
     * 
     * @see Profile
     */
    void createOrUpdateProfile(App42User *user, SEL_App42CallFuncND pSelector);
    
    /**
     * Gets the list of Users based on Profile Data
     *
     * @param profileData
     *            - Profile Data key/value for which Users need to be retrieved
     *
     * @return List of User Object for the specified profile data
     *
     */
    void GetUsersByProfileData(App42UserProfile *profileData, SEL_App42CallFuncND pSelector);
    
    /**
     * Logout from current session
     *
     * @param sessionId
     *            - sessionId
     * @throws App42Exception
     */

    void Logout(const char *sessionId, SEL_App42CallFuncND pSelector);
    
    void GetRolesByUser(const char *userName, SEL_App42CallFuncND pSelector);
    
    void AssignRoles(const char *uName, vector<string> roleList, SEL_App42CallFuncND pSelector);

};
#endif /* defined(__App42CPPSDK__UserService__) */

//
//  UploadService.h
//  App42Cocos2dX3.0Sample
//
//  Created by Rajeev Ranjan on 04/04/14.
//
//

#ifndef __App42Cocos2dX3_0Sample__UploadService__
#define __App42Cocos2dX3_0Sample__UploadService__

#include <iostream>
#include "App42Service.h"

using namespace std;
namespace App42
{
	typedef enum FileType
	{
		AUDIO,
		VIDEO,
		IMAGE,
		BINARY,
		TXT,
		XML,
		CSV,
		JSON,
		OTHER
	}FileType;

	class UploadService : public App42Service
	{

	private:
		UploadService();
		static UploadService* _instance;

		inline const char *getUploadFileType(FileType fileType)
		{
			const char *strings[] = { "AUDIO", "VIDEO", "IMAGE", "BINARY", "TXT", "XML", "CSV", "JSON", "OTHER" };

			return strings[fileType];
		}

		string BuildGrantAccessBody(string json);

	public:

		/**
		 * This is a constructor that takes
		 *
		 * @param apiKey
		 * @param secretKey
		 * @param server
		 *
		 */
		static UploadService* Initialize(string apikey, string secretkey);

		static void Terminate();

		/**
		 * Builds the instance of UserService.
		 *
		 * @return UserService - UserService Object
		 */
		static UploadService* getInstance();

		void UploadFile(const char *fileName, const char * filePath, FileType fileType, const char * description,  SEL_App42CallFuncND pSelector);

		void UploadFile(const char * fileName, unsigned char* fileData, int fileDataSize, FileType fileType, const char * description,  SEL_App42CallFuncND pSelector);

		void UploadFileForUser(const char * fileName, const char * userName, const char * filePath, FileType fileType, const char * description,  SEL_App42CallFuncND pSelector);

		void UploadFileForUser(const char * fileName, const char * userName, unsigned char* fileData, int fileDataSize, FileType fileType, const char * description,  SEL_App42CallFuncND pSelector);

		void GetAllFiles( SEL_App42CallFuncND pSelector);

		void GetAllFilesCount( SEL_App42CallFuncND pSelector);

		void GetAllFiles(int max, int offset,  SEL_App42CallFuncND pSelector);

		void GetFileByUser(const char * fileName, const char * userName,  SEL_App42CallFuncND pSelector);

		void GetAllFilesByUser(const char * userName,  SEL_App42CallFuncND pSelector);

		void GetAllFilesByUser(const char * userName, int max, int offset,  SEL_App42CallFuncND pSelector);

		void GetAllFilesCountByUser(const char * userName,  SEL_App42CallFuncND pSelector);

		void GetFileByName(const char * fileName,  SEL_App42CallFuncND pSelector);

		void RemoveFileByUser(const char * fileName, const char * userName,  SEL_App42CallFuncND pSelector);

		void RemoveAllFilesByUser(const char * userName,  SEL_App42CallFuncND pSelector);

		void RemoveFileByName(const char * fileName,  SEL_App42CallFuncND pSelector);

		void RemoveAllFiles( SEL_App42CallFuncND pSelector);

		void GetFilesCountByType(FileType fileType,  SEL_App42CallFuncND pSelector);

		void GetFilesByType(FileType fileType,  SEL_App42CallFuncND pSelector);

		void GetFilesByType(FileType fileType, int max, int offset,  SEL_App42CallFuncND pSelector);

		/**
		 *
		 * @param fileName
		 * @param userName
		 * @param aclList
		 * @return void
		 * @throws App42Exception
		 */
		void GrantAccess(const char* fileName, const char* userName,
			vector<ACL> aclList,  SEL_App42CallFuncND pSelector);

		/**
		 *
		 * @param fileName
		 * @param userName
		 * @param aclList
		 * @return void
		 * @throws App42Exception
		 */
		void RevokeAccess(const char* fileName, const char* userName,
			vector<ACL> aclList,  SEL_App42CallFuncND pSelector);
	};
}
#endif /* defined(__App42Cocos2dX3_0Sample__UploadService__) */
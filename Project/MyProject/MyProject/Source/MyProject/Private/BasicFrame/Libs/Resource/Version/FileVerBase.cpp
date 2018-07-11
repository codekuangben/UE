﻿#include "MyProject.h"
#include "FileVerBase.h"
#include "FileVerInfo.h"
#include "AddOnceAndCallOnceEventDispatch.h"
#include "UtilStr.h"
#include "MacroDef.h"

MY_BEGIN_NAMESPACE(MyNS)

FileVerBase::FileVerBase()
{
	this->mIsMiniLoadSuccess = false;
	this->mIsVerLoadSuccess = false;

	this->mCurVer = "";           // 当前版本，当前日期，例如 201606091136-201606091136-0(主版本-次版本-补丁版本)
	this->mFileVerInfo = MY_NEW FileVerInfo();

	this->mMiniLoadedDispatch = MY_NEW AddOnceAndCallOnceEventDispatch();
	this->mLoadedDispatch = MY_NEW AddOnceAndCallOnceEventDispatch();

	this->mMajorVersion = "0";    // 主版本
	this->mMinorVersion = "0";    // 次版本
	this->mPatchVersion = "0";    // 补丁版本
	this->mTimeStamp = "0";       // 时间戳
}

std::string FileVerBase::getDetailVersionString()
{
	std::string ret = "";

	ret = UtilStr::Format("{0}-{1}-{2}-{3}", this->mMajorVersion, this->mMinorVersion, this->mPatchVersion, this->mTimeStamp);

	return ret;
}

std::string FileVerBase::getDotVersionString()
{
	std::string ret = "";

	ret = UtilStr::Format("{0}.{1}.{2}", this->mMajorVersion, this->mMinorVersion, this->mPatchVersion);

	return ret;
}

std::string FileVerBase::getVersionString()
{
	std::string ret = "";

	ret = UtilStr::Format("{0}-{1}-{2}", this->mMajorVersion, this->mMinorVersion, this->mPatchVersion);

	return ret;
}

void FileVerBase::saveMiniVerToPersistentPath()
{
	if (MacroDef::ENABLE_LOG)
	{
		GLogSys->log("FileVerBase::saveMiniVerToPersistentPath, start", LogTypeId::eLogAutoUpdate);
	}

	if (!this->mFileVerInfo.isNoVerInfo())
	{
		if (MacroDef::ENABLE_LOG)
		{
			GLogSys->log("FileVerBase::saveMiniVerToPersistentPath, isNoVerInfo is false", LogTypeId::eLogAutoUpdate);
		}

		std::string path = UtilFileIO::combine(MFileSys::msPersistentDataPath, VerFileName::VER_MINI);

		if (UtilFileIO::existFile(path))
		{
			UtilFileIO::deleteFile(path);
		}

		MDataStream dataStream = new MDataStream(path, nullptr, MFileMode::eCreateNew, MFileAccess.eWrite);
		dataStream.open();

		std::string line = string.Format("Version={0}", this->mCurVer);
		dataStream.writeLine(line);

		line = string.Format("{0}={1}={2}={3}={4}", this->mFileVerInfo.mOrigPath, this->mFileVerInfo.mResUniqueId, this->mFileVerInfo.mLoadPath, this->mFileVerInfo.mFileMd5, this->mFileVerInfo.mFileSize);
		dataStream.writeLine(line);

		dataStream.dispose();
		dataStream = nullptr;
	}
}

void FileVerBase::parseMiniFile(string text)
{
	std::string[] lineSplitStr = { UtilEngineWrap.CR_LF };
	std::string[] equalSplitStr = { UtilEngineWrap.SEPARATOR };
	std::string[] lineList = text.Split(lineSplitStr, StringSplitOptions.RemoveEmptyEntries);
	int lineIdx = 0;
	std::string[] equalList = nullptr;

	// 第一行是版本号
	lineIdx = 0;

	if (lineList.Length > 0)
	{
		UtilStr.removeLastCR(ref lineList[lineIdx]);
		equalList = lineList[lineIdx].Split(equalSplitStr, StringSplitOptions.RemoveEmptyEntries);

		if (equalList.Length >= 2)
		{
			this->mCurVer = equalList[1];

			string[] verSplitStr = { "&" };
			string[] verStrList = this->mCurVer.Split(verSplitStr, StringSplitOptions.RemoveEmptyEntries);

			if (verStrList.Length == 4)
			{
				this->mMajorVersion = verStrList[0];
				this->mMinorVersion = verStrList[1];
				this->mPatchVersion = verStrList[2];
				this->mTimeStamp = verStrList[3];
			}
		}
		else
		{
			if (MacroDef.ENABLE_ERROR)
			{
				GLogSys->error(string.Format("FileVerBase::parseMiniFile, curversion error, content = {0}", text), LogTypeId::eErrorDownload);
			}
		}
	}
	else
	{
		if (MacroDef.ENABLE_ERROR)
		{
			GLogSys->error(string.Format("FileVerBase::parseMiniFile, LessEqual 0 error, content = {0}", text), LogTypeId::eErrorDownload);
		}
	}

	// 第二行是版本的版本
	lineIdx = 1;

	if (lineList.Length > 1)
	{
		UtilStr.removeLastCR(ref lineList[lineIdx]);

		equalList = lineList[lineIdx].Split(equalSplitStr, StringSplitOptions.RemoveEmptyEntries);

		if (equalList.Length >= 5)
		{
			this->mFileVerInfo.mOrigPath = equalList[0];
			this->mFileVerInfo.mResUniqueId = equalList[1];
			this->mFileVerInfo.mLoadPath = equalList[2];
			this->mFileVerInfo.mFileMd5 = equalList[3];
			this->mFileVerInfo.mFileSize = Int32.Parse(equalList[4]);
		}
		else
		{
			if (MacroDef.ENABLE_ERROR)
			{
				GLogSys->error(string.Format("FileVerBase::parseMiniFile, version info error, content = {0}", text), LogTypeId::eErrorDownload);
			}
		}
	}
	else
	{
		if (MacroDef.ENABLE_ERROR)
		{
			GLogSys->error(string.Format("FileVerBase::parseMiniFile, less equal 1 error, content = {0}", text), LogTypeId::eErrorDownload);
		}
	}
}

// 这个主要是解析版本文件的
void FileVerBase::_loadFormText(string text, MDictionary<string, FileVerInfo> dic, MDictionary<string, FileVerInfo> abDic)
{
	string[] lineSplitStr = { UtilEngineWrap.CR_LF };
	string[] equalSplitStr = { UtilEngineWrap.SEPARATOR };
	string[] lineList = text.Split(lineSplitStr, StringSplitOptions.RemoveEmptyEntries);

	int lineIdx = 0;
	string[] equalList = nullptr;
	FileVerInfo fileInfo = nullptr;

	while (lineIdx < lineList.Length)
	{
		UtilStr.removeLastCR(ref lineList[lineIdx]);

		equalList = lineList[lineIdx].Split(equalSplitStr, StringSplitOptions.RemoveEmptyEntries);

		if (equalList.Length == 5)
		{
			fileInfo = new FileVerInfo();

			fileInfo.mOrigPath = equalList[0];
			fileInfo.mResUniqueId = equalList[1];
			fileInfo.mLoadPath = equalList[2];
			fileInfo.mFileMd5 = equalList[3];
			fileInfo.mFileSize = Int32.Parse(equalList[4]);

			//dic[fileInfo.mResUniqueId] = fileInfo;
			dic[fileInfo.mOrigPath] = fileInfo;

			if (nullptr != abDic && !abDic.containsKey(fileInfo.mLoadPath))
			{
				abDic[fileInfo.mLoadPath] = fileInfo;
			}
		}
		else
		{
			if (MacroDef.ENABLE_ERROR)
			{
				GLogSys->error(string.Format("FileVerBase::loadFormText, version info error, content = {0}", lineList[lineIdx]), LogTypeId::eErrorDownload);
			}
		}

		++lineIdx;
	}

	int index = 0;
	int listLen = 0;

	if (nullptr != lineSplitStr)
	{
		index = 0;
		listLen = lineSplitStr.Length;

		while (index < listLen)
		{
			lineSplitStr[index] = nullptr;
			index += 1;
		}

		lineSplitStr = nullptr;
	}

	if (nullptr != equalSplitStr)
	{
		index = 0;
		listLen = equalSplitStr.Length;

		while (index < listLen)
		{
			equalSplitStr[index] = nullptr;
			index += 1;
		}

		equalSplitStr = nullptr;
	}

	if (nullptr != lineList)
	{
		index = 0;
		listLen = lineList.Length;

		while (index < listLen)
		{
			lineList[index] = nullptr;
			index += 1;
		}

		lineList = nullptr;
	}

	if (nullptr != equalList)
	{
		index = 0;
		listLen = equalList.Length;

		while (index < listLen)
		{
			equalList[index] = nullptr;
			index += 1;
		}

		equalList = nullptr;
	}

	fileInfo = nullptr;
}

MY_END_NAMESPACE
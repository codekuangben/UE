﻿using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using SDK.Lib;

namespace ToolSet
{
	/**
     * @ref UE4资源热更新 
     * @url http://blog.csdn.net/liulong1567/article/details/71597892
	 * @url https://blog.csdn.net/or_7r_ccl/article/details/53053337
     */
	public class TestBuildOnePak : TestBase
	{
        protected string mUE4EngineRootPath;    // UE4 Engine 根目录
        protected string mProjectRootPath;      // Project 根目录
        protected string mPakOutPath;
        protected bool mIsBuildPakSuccess;
        protected List<string> mBuildPakFileList;   // 打包文件列表
		protected bool mIsEncrypt;
		protected bool mIsCompress;
		protected bool mIsExportDependencies;
		protected bool mIsEnableAbslog;
		protected bool mIsEnableOrder;
		protected bool mIsWindowsPlatform;
		protected bool mIsTest;
		protected bool mIsList;
		protected bool mIsExtract;
		protected string mPakOtherParams;

		public TestBuildOnePak()
        {
            this.mIsBuildPakSuccess = false;
            this.mBuildPakFileList = new List<string>();
			this.mIsEncrypt = false;
			this.mIsCompress = false;
			this.mIsExportDependencies = true;
			this.mIsEnableAbslog = true;
			this.mIsEnableOrder = false;
			this.mIsWindowsPlatform = true;
			this.mIsTest = false;
			this.mIsList = false;
			this.mIsExtract = false;

			this.mPakOtherParams = " -UTF8Output";
		}

        public override void init()
        {
			base.init();

			this.mUE4EngineRootPath = CtxExt.msExtInstance.mProjectConfig.getEngineRootPath();
			this.mProjectRootPath = CtxExt.msExtInstance.mProjectConfig.getProjectRootPath();
			this.mPakOutPath = CtxExt.msExtInstance.mProjectConfig.getPakOutPath();
			this._initCommandLineParams();

			this.addPakFile("BaseMaterial.uasset");
            this.addPakFile("GrayMaterial.uasset");
            this.addPakFile("UFO.uasset");
        }

        public override void dispose()
        {
			base.dispose();
        }

		public override void run()
		{
			base.run();

			this.build();
		}

		protected void _initCommandLineParams()
		{
			string logFile = string.Format("{0}/Pak.log", this.mPakOutPath);
			string exportDependenciesFile = string.Format("{0}/DependenciesFile.txt", this.mPakOutPath);
			string orderFile = string.Format("{0}/Order.txt", this.mPakOutPath);
			string extractPath = string.Format("{0}/Extract", this.mPakOutPath);

			if (this.mIsEncrypt)
			{
				this.mPakOtherParams = string.Format("{0} {1}", this.mPakOtherParams, "-encrypt");
			}
			if (this.mIsCompress)
			{
				this.mPakOtherParams = string.Format("{0} {1}", this.mPakOtherParams, "-compress");
			}
			if (this.mIsExportDependencies)
			{
				this.mPakOtherParams = string.Format("{0} -ExportDependencies={1} -NoAssetRegistryCache -ForceDependsGathering", this.mPakOtherParams, exportDependenciesFile);
			}
			if (this.mIsEnableAbslog)
			{
				this.mPakOtherParams = string.Format("{0} -abslog={1}", this.mPakOtherParams, logFile);
			}
			if (this.mIsEnableOrder)
			{
				this.mPakOtherParams = string.Format("{0} -order={1}", this.mPakOtherParams, orderFile);
			}
			if (this.mIsWindowsPlatform)
			{
				this.mPakOtherParams = string.Format("{0} -platform={1}", this.mPakOtherParams, "Windows");
			}
			if (this.mIsTest)
			{
				this.mPakOtherParams = string.Format("{0} -Test", this.mPakOtherParams);
			}
			if (this.mIsList)
			{
				this.mPakOtherParams = string.Format("{0} -List={1}", this.mPakOtherParams, "Windows");
			}
			if (this.mIsExtract)
			{
				if (UtilFileIO.existDirectory(extractPath))
				{
					UtilFileIO.createDirectory(extractPath, true);
				}

				this.mPakOtherParams = string.Format("{0} -Extract {1}", this.mPakOtherParams, extractPath);
			}
		}

		public string getUE4EngineRootPath()
        {
            return this.mUE4EngineRootPath;
        }

        public void setUE4EngineRootPath(string value)
        {
            this.mUE4EngineRootPath = value;
        }

        public string getPakOutPath()
        {
            return this.mPakOutPath;
        }

        public void setPakOutPath(string value)
        {
            this.mPakOutPath = value;
        }

        public void addPakFile(string fileName)
        {
			string fullFileName = string.Format("{0}/{1}/{2}", this.mProjectRootPath, @"Content/MyAsset/MyFly/Flying/Meshes", fileName);
			this.mBuildPakFileList.Add(fullFileName);
        }

        public void build()
        {
			// 检查选中的引擎根目录,其目录下是否包含有[UnrealPak.exe]文件
			string unrealPakPath = string.Format("{0}/{1}", this.mUE4EngineRootPath, "Engine/Binaries/Win64/UnrealPak-Win64-Debug.exe");

			if (!File.Exists(unrealPakPath))
            {
                this.mIsBuildPakSuccess = false;
            }
            else
            {
                this._buildOneAssetToOnePak();
                this._buildMultiAssetToOnePak();
				this._buildOnePakFromConfig();

				System.Console.WriteLine("Pak Success");
			}
        }

        protected void _buildOneAssetToOnePak()
        {
            int index = 0;
            int listLen = this.mBuildPakFileList.Count;
            string assetFullName = "";
            string assetName = "";
            string outPath = "";

			while (index < listLen)
            {
                assetFullName = UtilFileIO.normalPath(this.mBuildPakFileList[index]);
                assetName = UtilFileIO.getFileNameNoExt(assetFullName);
				if (!UtilFileIO.existDirectory(this.mPakOutPath))
				{
					UtilFileIO.createDirectory(this.mPakOutPath, true);
				}
				outPath = string.Format("{0}/{1}.pak", this.mPakOutPath, assetName);

				//通过[Process]相关类来多次调用[UnrealPak.exe]程序来打包  
				using (Process process = new Process())
				{
					ProcessStartInfo processStartInfo = new ProcessStartInfo();
					processStartInfo.FileName = string.Format("{0}/{1}", this.mUE4EngineRootPath, "Engine/Binaries/Win64/UnrealPak-Win64-Debug.exe");
					processStartInfo.Arguments = string.Format("{0} {1}{2}", @outPath, @assetFullName, this.mPakOtherParams);
					processStartInfo.WindowStyle = ProcessWindowStyle.Minimized;
					processStartInfo.UseShellExecute = false;
					processStartInfo.RedirectStandardError = true;
					processStartInfo.RedirectStandardOutput = true;

					process.StartInfo = processStartInfo;
					process.Start();
					UtilSysLibWrap.writeConsoleFromProcess(process);
					process.WaitForExit();
				}
				
				index += 1;
            }
        }

		protected void _buildMultiAssetToOnePak()
        {
            int index = 0;
            int listLen = this.mBuildPakFileList.Count;
            string outPath = string.Format("{0}/{1}.pak", this.mPakOutPath, "MultiOne");
            string cmdParams = outPath;
            string assetFullName = "";

			if (!UtilFileIO.existDirectory(this.mPakOutPath))
			{
				UtilFileIO.createDirectory(this.mPakOutPath, true);
			}

			while (index < listLen)
            {
				assetFullName = UtilFileIO.normalPath(this.mBuildPakFileList[index]);
				cmdParams = string.Format("{0} {1}", cmdParams, @assetFullName);

                index += 1;
            }

            // 通过 [Process] 相关类来多次调用 [UnrealPak.exe] 程序来打包  
            ProcessStartInfo processStartInfo = new ProcessStartInfo();
			processStartInfo.FileName = string.Format("{0}/{1}", this.mUE4EngineRootPath, @"Engine/Binaries/Win64/UnrealPak-Win64-Debug.exe");
			processStartInfo.Arguments = string.Format("{0}{1}", cmdParams, this.mPakOtherParams);
			processStartInfo.WindowStyle = ProcessWindowStyle.Minimized;
			processStartInfo.UseShellExecute = false;
			processStartInfo.RedirectStandardError = true;
			processStartInfo.RedirectStandardOutput = true;

			Process process = Process.Start(processStartInfo);
			UtilSysLibWrap.writeConsoleFromProcess(process);
			process.WaitForExit();
		}

		protected void _buildOnePakFromConfig()
		{

		}

		protected void _unpackOnePak()
		{
			// https://forums.unrealengine.com/community/general-discussion/105459-unpacking-a-pak-file
			// D:\EpicGames\4.12\Engine\Binaries\Win64\UnrealPak.exe C:\Users\Vlad\Desktop\YourPak.pak C:\Users\Vlad\Desktop -test
		}
	}
}
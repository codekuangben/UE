﻿namespace SDK.Lib
{
    public class MProfileInfo
    {
        public string mName;
        public MDictionary<string, MProfileInfo> mChildren;
        public MProfileInfo mParent;

        public double mStartTime;       // 进入块开始时间
        public double mTotalTime;       // 当前块总共执行时间,包括子函数调用
        public double mSelfTime;        // 当前块自己执行时间,不包括子函数调用
        public int mActivations;        // 调用次数
        public double mMaxTime;         // 最大一次执行时间
        public double mMinTime;         // 最小一次执行时间

        public MProfileInfo(string name, MProfileInfo parent = null)
        {
            this.mChildren = new MDictionary<string, MProfileInfo>();
            this.mName = name;
            this.mParent = parent;
        }

        public void wipe()
        {
            if(MacroDef.ENABLE_LOG)
            {
                Ctx.msInstance.mLogSys.log(string.Format("MProfileInfo::wipe, name = {0}", mName), LogTypeId.eLogProfileDebug);
            }

            this.mStartTime = 0;
            this.mTotalTime = 0;
            this.mSelfTime = 0;
            this.mActivations = 0;
            // 如果清除后，从没有进入这个块，那么 maxTime 值就是 int.MinValue，minTime 值就是  int.MaxValue
            this.mMaxTime = int.MinValue;
            this.mMinTime = int.MaxValue;
        }
    }
}
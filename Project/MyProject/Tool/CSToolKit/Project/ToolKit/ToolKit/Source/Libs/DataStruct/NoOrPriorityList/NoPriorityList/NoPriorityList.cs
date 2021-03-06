﻿namespace SDK.Lib
{
    /**
     * @brief 优先级队列
     */
    public class NoPriorityList : GObject, INoOrPriorityList
    {
        protected MList<INoOrPriorityObject> mNoPriorityProcessObjectList;  // 优先级对象列表

        protected MDictionary<INoOrPriorityObject, int> mDic;       // 查找字典
        protected bool mIsSpeedUpFind;      // 是否开启查找

        public NoPriorityList()
        {
            this.mNoPriorityProcessObjectList = new MList<INoOrPriorityObject>();
            this.mIsSpeedUpFind = false;
        }

        override public void init()
        {

        }

        override public void dispose()
        {
            if(null != this.mNoPriorityProcessObjectList)
            {
                this.mNoPriorityProcessObjectList.dispose();
                this.mNoPriorityProcessObjectList = null;
            }
            if (null != this.mDic)
            {
                this.mDic.dispose();
                this.mDic = null;
            }
        }

        public void setIsSpeedUpFind(bool value)
        {
            this.mIsSpeedUpFind = value;

            if (this.mIsSpeedUpFind)
            {
                this.mDic = new MDictionary<INoOrPriorityObject, int>();
            }
        }

        public void setIsOpKeepSort(bool value)
        {

        }

        public void clear()
        {
            this.mNoPriorityProcessObjectList.clear();

            if(this.mIsSpeedUpFind)
            {
                this.mDic.clear();
            }
        }

        public int count()
        {
            return this.mNoPriorityProcessObjectList.count();
        }

        public INoOrPriorityObject get(int index)
        {
            INoOrPriorityObject ret = null;

            if(index < this.count())
            {
                ret = this.mNoPriorityProcessObjectList.get(index);
            }

            return ret;
        }

        public bool contains(INoOrPriorityObject item)
        {
            bool ret = false;

            if (null != item)
            {
                if (this.mIsSpeedUpFind)
                {
                    ret = this.mDic.containsKey(item);
                }
                else
                {
                    int index = 0;
                    int listLen = this.mNoPriorityProcessObjectList.count();

                    while (index < listLen)
                    {
                        if (item == this.mNoPriorityProcessObjectList.get(index))
                        {
                            ret = true;
                            break;
                        }

                        ++index;
                    }
                }
            }
            else
            {
                if (MacroDef.ENABLE_LOG)
                {
                    Ctx.msInstance.mLogSys.log("NoPriorityList::Contains, failed", LogTypeId.eLogNoPriorityListCheck);
                }
            }

            return ret;
        }

        public void removeAt(int index)
        {
            if (this.mIsSpeedUpFind)
            {
                this.effectiveRemove(this.mNoPriorityProcessObjectList[index]);
            }
            else
            {
                this.mNoPriorityProcessObjectList.removeAt(index);
            }
        }

        public int getIndexByNoPriorityObject(INoOrPriorityObject priorityObject)
        {
            int retIndex = -1;

            int index = 0;
            int listLen = this.mNoPriorityProcessObjectList.count();

            while (index < listLen)
            {
                if (this.mNoPriorityProcessObjectList.get(index) == priorityObject)
                {
                    retIndex = index;
                    break;
                }

                ++index;
            }

            return retIndex;
        }

        public int getIndexByNoOrPriorityObject(INoOrPriorityObject priorityObject)
        {
            return this.getIndexByNoPriorityObject(priorityObject);
        }

        public void addNoPriorityObject(INoOrPriorityObject noPriorityObject)
        {
            if (null != noPriorityObject)
            {
                if (!this.contains(noPriorityObject))
                {
                    this.mNoPriorityProcessObjectList.add(noPriorityObject);

                    if (this.mIsSpeedUpFind)
                    {
                        this.mDic.add(noPriorityObject, this.mNoPriorityProcessObjectList.count() - 1);
                    }
                }
            }
            else
            {
                if (MacroDef.ENABLE_LOG)
                {
                    Ctx.msInstance.mLogSys.log("NoPriorityList::addNoPriorityObject, failed", LogTypeId.eLogNoPriorityListCheck);
                }
            }
        }

        public void removeNoPriorityObject(INoOrPriorityObject noPriorityObject)
        {
            if (null != noPriorityObject)
            {
                if (this.contains(noPriorityObject))
                {
                    if (this.mIsSpeedUpFind)
                    {
                        this.effectiveRemove(noPriorityObject);
                    }
                    else
                    {
                        int index = this.getIndexByNoPriorityObject(noPriorityObject);

                        if (-1 != index)
                        {
                            this.mNoPriorityProcessObjectList.removeAt(index);
                        }
                    }
                }
            }
            else
            {
                if (MacroDef.ENABLE_LOG)
                {
                    Ctx.msInstance.mLogSys.log("NoPriorityList::addNoPriorityObject, failed", LogTypeId.eLogNoPriorityListCheck);
                }
            }
        }

        public void addNoOrPriorityObject(INoOrPriorityObject noPriorityObject, float priority = 0.0f)
        {
            this.addNoPriorityObject(noPriorityObject);
        }

        public void removeNoOrPriorityObject(INoOrPriorityObject noPriorityObject)
        {
            this.removeNoPriorityObject(noPriorityObject);
        }

        // 快速移除元素
        protected bool effectiveRemove(INoOrPriorityObject item)
        {
            bool ret = false;

            if (this.mDic.containsKey(item))
            {
                ret = true;

                int idx = this.mDic[item];
                this.mDic.remove(item);

                if (idx == this.mNoPriorityProcessObjectList.count() - 1)    // 如果是最后一个元素，直接移除
                {
                    this.mNoPriorityProcessObjectList.removeAt(idx);
                }
                else
                {
                    this.mNoPriorityProcessObjectList.set(idx, this.mNoPriorityProcessObjectList.get(this.mNoPriorityProcessObjectList.count() - 1));
                    this.mNoPriorityProcessObjectList.removeAt(this.mNoPriorityProcessObjectList.count() - 1);
                    this.mDic.add(this.mNoPriorityProcessObjectList.get(idx), idx);
                }
            }

            return ret;
        }

        protected void updateIndex(int idx)
        {
            int listLen = this.mNoPriorityProcessObjectList.count();

            while (idx < listLen)
            {
                this.mDic.add(this.mNoPriorityProcessObjectList.get(idx), idx);

                ++idx;
            }
        }
    }
}
﻿namespace SDK.Lib
{
    /**
     * @brief 动态增长的缓冲区，不是环形的，从 0 开始增长的
     */
    public class MDynBuffer<T> : GObject
    {
        public uint mCapacity;         // 分配的内存空间大小，单位大小是字节
        public uint mMaxCapacity;      // 最大允许分配的存储空间大小 
        public uint mSize;              // 存储在当前缓冲区中的数量
        public T[] mBuffer;            // 当前环形缓冲区

        public MDynBuffer(uint initCapacity = 1 * 1024/*DataCV.INIT_CAPACITY*/, uint maxCapacity = 8 * 1024 * 1024/*DataCV.MAX_CAPACITY*/)      // mono 模板类中使用常亮报错， vs 可以
        {
            this.mMaxCapacity = maxCapacity;
            this.mCapacity = initCapacity;
            this.mSize = 0;
            this.mBuffer = new T[this.mCapacity];
        }

        public T[] getBuffer()
        {
            return this.mBuffer;
        }

        public void setBuffer(T[] value)
        {
            this.mBuffer = value;
            this.mCapacity = (uint)this.mBuffer.Length;
        }

        public uint getMaxCapacity()
        {
            return this.mMaxCapacity;
        }

        public void setMaxCapacity(uint value)
        {
            this.mMaxCapacity = value;
        }

        public uint getCapacity()
        {
            return mCapacity;
        }

        public void setCapacity(uint value)
        {
            if (value == mCapacity)
            {
                return;
            }

            if (value < this.getSize())       // 不能分配比当前已经占有的空间还小的空间
            {
                return;
            }

            T[] tmpbuff = new T[value];   // 分配新的空间
            MArray.Copy(this.mBuffer, 0, tmpbuff, 0, (int)this.mSize);  // 这个地方是 mSize 还是应该是 mCapacity，如果是 CircleBuffer 好像应该是 mCapacity，如果是 MByteBuffer ，好像应该是 mCapacity。但是 DynBuffer 只有 MByteBuffer 才会使用这个函数，因此使用 mSize 就行了，但是如果使用 mCapacity 也没有问题

            this.mBuffer = tmpbuff;
            this.mCapacity = value;
        }

        public uint getSize()
        {
            return this.mSize;
        }

        public void setSize(uint value)
        {
            if (value > this.getCapacity())
            {
                this.extendDeltaCapicity(value - this.getSize());
            }

            this.mSize = value;
        }

        public void extendDeltaCapicity(uint delta)
        {
            this.setCapacity(DynBufResizePolicy.getCloseSize(this.getSize() + delta, this.getCapacity(), this.getMaxCapacity()));
        }
    }
}
﻿using System.Collections.Generic;

namespace SDK.Lib
{
    public class MQueue<T> : GObject
    {
        protected Queue<T> mQueue;

        public MQueue()
        {
            this.mQueue = new Queue<T>();
        }

        public void Clear()
        {
            this.mQueue.Clear();
        }

        public int Count()
        {
            return this.mQueue.Count;
        }

        public T Dequeue()
        {
            return this.mQueue.Dequeue();
        }

        public void Enqueue(T item)
        {
            this.mQueue.Enqueue(item);
        }

        public T Peek()
        {
            return this.mQueue.Peek();
        }
    }
}

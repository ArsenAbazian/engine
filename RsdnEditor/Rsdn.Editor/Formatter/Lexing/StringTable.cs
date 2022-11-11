using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;

namespace Rsdn.Editor.Lexing
{
	public sealed class StringTable
	{
		private const int BUCKET_SIZE = 3;
		private readonly List<Bucket> buckets = new List<Bucket>();
		private Bucket lastBucket;
		private Bucket lbBucket;
		private int lastOffset = -1;

		class Bucket
		{
			public readonly char Char;
			public bool BucketValue;
			public readonly List<Bucket> Buckets = new List<Bucket>();
			public readonly List<char[]> Strings = new List<char[]>();
			public readonly List<Int32> Styles = new List<Int32>();

			public Bucket(char c)
			{
				Char = c;
			}
		}

		public StringTable()
		{

		}

		public void Add(string value, int code)
		{
			if (value == null || value.Length == 0)
				throw new ArgumentNullException("value");

			InternalAdd(value, 0, buckets, code);
			_count++;
		}


		private void InternalAdd(string value, int offset, List<Bucket> buckets, int code)
		{
			char c = value[offset];

			foreach (Bucket b in buckets)
				if (b.Char == c)
				{
					if (offset == BUCKET_SIZE - 1 || value.Length == offset + 1)
					{
						b.Strings.Add(value.ToCharArray());
						b.Styles.Add(code);

						if (value.Length == offset + 1)
							b.BucketValue = true;
					}
					else
						InternalAdd(value, ++offset, b.Buckets, code);

					return;
				}

			Bucket newBuck = new Bucket(c);
			buckets.Add(newBuck);

			if (value.Length == offset + 1)
				newBuck.BucketValue = true;

			if (offset == BUCKET_SIZE - 1 || value.Length == offset + 1)
			{
				newBuck.Strings.Add(value.ToCharArray());
				newBuck.Styles.Add(code);
			}
			else
				InternalAdd(value, ++offset, newBuck.Buckets, code);
		}



		[DebuggerHidden]
		public int Match(char c)
		{
			if (++lastOffset < BUCKET_SIZE)
			{
				List<Bucket> bList = lastOffset == 0 ? buckets : lastBucket.Buckets;
				int bCount = bList.Count;

				for (int i = 0; i < bCount; i++)
				{
					Bucket b = bList[i];

					if (b.Char - c == 0)
					{
						lbBucket = lastBucket;
						lastBucket = b;
						return b.BucketValue ? b.Styles[0] : 0;
					}
				}

				lastOffset--;
			}
			else
			{
				int sCount = lastBucket.Strings.Count;
				for (int i = 0; i < sCount; i++)
				{
					char[] cz = lastBucket.Strings[i];
					if (cz.Length >= lastOffset + 1 && cz[lastOffset] - c == 0)
						return lastOffset == cz.Length - 1 ? lastBucket.Styles[i] : 0;
				}

				lastOffset--;
			}

			return -1;
		}


		[DebuggerHidden]
		public void Reset()
		{
			lastOffset = -1;
		}


		[DebuggerHidden]
		public void ResetStep()
		{
			if (lbBucket == null)
				throw new ArgumentNullException("lbBucket");

			lastBucket = lbBucket;
			lastOffset--;
		}


		public int Offset
		{
			[DebuggerHidden]
			get { return lastOffset; }
		}


		private int _count;
		public int Count
		{
			[DebuggerHidden]
			get { return _count; }
		}
	}
}
using System;
using System.Collections;
using System.Collections.Generic;

namespace Rsdn.Editor.Lexing
{
	public class PatternCombine
	{
		private List<Pattern> patterns;
		private List<Int32> codes;
		private int[] pcodes = new int[0];

		public PatternCombine()
		{
			patterns = new List<Pattern>();
			codes = new List<Int32>();
		}


		public void Add(string pattern, int code)
		{
			patterns.Add(new Pattern(pattern));
			codes.Add(code);
		}


		public void Compile()
		{
			pcodes = new int[patterns.Count];
		}


		public int Match(char c)
		{
			return _offset++ == -1 ? FirstMatch(c) : InternalMatch(c);
		}


		private int FirstMatch(char c)
		{
			int index = -1;

			for (int i = 0; i < patterns.Count; i++)
			{
				bool ret = patterns[i].Match(c);
				pcodes[i] = ret ? codes[i] : 0;

				if (ret && index == -1)
					index = patterns[i].EndSearch ? codes[i] : 0;
			}

			return index;
		}


		private int InternalMatch(char c)
		{
			int index = -1;

			for (int i = 0; i < pcodes.Length; i++)
			{
				if (pcodes[i] == 0)
					continue;

				bool ret = patterns[i].Match(c);

				pcodes[i] = ret ? 1 : 0;

				if (ret && index == -1)
					index = patterns[i].EndSearch ? codes[i] : 0;
				else if (!ret)
					pcodes[i] = 0;
			}

			return index;
		}


		public void Reset()
		{
			for (int i = 0; i < patterns.Count; i++)
			{
				pcodes[i] = 0;
				patterns[i].Reset();
			}

			_offset = -1;
		}


		public void ResetStep()
		{
			if (_offset == 0)
				Reset();

			for (int i = 0; i < patterns.Count; i++)
				patterns[i].ResetStep();

			_offset--;
		}


		private int _offset = -1;
		public int Offset
		{
			get { return _offset; }
		}
	}
}
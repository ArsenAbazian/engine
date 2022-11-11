using System;
using System.Collections;
using System.Collections.Generic;

namespace Rsdn.Editor.Lexing
{
	public class Pattern
	{
		private List<PatternStep> patterns;
		private int literalChecks;
		private PatternStep lastPattern;
		private PatternStep beforePattern;
		string ptr;

		public Pattern(string pattern)
		{
			ptr = pattern;
			patterns = new PatternParser().Parse(pattern);
		}


		public bool Match(char c)
		{
			c = Char.ToUpper(c);

			if (lastPattern.Continuos || (lastPattern.AndSequence &&
				lastPattern.SymbolCheck.Length > literalChecks))
			{
				if (MatchPattern(lastPattern, c))
					return true;
				else if (!lastPattern.Continuos)
					return false;
			}

		ptry:
			if (patterns.Count < _offset + 1)
				return false;

			beforePattern = lastPattern;
			lastPattern = patterns[_offset++];
			literalChecks = 0;
			bool ret = MatchPattern(lastPattern, c);

			if (!ret && lastPattern.Optional)
				goto ptry;
			else
				return ret;
		}


		private bool MatchPattern(PatternStep step, char c)
		{
			bool ret = false;

			if ((step.Checks & CheckTypes.IsNumeric) == CheckTypes.IsNumeric)
			{
				ret = Char.IsNumber(c);

				if (ret)
					return true;
			}

			if ((step.Checks & CheckTypes.IsAlphaNumeric) == CheckTypes.IsAlphaNumeric)
			{
				ret = Char.IsNumber(c) || Char.IsLetter(c);

				if (ret)
					return true;
			}

			if ((step.Checks & CheckTypes.IsAlpha) == CheckTypes.IsAlpha)
			{
				ret = Char.IsLetter(c);

				if (ret)
					return true;
			}

			if ((step.Checks & CheckTypes.IsBrace) == CheckTypes.IsBrace)
			{
				ret = c == '\'';

				if (ret)
					return true;
			}

			if ((step.Checks & CheckTypes.IsSpecificSymbol) == CheckTypes.IsSpecificSymbol)
			{
				ret = step.AndSequence ? step.SymbolCheck[literalChecks++] == c :
					step.SymbolCheck.IndexOf(c) != -1;

				if (ret)
					return true;
			}

			return false;
		}


		public void Reset()
		{
			_offset = 0;
			literalChecks = 0;
			lastPattern = default(PatternStep);
		}


		public void ResetStep()
		{
			if (lastPattern.AndSequence)
				literalChecks = literalChecks == 0 ? 0 : literalChecks - 1;
			else if (!lastPattern.Continuos && _offset != 0)
			{
				lastPattern = beforePattern;
				_offset--;
			}
		}


		private int _offset;
		public int Offset
		{
			get { return _offset; }
		}


		public bool EndSearch
		{
			get
			{
				if (!lastPattern.AndSequence)
					return patterns.Count == _offset;
				else
					return patterns.Count == _offset &&
						literalChecks == lastPattern.SymbolCheck.Length;
			}
		}
	}
}
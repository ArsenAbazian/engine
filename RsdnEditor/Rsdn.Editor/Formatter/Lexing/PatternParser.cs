using System;
using System.Collections;
using System.Collections.Generic;

namespace Rsdn.Editor.Lexing
{
	public class PatternParser
	{
		private CheckTypes types = default(CheckTypes);
		private BlockType block = BlockType.None;
		private string matchString = String.Empty;
		private bool literal;
		private bool andSequence;

		public List<PatternStep> Parse(string pattern)
		{
			return InternalParse(pattern.ToUpper());
		}


		private List<PatternStep> InternalParse(string pattern)
		{
			List<PatternStep> steps = new List<PatternStep>();

			for (int i = 0; i < pattern.Length; i++)
			{
				char c = pattern[i];

				switch (c)
				{
					case 'N':
					case 'A':
					case 'X':
					case 'B':
						ProcessBuiltIn(c, steps);
						break;
					case ' ':
						if (literal)
							matchString += c;
						break;
					case '!':
						if (literal)
							matchString += c;
						else if (i < pattern.Length - 1 && pattern[i + 1] == '\'')
							andSequence = true;
						break;
					case '\'':
						if (!literal)
							literal = true;
						else
							AddLiteral(c, steps);
						break;
					case '[':
						if (literal)
							matchString += c;
						else
							block = BlockType.Optional;
						break;
					case '(':
						if (literal)
							matchString += c;
						else
							block = BlockType.Group;
						break;
					case '{':
						if (literal)
							matchString += c;
						else
							block = BlockType.Continuos;
						break;
					case '<':
						if (literal)
							matchString += c;
						else
							block = BlockType.OptionalContinuos;
						break;
					case ']':
					case ')':
					case '}':
					case '>':
						if (literal)
							matchString += c;
						else
							AddBlock(steps);
						break;
					default:
						if (c != ' ' && c != '\t' && c != '\r' && c != '\n')
						{
							if (literal)
								matchString += c;
							else
								throw new FormatException();
						}
						break;
				}
			}

			return steps;
		}


		private void AddBlock(List<PatternStep> steps)
		{
			PatternStep s = new PatternStep();
			s.Checks = types;
			s.SymbolCheck = matchString;

			if (block == BlockType.OptionalContinuos)
				s.Continuos = s.Optional = true;
			else
			{
				s.Continuos = block == BlockType.Continuos;
				s.Optional = block == BlockType.Optional;
			}

			s.AndSequence = andSequence;
			steps.Add(s);
			matchString = String.Empty;
			block = BlockType.None;
			andSequence = false;
		}


		private void ProcessBuiltIn(char c, List<PatternStep> steps)
		{
			if (literal)
				matchString += c;
			else
			{
				if (block == BlockType.None)
				{
					PatternStep s = new PatternStep();
					s.Checks = CharToCheckType(c);
					steps.Add(s);
				}
				else
					types |= CharToCheckType(c);
			}
		}


		private CheckTypes CharToCheckType(char c)
		{
			switch (c)
			{
				case 'N': return CheckTypes.IsNumeric;
				case 'A': return CheckTypes.IsAlpha;
				case 'X': return CheckTypes.IsAlphaNumeric;
				case 'B': return CheckTypes.IsBrace;
				default: return CheckTypes.None;
			}
		}


		private void AddLiteral(char c, List<PatternStep> steps)
		{
			literal = false;

			if (block == BlockType.None)
			{
				PatternStep s = new PatternStep();
				s.Checks = CheckTypes.IsSpecificSymbol;
				s.SymbolCheck = matchString;
				s.AndSequence = andSequence;
				matchString = String.Empty;
				andSequence = false;
				steps.Add(s);
			}
			else
				types |= CheckTypes.IsSpecificSymbol;
		}
	}
}
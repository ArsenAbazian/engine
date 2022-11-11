using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;

namespace Rsdn.Editor.Lexing
{
	internal class ConfigurableScanner : IScanner
	{
		private CharKind oldKind;
		private CharKind lastKind;
		private Pattern operatorPattern;
		private Pattern numberPattern;
		private Pattern identifierPattern;

		public ConfigurableScanner(Pattern operatorPattern, Pattern numberPattern, Pattern identifierPattern)
		{
			this.operatorPattern = operatorPattern;
			this.numberPattern = numberPattern;
			this.identifierPattern = identifierPattern;
		}


		[DebuggerHidden]
		public CharKind GetCharKind(string text, int index)
		{
			char c = text[index];
			CharKind ret = CharKind.Empty;

			if (c == ' ' || c == '\t' || c == '\r')
			{
				Reset();
				ret = CharKind.Space;
			}
			else if (c == '\n')
				ret = CharKind.NewLine;
			else
			{
				switch (lastKind)
				{
					case CharKind.Identifier:
						ret = identifierPattern.Match(c) ? CharKind.Identifier : CharKind.Undefined;
						break;
					case CharKind.Numeric:
						ret = numberPattern.Match(c) ? CharKind.Numeric : CharKind.Undefined;
						break;
					case CharKind.Operator:
						ret = operatorPattern.Match(c) ? CharKind.Operator : CharKind.Undefined;
						break;
					case CharKind.Space:
					case CharKind.Undefined:
					case CharKind.Empty:
						ret = Match(c);
						break;
				}

				if (ret == CharKind.Undefined && (lastKind == CharKind.Identifier ||
					lastKind == CharKind.Numeric || lastKind == CharKind.Operator))
				{
					Reset();
					ret = Match(c);
				}
			}

			oldKind = lastKind;
			lastKind = ret;
			return ret;
		}


		private CharKind Match(char c)
		{
			CharKind ret = identifierPattern.Match(c) ? CharKind.Identifier : CharKind.Undefined;

			if (ret != CharKind.Identifier)
			{
				ret = operatorPattern.Match(c) ? CharKind.Operator : CharKind.Undefined;

				if (ret != CharKind.Operator)
					ret = numberPattern.Match(c) ? CharKind.Numeric : CharKind.Undefined;
			}

			return ret;
		}


		void IScanner.Reset()
		{
			oldKind = lastKind = CharKind.Empty;
			Reset();
		}


		void IScanner.ResetStep()
		{
			this.operatorPattern.ResetStep();
			this.numberPattern.ResetStep();
			this.identifierPattern.ResetStep();
			lastKind = oldKind;
			oldKind = CharKind.Empty;
		}


		private void Reset()
		{
			this.operatorPattern.Reset();
			this.numberPattern.Reset();
			this.identifierPattern.Reset();
		}
	}
}
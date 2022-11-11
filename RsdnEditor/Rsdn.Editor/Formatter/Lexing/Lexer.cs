using System;
using System.Collections;
using System.Collections.Generic;

namespace Rsdn.Editor.Lexing
{
	public class Lexer : IConfigurableLexer
	{
		private byte userState;
		private int nestedUserState;
		private string text;
		private CharKind charKind;
		private int position;
		private int blockPosition = -1;
		private bool ignoreFirstBlock;
		private long keywordHash;
		private Block resBlock;
		private Block currentBlock;

		private int lastBlockPosition = -1;
		private int lastBlockCode = -1;

		public Lexer(SyntaxDefinition syntax)
		{
			_tokens = new List<Token>();
			_syntax = syntax;
		}


		public int Process(string text, int position)
		{
			_tokens.Clear();

			if (text.Length == 0)
				return 0;

			Block b = null;

			if (!_syntax.BlockMap.TryGetValue(userState, out b))
				b = _syntax.RootBlock;

			//if (b == null)
			//{
			keywordHash = 0;
			resBlock = null;
			//}

			lastBlockPosition = lastBlockCode = -1;
			this.ignoreFirstBlock = false;
			this.text = text;
			this.position = position;
			this.blockPosition = -1;
			this.charKind = CharKind.Empty;
			return b.LexerCode != null ? Delegate(b.Parent, b) :
				InternalProcess(b);
		}


		private int InternalProcess(Block block)
		{
			try
			{
				currentBlock = block;
				block.Scanner.Reset();
				int ownStart = position;
				int countEntity = 0;
				bool keyMatched = block.FirstKeywordRule && userState == block.Code;
				int textLength = text.Length;
				CharKind lastCharKind = default(CharKind);

				for (; position < textLength; position++)
				{
					char c = CharAt(position);
					charKind = block.Scanner.GetCharKind(text, position);

					switch (charKind)
					{
						case CharKind.Identifier:
							if (block.Keywords.Offset != -1 || !CheckBlock(block, ownStart))
							{
								if (!ignoreFirstBlock && (block.Keywords.Offset != -1 ||
									lastCharKind != CharKind.Identifier) &&
									blockPosition == -1)
									CheckKeyword(block, ownStart, ref keyMatched);
							}
							else
								return position;
							break;
						case CharKind.Numeric:
							countEntity++;
							CheckSpecial(block, ref countEntity, ref charKind);
							break;
						case CharKind.Operator:
							if (!keyMatched)
								keyMatched = CheckFirstKeyword(block, ownStart);

							if (!CheckBlock(block, ownStart))
							{
								if (ignoreFirstBlock)
									continue;

								countEntity++;
								CheckSpecial(block, ref countEntity, ref charKind);
							}
							else
								return position;
							break;
						case CharKind.Undefined:
							if (!keyMatched)
								keyMatched = CheckFirstKeyword(block, ownStart);

							if (CheckBlock(block, ownStart))
								return position;
							break;
						case CharKind.NewLine:
							if (!keyMatched)
								keyMatched = CheckFirstKeyword(block, ownStart);

							ProcessFrameEnd(block, ownStart);
							break;
						case CharKind.Space:
							block.Keywords.Reset();

							if (!keyMatched)
								keyMatched = CheckFirstKeyword(block, ownStart);

							if (block.End != null && block.End[0] - ' ' == 0)
								ProcessBlockEnd(block, ref ownStart);
							break;
					}

					lastCharKind = charKind;
				}

				ProcessFrameEnd(block, ownStart);
				return position;
			}
			finally
			{
				block.Keywords.Reset();
				block.BlockStarts.Reset();
			}
		}


		private bool CheckFirstKeyword(Block block, int ownStart)
		{
			if (block.Keywords.Count == 0 && block.FirstKeywordRule)
			{
				_tokens.Add(new Token(TokenType.Keyword, ownStart,
					position - ownStart, block.Stylesheet.KeywordStyle));
				return true;
			}
			else
				return false;
		}


		private char CharAt(int position)
		{
			return currentBlock.CaseSensitive ? text[position] : Char.ToUpper(text[position]);
		}


		private void CheckSpecial(Block block, ref int countEntity, ref CharKind charKind)
		{
			CharKind ck = LookAhead(block);

			if (ck != charKind)
			{
				Token t = new Token(charKind == CharKind.Operator ?
					TokenType.Operator : TokenType.Numeric, position - countEntity + 1, countEntity,
					charKind == CharKind.Operator ? block.Stylesheet.OperatorStyle : block.Stylesheet.NumericStyle);

				_tokens.Add(t);
				countEntity = 0;
			}
		}

		private CharKind LookAhead(Block block)
		{
			CharKind ret = text.Length <= position + 1 ? CharKind.Empty :
				block.Scanner.GetCharKind(text, position + 1);
			block.Scanner.ResetStep();
			return ret;
		}


		private void StartBlock(int blockCode, Block block, int ownStart)
		{
			block.BlockStarts.Reset();
			Block b = _syntax.BlockMap[blockCode];
			lastBlockPosition = lastBlockCode = -1;

			if (/*block.Keywords.Count == 0 && */block.LexerCode == null)
				_tokens.Add(new Token(TokenType.Block, ownStart,
					position - ownStart, block.Stylesheet.Style));

			_tokens.Add(new Token(TokenType.BlockStart, position - blockPosition - 1,
				blockPosition + 1, b.Stylesheet.StartStyle));
			blockPosition = -1;

			if (b.LexerCode == null)
				InternalProcess(b);
			else
				Delegate(block, b);
		}


		private int Delegate(Block parent, Block block)
		{
			ILexer lexer = LexerManager.Instance.GetLexer(block.LexerCode);

			if (lexer == null)
				return InternalProcess(block);
			else
			{
				lexer.ParentBlock = block;
				lexer.State = nestedUserState;
				int oldPosition = position;
				position = lexer.Process(text, position);
				nestedUserState = lexer.State;
				lexer.ParentBlock = null;
				_tokens.AddRange(lexer.Tokens);
				userState = block.Code;
				_continueState = lexer.ContinueState;

				if (block.ReparseRule == ReparseRule.None)
					return _continueState ? position : InternalProcess(parent);
				else
				{
					bool oldCont = _continueState;
					position = oldPosition;
					userState = (Byte)(nestedUserState >> 16);
					Block b = block.ReparseRule == ReparseRule.OwnGrammar ?
						block : block.Parent;

					if (!_syntax.BlockMap.TryGetValue(userState, out b))
						b = _syntax.RootBlock;

					InternalProcess(b);
					nestedUserState = (Byte)nestedUserState | (Byte)(nestedUserState >> 8) << 8 | userState << 16;
					userState = block.Code;
					_continueState = oldCont;
					return position;
				}
			}
		}


		private void CheckKeyword(Block block, int ownStart, ref bool keyMatched)
		{
			int ret = block.Keywords.Match(CharAt(position));

			if (ret > 0)
			{
				if (LookAhead(block) != CharKind.Identifier)
				{
					int style = ret & UInt16.MaxValue;
					ushort hash = (UInt16)(ret >> 16);
					keywordHash += hash;

					if (!keyMatched)
						_tokens.Add(new Token(TokenType.Keyword, position - block.Keywords.Offset,
							block.Keywords.Offset + 1, style));

					block.Keywords.Reset();

					if (hash != 0)
					{
						byte code;
						_syntax.RootBlock.HashStarts.TryGetValue(keywordHash, out code);

						if (code != 0)
						{
							keywordHash = 0;

							if (block.Parent == null)
								StartBlock(code, block, ownStart);
							else
								resBlock = _syntax.BlockMap[code];
						}
					}

					keyMatched = block.FirstKeywordRule;
				}
			}
			else if (ret < 0)
				block.Keywords.Reset();
		}


		private bool CheckBlock(Block block, int ownStart)
		{
			if (ignoreFirstBlock)
			{
				ignoreFirstBlock = false;
				return false;
			}

			char c = CharAt(position);
			int res = block.BlockStarts.Match(c);

			if (res >= 0)
				blockPosition++;

			if (res > 0)
			{
				if (c == ' ' || text.Length == position + 1 ||
					(text.Length > position + 1 && block.BlockStarts.Match(CharAt(position + 1)) < 0))
				{
					position++;
					StartBlock(res, block, ownStart);
					return true;
				}
				else if (text.Length > position + 1)
				{
					lastBlockPosition = position;
					lastBlockCode = res;
					block.BlockStarts.ResetStep();
				}
			}
			else if (res < 0)
			{
				block.BlockStarts.Reset();

				if (lastBlockCode != -1)
				{
					blockPosition -= position - (lastBlockPosition + 1);
					position = lastBlockPosition + 1;
					StartBlock(lastBlockCode, block, ownStart);
					return true;
				}
				else if (blockPosition > -1)
				{
					position -= blockPosition + 1;
					blockPosition = -1;
					ignoreFirstBlock = true;
					//return false;
				}

				blockPosition = -1;

				if (block.Parent != null && block.End != null && position + block.End.Length <= text.Length &&
				   block.End[0] == CharAt(position))
				{
					if (FindBlockEnd(block))
						return ProcessBlockEnd(block, ref ownStart);
				}

				if (_parentBlock != null && _parentBlock.End != null &&
					position + _parentBlock.End.Length <= text.Length && _parentBlock.End[0] == CharAt(position))
				{
					int oldPosition = position;

					if (FindBlockEnd(_parentBlock))
					{
						if (ProcessBlockEnd(_parentBlock, ref ownStart))
						{
							if (block.Parent != null)
							{
								_tokens.Add(new Token(TokenType.Block, ownStart,
									oldPosition - ownStart, block.Stylesheet.Style));
							}

							return true;
						}
					}
				}
			}

			return false;
		}


		private bool ProcessBlockEnd(Block curBlock, ref int ownStart)
		{
			char pc = CharAt(position - 1);
			_continueState = false;

			if (pc == curBlock.EscapeChar/* || (curBlock.End.Length != 0 &&
                pc == curBlock.End[0])*/
										)
				return false;

			if (curBlock != _parentBlock)
				_tokens.Add(new Token(TokenType.Block, ownStart,
					position - ownStart - curBlock.End.Length + 1, curBlock.Stylesheet.Style));

			_tokens.Add(new Token(TokenType.BlockEnd, position - curBlock.End.Length + 1,
				curBlock.End.Length, curBlock.Stylesheet.EndStyle));
			position++;

			if (resBlock != null && (curBlock.Parent == null || curBlock.Parent.Parent == null))
			{
				InternalProcess(resBlock);
				resBlock = null;
				keywordHash = 0;
			}
			else if (curBlock != _parentBlock)
				InternalProcess(curBlock.Parent);

			return true;
		}


		private bool FindBlockEnd(Block block)
		{
			for (int i = 0; i < block.End.Length; i++)
			{
				if (!(block.End[i] == CharAt(i + position)))
					return false;
			}

			position += block.End.Length - 1;
			return true;
		}


		private void ProcessFrameEnd(Block block, int ownStart)
		{
			if (block.Parent != null && block.Keywords.Count == 0)
				_tokens.Add(new Token(TokenType.Block, ownStart,
					position - ownStart, block.Stylesheet.Style));

			bool cc = false;

			if (!block.MultiLine && block.ContinuationChar != '\0')
				cc = FindContinuationChar(block.ContinuationChar);

			if (block.Parent != null && !block.MultiLine && !cc)
				InternalProcess(block.Parent);
			else
			{
				Block b = block.Parent != null && !block.MultiLine && !cc ?
					block.Parent : block;
				userState = b.Code;
				_continueState = b.MultiLine || cc;
			}
		}

		private bool FindContinuationChar(char cc)
		{
			for (int i = position - 1; i > 0; i--)
			{
				char c = CharAt(i);
				if (c != ' ' && c != '\t')
					return c == cc;
			}

			return false;
		}


		private Block _parentBlock;
		public Block ParentBlock
		{
			get { return _parentBlock; }
			set { _parentBlock = value; }
		}


		private List<Token> _tokens;
		public List<Token> Tokens
		{
			get { return _tokens; }
		}


		public int State
		{
			get { return userState | nestedUserState << 8; }
			set
			{
				this.userState = (Byte)(value & Byte.MaxValue);
				this.nestedUserState = value >> 8;
			}
		}


		private bool _continueState;
		public bool ContinueState
		{
			get { return _continueState; }
		}


		private SyntaxDefinition _syntax;
		public SyntaxDefinition Syntax
		{
			get { return _syntax; }
			set { _syntax = value; }
		}
	}
}
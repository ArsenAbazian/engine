using System;
using System.Collections;
using System.Collections.Generic;

namespace Rsdn.Editor.Lexing
{
	public sealed class LexerManager
	{
		private Dictionary<String,ILexer> lexers;

		public LexerManager()
		{
			lexers = new Dictionary<String,ILexer>();
		}

		public bool RegisterLexer(ILexer lexer, string code)
		{
			if (lexers.ContainsKey(code))
				return false;
			else
			{
				lexers.Add(code, lexer);
				return true;
			}
		}


		public ILexer GetLexer(string code)
		{
			ILexer lexer;
			lexers.TryGetValue(code, out lexer);
			return lexer;
		}


		private readonly static LexerManager _instance = new LexerManager();
		public static LexerManager Instance
		{
			get { return _instance; }
		}
	}
}
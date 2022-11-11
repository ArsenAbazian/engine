using System;
using System.Collections;
using System.Collections.Generic;

namespace Rsdn.Editor.Lexing
{
	public sealed class Stylesheet
	{
		internal Stylesheet()
		{

		}


		private int _style;
		public int Style
		{
			get { return _style; }
			internal set { _style = value; }
		}

		private int _startStyle;
		public int StartStyle
		{
			get { return _startStyle; }
			internal set { _startStyle = value; }
		}

		private int _endStyle;
		public int EndStyle
		{
			get { return _endStyle; }
			internal set { _endStyle = value; }
		}


		private int _operatorStyle;
		public int OperatorStyle
		{
			get { return _operatorStyle; }
			internal set { _operatorStyle = value; }
		}


		private int _numericStyle;
		public int NumericStyle
		{
			get { return _numericStyle; }
			internal set { _numericStyle = value; }
		}


		private int _identifierStyle;
		public int IdentifierStyle
		{
			get { return _identifierStyle; }
			internal set { _identifierStyle = value; }
		}


		private int _keywordStyle;
		public int KeywordStyle
		{
			get { return _keywordStyle; }
			internal set { _keywordStyle = value; }
		}
	}
}
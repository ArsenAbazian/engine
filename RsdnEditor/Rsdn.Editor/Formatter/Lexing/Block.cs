using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;

namespace Rsdn.Editor.Lexing
{
	public sealed class Block
	{
		#region Construction
		public Block(string start, string end, Block parent)
			: this()
		{
			_start = start == null || start.Length == 0 ? null : start;
			_end = end == null || end.Length == 0 ? null : end;
			_parent = parent;
		}


		public Block()
		{
			_multiLine = true;
			_keywords = new StringTable();
			_blockStarts = new StringTable();
			_blockEnds = new StringTable();
			_hashStarts = new Dictionary<Int64, Byte>();
			_attributes = new Hashtable();
			_stylesheet = new Stylesheet();
		}
		#endregion


		#region Methods
		public void AddNestedBlock(Block b)
		{
			_blockStarts.Add(b.Start, b.Code);
			_blockEnds.Add(b.End, b.Code);
		}
		#endregion


		#region Properties
		private Hashtable _attributes;
		public Hashtable Attributes
		{
			get { return _attributes; }
		}


		private byte _code;
		public byte Code
		{
			//[DebuggerHidden]
			get { return _code; }
			set 
            { 
                _code = value; 
            }
		}


		private ReparseRule _reparseRule = ReparseRule.None;
		public ReparseRule ReparseRule
		{
			get { return _reparseRule; }
			set { _reparseRule = value; }
		}


		private string _lexerCode;
		public string LexerCode
		{
			get { return _lexerCode; }
			set { _lexerCode = value; }
		}


		private Block _parent;
		public Block Parent
		{
			get { return _parent; }
			internal set { _parent = value; }
		}


		private Dictionary<Int64, Byte> _hashStarts;
		public Dictionary<Int64, Byte> HashStarts
		{
			get { return _hashStarts; }
		}


		private long _hashEnd;
		public long HashEnd
		{
			get { return _hashEnd; }
			set { _hashEnd = value; }
		}


		private Stylesheet _stylesheet;
		public Stylesheet Stylesheet
		{
			[DebuggerHidden]
			get { return _stylesheet; }
			set { _stylesheet = value; }
		}


		private bool _caseSensitive = true;
		public bool CaseSensitive
		{
			[DebuggerHidden]
			get { return _caseSensitive; }
			set { _caseSensitive = value; }
		}


		private bool _multiLine;
		public bool MultiLine
		{
			[DebuggerHidden]
			get { return _multiLine; }
			set { _multiLine = value; }
		}


		private string _start;
		public string Start
		{
			[DebuggerHidden]
			get { return _start; }
			set { _start = value == null || value.Length == 0 ? null : value; }
		}


		private char _escapeChar;
		public char EscapeChar
		{
			[DebuggerHidden]
			get { return _escapeChar; }
			set { _escapeChar = value; }
		}


		private char _continuationChar;
		public char ContinuationChar
		{
			[DebuggerHidden]
			get { return _continuationChar; }
			set { _continuationChar = value; }
		}


		private string _end;
		public string End
		{
			[DebuggerHidden]
			get { return _end; }
			set { _end = value == null || value.Length == 0 ? null : value; }
		}


		private StringTable _keywords;
		public StringTable Keywords
		{
			[DebuggerHidden]
			get { return _keywords; }
		}


		private StringTable _blockStarts;
		internal StringTable BlockStarts
		{
			[DebuggerHidden]
			get { return _blockStarts; }
		}


		private bool _firstKeywordRule;
		public bool FirstKeywordRule
		{
			get { return _firstKeywordRule; }
			set { _firstKeywordRule = value; }
		}

		private StringTable _blockEnds;
		internal StringTable BlockEnds
		{
			[DebuggerHidden]
			get { return _blockEnds; }
		}


		private IScanner _scanner = new Scanner();
		internal IScanner Scanner
		{
			[DebuggerHidden]
			get { return _scanner; }
			set { _scanner = value; }
		}
		#endregion
	}
}
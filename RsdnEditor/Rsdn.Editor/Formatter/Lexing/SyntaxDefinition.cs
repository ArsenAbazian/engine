using System;
using System.Collections;
using System.Collections.Generic;

namespace Rsdn.Editor.Lexing
{
	public sealed class SyntaxDefinition
	{
		public SyntaxDefinition(string key, IStyleProcessor styleProcessor, Block rootBlock)
			: this(key, styleProcessor)
		{
			_rootBlock = rootBlock;
		}


		internal SyntaxDefinition(string key, IStyleProcessor styleProcessor)
		{
			_key = key;
			_styleProcessor = styleProcessor;
			_blockMap = new Dictionary<Int32,Block>();
		}


		private IStyleProcessor _styleProcessor;
		public IStyleProcessor StyleProcessor
		{
			get { return _styleProcessor; }
		}


		private string _key;
		public string Key
		{
			get { return _key; }
		}


		private Dictionary<Int32,Block> _blockMap;
		public Dictionary<Int32, Block> BlockMap
		{
			get { return _blockMap; }
		}


		private Block _rootBlock;
		public Block RootBlock
		{
			get { return _rootBlock; }
			internal set { _rootBlock = value; }
		}
	}

}
using System;
using System.IO;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Xml;
using System.Xml.Schema;

namespace Rsdn.Editor.Lexing
{
	public sealed class DefinitionReader<T> where T : IStyleProcessor, new()
	{
		private const string SCHEMA = "Rsdn.Editor.Formatter.Lexing.Grammar.xsd";
		private string lexerKey;
		private Dictionary<String, ConfigurableScanner> scanners;
		private Dictionary<String, Block> blocks;
		private Dictionary<String, Int32> styles;
		private class KeywordList : List<String>
		{
			public readonly string Class;
			public KeywordList(string @class) { this.Class = @class; }
		}
		private class ReaderStopException : ApplicationException { }

		public DefinitionReader()
		{
			scanners = new Dictionary<String, ConfigurableScanner>();
			blocks = new Dictionary<String, Block>();
			styles = new Dictionary<String, Int32>();
			_styleProcessor = new T();
		}

		public SyntaxDefinition Process(Stream stream)
		{
			_errors.Clear();
			scanners.Clear();
			blocks.Clear();
			styles.Clear();
			XmlReaderSettings settings = new XmlReaderSettings();
			settings.IgnoreComments = true;
			settings.ValidationType = ValidationType.Schema;
			Stream schemaStream = typeof(DefinitionReader<>).Assembly.
				GetManifestResourceStream(SCHEMA);
			settings.Schemas.Add(XmlSchema.Read(schemaStream, null));
			settings.ValidationEventHandler += new ValidationEventHandler(DefinitionError);
			XmlReader reader = null;

			try
			{
				reader = XmlReader.Create(stream, settings);
				return InternalProcess(reader);
			}
			catch (XmlException ex)
			{
				_errors.Add(new DefinitionError(ex.LineNumber,
					ex.LinePosition, ex.Message, DefinitionErrorType.XmlError));
			}
			catch (Exception ex)
			{
				if (ex is ReaderStopException)
					return null;
				else
				{
					_errors.Add(new DefinitionError(ex.Message, DefinitionErrorType.Undefined));
					throw;
				}
			}
			finally
			{
				if (reader != null)
					reader.Close();
			}

			return null;
		}


		private SyntaxDefinition InternalProcess(XmlReader reader)
		{
			while (reader.Read())
			{
				if (CheckStart(reader, "Lexer"))
					lexerKey = reader["Key"];
				if (CheckStart(reader, "Scanners"))
					ProcessScanners(reader);
				else if (CheckStart(reader, "Blocks"))
					ProcessBlocks(reader);
				else if (CheckStart(reader, "Styles"))
					ProcessStyles(reader);
			}

			return BuildDefinition();
		}


		private SyntaxDefinition BuildDefinition()
		{
			SyntaxDefinition def = new SyntaxDefinition(lexerKey, _styleProcessor);
			string chunk = null;
			Dictionary<String, UInt32> hash = new Dictionary<String, UInt32>();
			int startHash = 2;
			byte blockCode = 0;

			foreach (Block b in blocks.Values)
			{
				blockCode++;

				if ((chunk = b.Attributes["Scanner"] as String) != null)
					b.Scanner = scanners[chunk];

                b.Code = blockCode;
				b.Stylesheet.Style = !String.IsNullOrEmpty(chunk = b.Attributes["ClassBlock"] as String) ? styles[chunk] : 0;
				b.Stylesheet.StartStyle = !String.IsNullOrEmpty(chunk = b.Attributes["ClassStartBlock"] as String) ?
					styles[chunk] : b.Stylesheet.Style;
				b.Stylesheet.EndStyle = !String.IsNullOrEmpty(chunk = b.Attributes["ClassEndBlock"] as String) ?
					styles[chunk] : b.Stylesheet.Style;
				b.Stylesheet.IdentifierStyle = !String.IsNullOrEmpty(chunk = b.Attributes["ClassIdentifier"] as String) ?
					styles[chunk] : b.Stylesheet.Style;
				b.Stylesheet.OperatorStyle = !String.IsNullOrEmpty(chunk = b.Attributes["ClassOperator"] as String) ?
					styles[chunk] : b.Stylesheet.Style;
				b.Stylesheet.NumericStyle = !String.IsNullOrEmpty(chunk = b.Attributes["ClassNumeric"] as String) ?
					styles[chunk] : b.Stylesheet.Style;
				b.Stylesheet.KeywordStyle = !String.IsNullOrEmpty(chunk = b.Attributes["ClassKeyword"] as String) ?
					styles[chunk] : b.Stylesheet.Style;
				b.Parent = !String.IsNullOrEmpty(chunk = b.Attributes["Parent"] as String) ? blocks[chunk] : null;

				if (b.Parent != null)
				{
					foreach (string s in (List<String>)b.Attributes["LiteralStarts"])
						if (s.Length > 0)
							b.Parent.BlockStarts.Add(b.CaseSensitive ? s : s.ToUpper(), blockCode);

					bool sameHash = false;

					foreach (string lit in (List<String>)b.Attributes["KeyStarts"])
					{
						long sum = 0;

						foreach (string s in lit.Split(new char[] { ' ', '\t', '\r', '\n' },
							StringSplitOptions.RemoveEmptyEntries))
						{
							if (s[0] == '(')
							{
								sameHash = true;

								if (s.Length == 1)
									continue;
							}
							else if (s[0] == ')')
								sameHash = false;

							hash.Add(b.CaseSensitive ? s : s.ToUpper(),
								(UInt32)(!sameHash ? startHash *= 2 : startHash));

							if (!sameHash)
								sum += startHash;
						}

						b.Parent.HashStarts.Add(sum, blockCode);
					}
				}
				else
					def.RootBlock = b;

				foreach (KeywordList keywords in (List<KeywordList>)b.Attributes["Keywords"])
					foreach (string s in keywords)
					{
						string kw = b.CaseSensitive ? s : s.ToUpper();
						b.Keywords.Add(kw, (UInt16)styles[keywords.Class] | (UInt16)(hash.ContainsKey(kw) ? hash[kw] : 0));
					}

				b.Attributes.Clear();
				def.BlockMap.Add(blockCode, b);
			}

			return def;
		}


		private void ProcessStyles(XmlReader reader)
		{
			while (reader.Read())
			{
				if (CheckStart(reader, "Class"))
				{
                    styles.Add(reader["Key"], styles.Count + 1);
                    _styleProcessor.ProcessStyle(reader, styles.Count);
                }
				else if (CheckEnd(reader, "Styles"))
					return;
			}
		}

		private void ProcessScanners(XmlReader reader)
		{
			while (reader.Read())
			{
				if (CheckStart(reader, "Scanner"))
					ProcessScanner(reader, reader["Key"]);
				else if (CheckEnd(reader, "Scanners"))
					return;
			}
		}


		private void ProcessBlocks(XmlReader reader)
		{
			while (reader.Read())
			{
				if (CheckStart(reader, "Block"))
					ProcessBlock(reader, reader["Key"]);
				else if (CheckEnd(reader, "Blocks"))
					return;
			}
		}


		private void ProcessBlock(XmlReader reader, string key)
		{
			Block block = new Block();
			blocks.Add(key, block);
			string chunk = String.Empty;
			block.End = reader["End"];
			block.CaseSensitive = reader["CaseSensitive"] == "true";
			block.MultiLine = reader["Multiline"] == "true";
			block.EscapeChar = ((chunk = reader["EscapeChar"]) ?? String.Empty).Length > 0 ? chunk[0] : '\0';
			block.ContinuationChar = ((chunk = reader["ContinuationChar"]) ?? String.Empty).Length > 0 ? chunk[0] : '\0';
			block.ReparseRule = (chunk = reader["ReparseRule"]) == null ? default(ReparseRule) :
				(ReparseRule)Enum.Parse(typeof(ReparseRule), chunk);
			block.FirstKeywordRule = reader["FirstKeywordRule"] == "true";
			block.LexerCode = reader["LexerCode"];
			block.Attributes.Add("Scanner", ((chunk = reader["Scanner"]) ?? String.Empty).Length > 0 ? chunk : null);
			block.Attributes.Add("Parent", reader["Parent"]);

			List<String> literalStarts = new List<String>();
			List<String> keyStarts = new List<String>();
			List<KeywordList> keywords = new List<KeywordList>();
			block.Attributes.Add("LiteralStarts", literalStarts);
			block.Attributes.Add("KeyStarts", keyStarts);
			block.Attributes.Add("Keywords", keywords);

			while (reader.Read())
			{
				if (CheckStart(reader, "Add"))
					block.Attributes.Add(String.Concat("Class", reader["For"]), reader["Class"]);
				else if (CheckStart(reader, "Start"))
				{
					if (String.Compare(reader["Type"], "Literal") == 0)
						literalStarts.Add(reader.ReadString());
					else
						keyStarts.Add(reader.ReadString());
				}
				else if (CheckStart(reader, "Keywords"))
				{
					KeywordList list = new KeywordList(reader["Class"]);

					foreach (string s in reader.ReadString().Split(new char[] { ' ', '\t', '\r', '\n' },
						StringSplitOptions.RemoveEmptyEntries))
						list.Add(s);

					keywords.Add(list);
				}
				else if (CheckEnd(reader, "Block"))
					return;
			}
		}


		private bool CheckStart(XmlReader reader, string name)
		{
			return String.Compare(reader.Name, name) == 0 && reader.IsStartElement();
		}


		private bool CheckEnd(XmlReader reader, string name)
		{
			return String.Compare(reader.Name, name) == 0 && !reader.IsStartElement();
		}

		private void ProcessScanner(XmlReader reader, string key)
		{
			Pattern numericPattern = null;
			Pattern identPattern = null;
			Pattern operPattern = null;

			while (reader.Read())
			{
				if (CheckStart(reader, "Pattern"))
				{
					switch (reader.GetAttribute("Type"))
					{
						case "Operator":
							operPattern = new Pattern(reader["Value"]);
							break;
						case "Numeric":
							numericPattern = new Pattern(reader["Value"]);
							break;
						case "Identifier":
							identPattern = new Pattern(reader["Value"]);
							break;
					}
				}
				else if (CheckEnd(reader, "Scanner"))
				{
					scanners.Add(key, new ConfigurableScanner(operPattern, numericPattern, identPattern));
					return;
				}
			}
		}

		void DefinitionError(object sender, ValidationEventArgs e)
		{
			_errors.Add(new DefinitionError(e.Exception.LineNumber, e.Exception.LinePosition,
				e.Message, e.Severity == XmlSeverityType.Warning ? DefinitionErrorType.SchemaWarning :
				DefinitionErrorType.SchemaError));

			if (e.Severity == XmlSeverityType.Error)
				throw new ReaderStopException();
		}


		private T _styleProcessor;
		public T StyleProcessor
		{
			get { return _styleProcessor; }
		}


		public bool HasErrors
		{
			get { return _errors.Count > 0; }
		}


		private List<DefinitionError> _errors = new List<DefinitionError>();
		public ReadOnlyCollection<DefinitionError> Errors
		{
			get { return _errors.AsReadOnly(); }
		}
	}
}
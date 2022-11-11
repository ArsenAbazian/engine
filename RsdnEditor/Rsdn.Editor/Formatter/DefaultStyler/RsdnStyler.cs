//#define STYLED_IDENT

using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace Rsdn.Editor.DefaultStyler
{
	class RsdnStyler : IStyler
	{
		public void StartParse(DocumentRowCollection rows, int currentState)
		{
			_rows = rows;
			_currentState = currentState;
		}

		private DocumentRowCollection _rows;
		int _currentState;

		public int CurrentState
		{
			get { return _currentState; }
		}

		List<AppliedStyle> _styles = new List<AppliedStyle>(20);

		public void GetStyles(int rowIndex)
		{
			_styles.Clear();
			DocumentRow currentRow = _rows[rowIndex];

			currentRow.StartStylerState = _currentState;
			string text = currentRow.Text;
			int len = text.Length;
			char[] chars = new char[len + 2];
			text.CopyTo(0, chars, 0, len);


			int i = 0;

			if (_currentState == RsdnState.MultilineComment)
			{
				DoMultilineComment(chars, _styles, ref i, len, ref _currentState, false);
				i++;
			}
			else if (_currentState == RsdnState.MultilineString)
			{
				DoMultilineString(chars, _styles, ref i, len, ref _currentState, false);
				i++;
			}


			for (; i < len; i++)
			{
				if (!Utils.IsWhiteSpace(chars[i]))
				{
					// Директива препроцессора?
					if (chars[i] == '#')
						DoPrepocessor(chars, _styles, ref i, ref len);

					break;
				}
			}

			for (; i < len; i++)
			{
				switch (chars[i])
				{
					case '/':
						if (chars[i + 1] == '*')
							DoMultilineComment(chars, _styles, ref i, len,
								ref _currentState, true);
						else if (chars[i + 1] == '/')
							DoInlineComment(_styles, ref i, len);
						break;
					case '@':
						if (chars[i + 1] == '"')
							DoMultilineString(chars, _styles, ref i, len,
								ref _currentState, true);
						break;
					case '"': DoInlineString(chars, _styles, ref i, len); break;
					case '\'': DoInlineChar(chars, _styles, ref i, len); break;
					case ':': DoSmails(chars, _styles, ref i, len); break;
					default:
						if (Utils.IsLetter(chars[i]))
							DoIdent(chars, _styles, ref i, ref len);
						break;
				}
			}

			currentRow.IsContinueStylingOnNextLine =
				_currentState == RsdnState.MultilineComment
				|| _currentState == RsdnState.MultilineString;

			if (_styles.Count == 0)
				currentRow.AppliedStyles = _emptyAppliedStyle;
			else
				currentRow.AppliedStyles = _styles.ToArray();
		}

		#region DoSmails

		private static void DoSmails(char[] chars, List<AppliedStyle> styles, ref int i, int len)
		{
			int smailLen;
			switch (chars[i + 1])
			{
				case ')':
					if (!IsSmail(Smail, chars, ref i, len))
						return;
					smailLen = Smail.Length;
					if (IsSmail(Biggrin, chars, ref i, len))
						smailLen = Biggrin.Length;
					if (IsSmail(Lol, chars, ref i, len))
						smailLen = Lol.Length;
					break;
				case 's':
					if (!IsSmail(Super, chars, ref i, len))
						return;
					smailLen = Super.Length;
					break;
				case 'x':
					if (!IsSmail(Xz, chars, ref i, len))
						return;
					smailLen = Xz.Length;
					break;
				default: return;
			}

			styles.Add(new AppliedStyle(_imageStyle, i, smailLen));
			i += smailLen - 1;
		}

		private static bool IsSmail(string smail, char[] chars, ref int i, int len)
		{
			if (len - i < smail.Length)
				return false;

			return new string(chars, i, smail.Length) == smail;
		}
		
		#endregion

		#region DoPrepocessor

		private static void DoPrepocessor(char[] chars, List<AppliedStyle> styles,
			ref int i, ref int length)
		{
			int start = i;
			styles.Add(new AppliedStyle(_preprocessorStyle, start, 1));

			for (i += 1; i < length && Utils.IsWhiteSpace(chars[i]); i++)
				;

			if (Utils.IsLetter(chars[i]))
			{
				start = i;

				for (; i <= length; i++)
					if (!Utils.IsLetterOrDigit(chars[i]))
					{
						int len = i - start;
						string str = new string(chars, start, len);

						switch (str)
						{
							case "if":
							case "else":
							case "endif":
							case "error":
							case "warning":
							case "define":
							case "region":
							case "endregion":
								styles.Add(new AppliedStyle(_preprocessorStyle, start, len));
								return;
							default:
								styles.Add(new AppliedStyle(_errorStyle, start, len));
								return;
						}
					}
			}
			else
				styles.Add(new AppliedStyle(_errorStyle, start, length - start));

			i = length - 1;
		}
		
		#endregion

		#region DoInlineChar

		private static void DoInlineChar(char[] chars, List<AppliedStyle> styles,
			ref int i, int length)
		{
			int start = i;
			i += 1;

			for (; i < length; i++)
			{
				if (chars[i] == '\\')
				{
					char ch = chars[i + 1];

					if (ch == '\'' || ch == '\\')
						i++;
				}
				else if (chars[i] == '\'')
				{
					int len = i - start + 1;
					styles.Add(new AppliedStyle(_inlineStringStyle, start, len));
					return;
				}
			}

			// Незакрытая inline-строка - это ошибка. По этому нужно 
			// вручную закрыть строку если ее забыл закрыть программист.
			styles.Add(new AppliedStyle(_inlineStringStyle, start, length - start));
			styles.Add(new AppliedStyle(_errorCharStyle, start, length - start));
		}

		#endregion

		#region DoIdent

		private static void DoIdent(char[] chars, List<AppliedStyle> styles, ref int i,
			ref int length)
		{
			string str;
			SimpleStyle style;
			int len;
			int start = i;
			i += 1;

			for (; i < length; i++)
			{
				if (!Utils.IsLetterOrDigit(chars[i]))
				{
					len = i - start;
					i--;
					str = new string(chars, start, len);
					style = GetStyleByKeyword(str);

#if STYLED_IDENT
					styles.Add(new AppliedStyle(style == null 
						? _identifierStyle : style, start, len));
#else
					if (style != null)
						styles.Add(new AppliedStyle(style, start, len));
#endif // STYLED_IDENT

					return;
				}
			}

			len = length - start;
			str = new string(chars, start, length - start);
			style = GetStyleByKeyword(str);


#if STYLED_IDENT
			styles.Add(new AppliedStyle(style == null 
				? _identifierStyle : style, start, len));
#else
			if (style != null)
				styles.Add(new AppliedStyle(style, start, len));
#endif // STYLED_IDENT
		}

		#endregion

		#region DoInlineString

		private static void DoInlineString(char[] chars, List<AppliedStyle> styles,
			ref int i, int length)
		{
			int start = i;
			i += 1;

			for (; i < length; i++)
			{
				if (chars[i] == '\\')
				{
					char ch = chars[i + 1];

					if (ch == '"' || ch == '\\')
						i++;
				}
				else if (chars[i] == '"')
				{
					int len = i - start + 1;
					styles.Add(new AppliedStyle(_inlineStringStyle, start, len));
					return;
				}
			}

			// Незакрытая inline-строка - это ошибка. По этому нужно 
			// вручную закрыть строку если ее забыл закрыть программист.
			styles.Add(new AppliedStyle(_inlineStringStyle, start, length - start));
			styles.Add(new AppliedStyle(_errorStringStyle, start, length - start));
		}
		
		#endregion

		#region DoMultilineString

		/// <summary>
		/// Разбирает многострочный строковый литерал. (литерал который 
		/// может перетекать на следующую строку.
		/// </summary>
		/// <param name="chars">Символы строки.</param>
		/// <param name="styles">Список стилей.</param>
		/// <param name="i">Текущий индекс сканера.</param>
		/// <param name="length">Длинна сканируемой строки.</param>
		/// <returns>true- если стиль перетекает на следующую строку.</returns>
		private static void DoMultilineString(char[] chars, List<AppliedStyle> styles,
			ref int i, int length, ref int currentState, bool isFirstLine)
		{
			int start = i;

			if (isFirstLine)
				i += 2;

			for (; i < length; i++)
			{
				if (chars[i] == '"')
				{
					if (chars[i + 1] == '"')
						i++;
					else
					{
						int len = i - start + 1;

						styles.Add(new AppliedStyle(_multilineStringStyle, start, len));
						currentState = RsdnState.Default;
						return; // стиль не перетекает на следующую строку.
					}
				}
			}
			
			// Стиль перетекает на следующую строку
			styles.Add(new AppliedStyle(_multilineStringStyle, start, length - start));
			// Стиль перетекает на следующую строку.
			currentState = RsdnState.MultilineString;
		}

		#endregion

		#region DoInlineComment

		private static void DoInlineComment(List<AppliedStyle> styles, ref int i, int length)
		{
			int start = i;
			i = length;

			styles.Add(new AppliedStyle(_inlineCommentStyle, start, length - start));
		}
		
		#endregion

		#region DoMultilineComment

		/// <summary>x
		/// Разбирает многострочный комментарий.
		/// </summary>
		/// <param name="chars">Символы строки.</param>
		/// <param name="styles">Список стилей.</param>
		/// <param name="i">Текущий индекс сканера.</param>
		/// <param name="length">Длинна сканируемой строки.</param>
		/// <returns>true- если стиль перетекает на следующую строку.</returns>
		private static void DoMultilineComment(char[] chars, List<AppliedStyle> styles,
			ref int i, int length, ref int currentState, bool isFirstLine)
		{
			int start = i;
			if (isFirstLine)
				i += 2;

			for (; i < length; i++)
			{
				if (chars[i] == '*' && chars[i + 1] == '/')
				{
					int len = i - start + 2;

					styles.Add(new AppliedStyle(_multilineCommentStyle, start, len));
					currentState = RsdnState.Default; // комментарий окончился на этой строке.
					return;
				}
			}
			
			// Стиль перетекает на следующую строку
			styles.Add(new AppliedStyle(_multilineCommentStyle, start, length - start));
			// Комментарий перетекает на следующую строку.
			currentState = RsdnState.MultilineComment;
		}

		#endregion

		private static AppliedStyle[] _emptyAppliedStyle = new AppliedStyle[0];

		#region Работа со стилями.

		private static SimpleStyle GetStyleByKeyword(string keyword)
		{
			return (SimpleStyle)_keywordMap[keyword];
		}

		static Hashtable _keywordMap = new Hashtable(30);

		#region codeStyle

		static SimpleStyle _codeStyle =
			new SimpleStyle("Code", PartialFont.CreateFont("Forte", 18, true),
			KnownColor.BlueViolet);

		static SimpleStyle _boldStyle =
			new SimpleStyle("Bold");

		static SimpleStyle _italicStyle =
			new SimpleStyle("talic");
		
		#endregion
		
		#region textStyle

		static SimpleStyle _textStyle =
			new SimpleStyle("Text", KnownColor.WindowText);
		
		#endregion

		#region quotingStyle

		static SimpleStyle _quotingStyle =
			new SimpleStyle("Quoting", KnownColor.InfoText, KnownColor.Info);

		static SimpleStyle _quotingBoldStyle =
			new SimpleStyle("QuotingBold", KnownColor.InfoText, KnownColor.Info);

		static SimpleStyle _quotingItalicStyle =
			new SimpleStyle("QuotingItalic", KnownColor.InfoText, KnownColor.Info);

		static SimpleStyle _quotingBoldItalicStyle =
			new SimpleStyle("QuotingBoldItalic", KnownColor.InfoText, KnownColor.Info);

		#endregion

		static SimpleStyle _inlineCommentStyle =
			new SimpleStyle("InlineComment",
			//new Font("Arial", 16, FontStyle.Bold),
			Color.Green);
		static SimpleStyle _multilineCommentStyle =
			new SimpleStyle("MultilineComment", Color.LightGreen);
		static SimpleStyle _inlineStringStyle =
			new SimpleStyle("InlineString", Color.Red);
		static SimpleStyle _multilineStringStyle =
			new SimpleStyle("MultilineString", Color.Red);
		static SimpleStyle _identifierStyle =
			new SimpleStyle("Identifier", Color.Magenta);
		static SimpleStyle _preprocessorStyle =
			new SimpleStyle("PreprocessorStyle", Color.DeepSkyBlue);


		//TODO: Подумать над тем как сделать информацию для токенов.
		static SimpleStyle _errorStringStyle =
			new SimpleStyle("ErrorString", Color.Empty, Color.LightPink, Color.Red);
		static SimpleStyle _errorCharStyle =
			new SimpleStyle("ErrorChar", Color.Empty, Color.LightCoral, Color.Red);
		static SimpleStyle _errorStyle =
			new SimpleStyle("Error", Color.Empty, Color.LightCoral, Color.Red);

		static ImagedStyle _imageStyle =
			new ImagedStyle("Image", GetDisplayImage);

		private static Image _smailImage = Rsdn.Editor.Properties.Resources.Smile;
		private static Image _superImage = Rsdn.Editor.Properties.Resources.Super;
		private static Image _xzImage = Rsdn.Editor.Properties.Resources.Xz;
		private static Image _brokenimgImage = Rsdn.Editor.Properties.Resources.BrokenImg;
		private static Image _biggrinImage = Rsdn.Editor.Properties.Resources.Biggrin;
		private static Image _lolImage = Rsdn.Editor.Properties.Resources.Lol;

		const string Smail = ":)";
		const string Super = ":super:";
		const string Xz = ":xz:";
		const string Biggrin = ":))";
		const string Lol = ":)))";
		
		private static Image GetDisplayImage(string text)
		{
			switch (text)
			{
				case Smail:   return _smailImage;
				case Super:   return _superImage;
				case Xz:      return _xzImage;
				case Biggrin: return _biggrinImage;
				case Lol:     return _lolImage;
				default:      return _brokenimgImage;
			}
		}

		static RsdnStyler()
		{
			//_codeStyle.Font = new Font(Style.DefaultStyle.Font, FontStyle.Bold);
			//_inlineCommentStyle.Font = new Font(Style.DefaultStyle.Font, FontStyle.Bold);

			//_codeStyle.Font = FontEx.CreateFont("Courier New", 12);

			//_inlineCommentStyle.Font = FontEx.CreateFont("Arial", 15, FontStyle.Bold);
			//Courier Tahoma Arial

			#region Инициализация _keywordMap

			_keywordMap["get"] = _codeStyle;
			_keywordMap["set"] = _codeStyle;
			_keywordMap["add"] = _codeStyle;
			_keywordMap["remove"] = _codeStyle;
			_keywordMap["partial"] = _codeStyle;
			_keywordMap["assembly"] = _codeStyle;
			_keywordMap["field"] = _codeStyle;
			_keywordMap["method"] = _codeStyle;
			_keywordMap["where"] = _codeStyle;
			_keywordMap["typevar"] = _codeStyle;
			_keywordMap["type"] = _codeStyle;
			_keywordMap["property"] = _codeStyle;
			_keywordMap["param"] = _codeStyle;

			/*----- C# keywords -----*/
			_keywordMap["abstract"] = _codeStyle;
			_keywordMap["as"] = _codeStyle;
			_keywordMap["base"] = _codeStyle;
			_keywordMap["bool"] = _codeStyle;
			_keywordMap["break"] = _codeStyle;
			_keywordMap["byte"] = _codeStyle;
			_keywordMap["case"] = _codeStyle;
			_keywordMap["catch"] = _codeStyle;
			_keywordMap["char"] = _codeStyle;
			_keywordMap["checked"] = _codeStyle;
			_keywordMap["class"] = _codeStyle;
			_keywordMap["const"] = _codeStyle;
			_keywordMap["continue"] = _codeStyle;
			_keywordMap["decimal"] = _codeStyle;
			_keywordMap["default"] = _codeStyle;
			_keywordMap["delegate"] = _codeStyle;
			_keywordMap["do"] = _codeStyle;
			_keywordMap["double"] = _codeStyle;
			_keywordMap["else"] = _codeStyle;
			_keywordMap["enum"] = _codeStyle;
			_keywordMap["event"] = _codeStyle;
			_keywordMap["explicit"] = _codeStyle;
			_keywordMap["extern"] = _codeStyle;
			_keywordMap["false"] = _codeStyle;
			_keywordMap["finally"] = _codeStyle;
			_keywordMap["fixed"] = _codeStyle;
			_keywordMap["float"] = _codeStyle;
			_keywordMap["for"] = _codeStyle;
			_keywordMap["foreach"] = _codeStyle;
			_keywordMap["goto"] = _codeStyle;
			_keywordMap["if"] = _codeStyle;
			_keywordMap["implicit"] = _codeStyle;
			_keywordMap["in"] = _codeStyle;
			_keywordMap["int"] = _codeStyle;
			_keywordMap["interface"] = _codeStyle;
			_keywordMap["internal"] = _codeStyle;
			_keywordMap["is"] = _codeStyle;
			_keywordMap["lock"] = _codeStyle;
			_keywordMap["long"] = _codeStyle;
			_keywordMap["namespace"] = _codeStyle;
			_keywordMap["new"] = _codeStyle;
			_keywordMap["null"] = _codeStyle;
			_keywordMap["object"] = _codeStyle;
			_keywordMap["operator"] = _codeStyle;
			_keywordMap["out"] = _codeStyle;
			_keywordMap["override"] = _codeStyle;
			_keywordMap["params"] = _codeStyle;
			_keywordMap["private"] = _codeStyle;
			_keywordMap["protected"] = _codeStyle;
			_keywordMap["public"] = _codeStyle;
			_keywordMap["readonly"] = _codeStyle;
			_keywordMap["ref"] = _codeStyle;
			_keywordMap["return"] = _codeStyle;
			_keywordMap["sbyte"] = _codeStyle;
			_keywordMap["sealed"] = _codeStyle;
			_keywordMap["short"] = _codeStyle;
			_keywordMap["sizeof"] = _codeStyle;
			_keywordMap["stackalloc"] = _codeStyle;
			_keywordMap["static"] = _codeStyle;
			_keywordMap["string"] = _codeStyle;
			_keywordMap["struct"] = _codeStyle;
			_keywordMap["switch"] = _codeStyle;
			_keywordMap["this"] = _codeStyle;
			_keywordMap["throw"] = _codeStyle;
			_keywordMap["true"] = _codeStyle;
			_keywordMap["try"] = _codeStyle;
			_keywordMap["typeof"] = _codeStyle;
			_keywordMap["uint"] = _codeStyle;
			_keywordMap["ulong"] = _codeStyle;
			_keywordMap["unchecked"] = _codeStyle;
			_keywordMap["unsafe"] = _codeStyle;
			_keywordMap["ushort"] = _codeStyle;
			_keywordMap["using"] = _codeStyle;
			_keywordMap["virtual"] = _codeStyle;
			_keywordMap["void"] = _codeStyle;
			_keywordMap["while"] = _codeStyle;
			_keywordMap["volatile"] = _codeStyle;

			#endregion
		}

		#endregion
	}
}

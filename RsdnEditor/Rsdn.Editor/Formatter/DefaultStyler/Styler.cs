//#define STYLED_IDENT

using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace Rsdn.Editor.DefaultStyler
{
	class Styler : IStyler
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

			if (_currentState == State.MultilineComment)
			{
				DoMultilineComment(chars, _styles, ref i, len, ref _currentState, false);
				i++;
			}
			else if (_currentState == State.MultilineString)
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
				_currentState == State.MultilineComment
				|| _currentState == State.MultilineString;

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
						currentState = State.Default;
						return; // стиль не перетекает на следующую строку.
					}
				}
			}
			
			// Стиль перетекает на следующую строку
			styles.Add(new AppliedStyle(_multilineStringStyle, start, length - start));
			// Стиль перетекает на следующую строку.
			currentState = State.MultilineString;
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
					currentState = State.Default; // комментарий окончился на этой строке.
					return;
				}
			}
			
			// Стиль перетекает на следующую строку
			styles.Add(new AppliedStyle(_multilineCommentStyle, start, length - start));
			// Комментарий перетекает на следующую строку.
			currentState = State.MultilineComment;
		}

		#endregion

		private static AppliedStyle[] _emptyAppliedStyle = new AppliedStyle[0];

		#region Работа со стилями.

		private static SimpleStyle GetStyleByKeyword(string keyword)
		{
			return (SimpleStyle)_keywordMap[keyword];
		}

		static Hashtable _keywordMap = new Hashtable(30);

		static SimpleStyle _keywordStyle =
			new SimpleStyle("Keyword", Color.Blue);
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

		static Styler()
		{
			//_codeStyle.Font = new Font(Style.DefaultStyle.Font, FontStyle.Bold);
			//_inlineCommentStyle.Font = new Font(Style.DefaultStyle.Font, FontStyle.Bold);

			//_keywordStyle.Font = FontEx.CreateFont("Tahoma", 16, FontStyle.Bold);
			
			//_inlineCommentStyle.Font = new Font("Arial", 15, FontStyle.Bold);
			//Courier Tahoma Arial

			#region Инициализация _keywordMap

			_keywordMap["get"] = _keywordStyle;
			_keywordMap["set"] = _keywordStyle;
			_keywordMap["add"] = _keywordStyle;
			_keywordMap["remove"] = _keywordStyle;
			_keywordMap["partial"] = _keywordStyle;
			_keywordMap["assembly"] = _keywordStyle;
			_keywordMap["field"] = _keywordStyle;
			_keywordMap["method"] = _keywordStyle;
			_keywordMap["where"] = _keywordStyle;
			_keywordMap["typevar"] = _keywordStyle;
			_keywordMap["type"] = _keywordStyle;
			_keywordMap["property"] = _keywordStyle;
			_keywordMap["param"] = _keywordStyle;

			/*----- C# keywords -----*/
			_keywordMap["abstract"] = _keywordStyle;
			_keywordMap["as"] = _keywordStyle;
			_keywordMap["base"] = _keywordStyle;
			_keywordMap["bool"] = _keywordStyle;
			_keywordMap["break"] = _keywordStyle;
			_keywordMap["byte"] = _keywordStyle;
			_keywordMap["case"] = _keywordStyle;
			_keywordMap["catch"] = _keywordStyle;
			_keywordMap["char"] = _keywordStyle;
			_keywordMap["checked"] = _keywordStyle;
			_keywordMap["class"] = _keywordStyle;
			_keywordMap["const"] = _keywordStyle;
			_keywordMap["continue"] = _keywordStyle;
			_keywordMap["decimal"] = _keywordStyle;
			_keywordMap["default"] = _keywordStyle;
			_keywordMap["delegate"] = _keywordStyle;
			_keywordMap["do"] = _keywordStyle;
			_keywordMap["double"] = _keywordStyle;
			_keywordMap["else"] = _keywordStyle;
			_keywordMap["enum"] = _keywordStyle;
			_keywordMap["event"] = _keywordStyle;
			_keywordMap["explicit"] = _keywordStyle;
			_keywordMap["extern"] = _keywordStyle;
			_keywordMap["false"] = _keywordStyle;
			_keywordMap["finally"] = _keywordStyle;
			_keywordMap["fixed"] = _keywordStyle;
			_keywordMap["float"] = _keywordStyle;
			_keywordMap["for"] = _keywordStyle;
			_keywordMap["foreach"] = _keywordStyle;
			_keywordMap["goto"] = _keywordStyle;
			_keywordMap["if"] = _keywordStyle;
			_keywordMap["implicit"] = _keywordStyle;
			_keywordMap["in"] = _keywordStyle;
			_keywordMap["int"] = _keywordStyle;
			_keywordMap["interface"] = _keywordStyle;
			_keywordMap["internal"] = _keywordStyle;
			_keywordMap["is"] = _keywordStyle;
			_keywordMap["lock"] = _keywordStyle;
			_keywordMap["long"] = _keywordStyle;
			_keywordMap["namespace"] = _keywordStyle;
			_keywordMap["new"] = _keywordStyle;
			_keywordMap["null"] = _keywordStyle;
			_keywordMap["object"] = _keywordStyle;
			_keywordMap["operator"] = _keywordStyle;
			_keywordMap["out"] = _keywordStyle;
			_keywordMap["override"] = _keywordStyle;
			_keywordMap["params"] = _keywordStyle;
			_keywordMap["private"] = _keywordStyle;
			_keywordMap["protected"] = _keywordStyle;
			_keywordMap["public"] = _keywordStyle;
			_keywordMap["readonly"] = _keywordStyle;
			_keywordMap["ref"] = _keywordStyle;
			_keywordMap["return"] = _keywordStyle;
			_keywordMap["sbyte"] = _keywordStyle;
			_keywordMap["sealed"] = _keywordStyle;
			_keywordMap["short"] = _keywordStyle;
			_keywordMap["sizeof"] = _keywordStyle;
			_keywordMap["stackalloc"] = _keywordStyle;
			_keywordMap["static"] = _keywordStyle;
			_keywordMap["string"] = _keywordStyle;
			_keywordMap["struct"] = _keywordStyle;
			_keywordMap["switch"] = _keywordStyle;
			_keywordMap["this"] = _keywordStyle;
			_keywordMap["throw"] = _keywordStyle;
			_keywordMap["true"] = _keywordStyle;
			_keywordMap["try"] = _keywordStyle;
			_keywordMap["typeof"] = _keywordStyle;
			_keywordMap["uint"] = _keywordStyle;
			_keywordMap["ulong"] = _keywordStyle;
			_keywordMap["unchecked"] = _keywordStyle;
			_keywordMap["unsafe"] = _keywordStyle;
			_keywordMap["ushort"] = _keywordStyle;
			_keywordMap["using"] = _keywordStyle;
			_keywordMap["virtual"] = _keywordStyle;
			_keywordMap["void"] = _keywordStyle;
			_keywordMap["while"] = _keywordStyle;
			_keywordMap["volatile"] = _keywordStyle;

			#endregion
		}

		#endregion
	}
}

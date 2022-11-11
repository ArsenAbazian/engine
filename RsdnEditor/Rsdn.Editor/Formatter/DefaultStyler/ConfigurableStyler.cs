using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Xml;
using System.Xml.XPath;
using Rsdn.Editor.Lexing;

namespace Rsdn.Editor.DefaultStyler
{
    public class ConfigurableStyler : IStyler
    {
		private static IConfigurableLexer lexer;
		private Dictionary<Int32,SimpleStyle> styles;
		
        public ConfigurableStyler()
        {
			
        }


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


		private string _lexerCode;
		public string LexerCode
		{
			get { return _lexerCode; }
			set
			{
				ILexer lex = LexerManager.Instance.GetLexer(value);
				
				if (lex == null)
					throw new ArgumentException("Lexer not found!", "value");

				if (!(lex is IConfigurableLexer))
					throw new NotSupportedException("Lexer type is not supported!");

				lexer = (IConfigurableLexer)lex;
				styles = ((SimpleStyleProcessor)lexer.Syntax.StyleProcessor).StyleMap;
				_lexerCode = value;
			}
		}


        List<AppliedStyle> _styles = new List<AppliedStyle>(20);
        private static AppliedStyle[] _emptyAppliedStyle = new AppliedStyle[0];

     
        public void GetStyles(int rowIndex)
        {
            DocumentRow currentRow = _rows[rowIndex];
            currentRow.StartStylerState = _currentState;
            lexer.State = _currentState;
            lexer.Process(currentRow.Text, 0);
            List<Token> tokens = lexer.Tokens;

            AppliedStyle[] arr = null;

            if (tokens != null)
            {
               arr = new AppliedStyle[tokens.Count];

               for (int i = 0; i < tokens.Count; i++)
               {
                   Token t = tokens[i];
				   arr[i] = new AppliedStyle(styles[t.Style], t.Position, t.Length);
               }
            }
                
            if (arr != null)
                currentRow.AppliedStyles = arr;
            else
                currentRow.AppliedStyles = _emptyAppliedStyle;

            _currentState = lexer.State;
            currentRow.IsContinueStylingOnNextLine = lexer.ContinueState;
        }

    }
}

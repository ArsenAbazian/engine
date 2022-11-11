using System;
using System.Drawing;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using Rsdn.Editor.DefaultStyler;

namespace Rsdn.Editor.Lexing
{
	public sealed class SimpleStyleProcessor : IStyleProcessor
	{
		private const string TRUE = "true";

		public SimpleStyleProcessor()
		{
			_styleMap = new Dictionary<Int32,SimpleStyle>();
		}


		public void ProcessStyle(XmlReader reader, int styleCode)
		{
			string stub;
			SimpleStyle style = new SimpleStyle(reader["Key"],
				PartialFont.CreateFont(reader["FontName"],
					(stub = reader["FontSize"]) != null ? new Nullable<Single>(Single.Parse(stub)) : 12,
					(stub = reader["Bold"]) != null ? new Nullable<Boolean>(TRUE == stub) : null,
					(stub = reader["Italic"]) != null ? new Nullable<Boolean>(TRUE == stub) : null,
					(stub = reader["Underline"]) != null ? new Nullable<Boolean>(TRUE == stub) : null,
					(stub = reader["Strikeout"]) != null ? new Nullable<Boolean>(TRUE == stub) : null),
				(stub = reader["ForeColor"]) != null ? Color.FromName(stub) : Color.Empty,
				(stub = reader["BackColor"]) != null ? Color.FromName(stub) : Color.Empty);				
			_styleMap.Add(styleCode, style);			
		}


		private Dictionary<Int32,SimpleStyle> _styleMap;
		public Dictionary<Int32,SimpleStyle> StyleMap
		{
			get { return _styleMap; }
		}
	}
}
